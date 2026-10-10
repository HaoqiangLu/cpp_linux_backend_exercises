#include "HttpResponse.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <unordered_map>

namespace {
// 状态码 -> 原因短语映射
std::string reasonFromCode(int code) {
    switch (code) {
        case 200: return "OK";
        case 204: return "No Content";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 304: return "Not Modified";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 408: return "Request Timeout";
        case 414: return "URI Too Long";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 503: return "Service Unavailable";
        case 505: return "HTTP Version Not Supported";
        default: return "Unknown";
    }
}

std::string toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); }
    );
    return r;
}
}   // namespace

HttpResponse::HttpResponse(int statusCode) : statusCode_(statusCode) {
    reasonPhrase_ = reasonFromCode(statusCode_);
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    headers_[key] = value;
}

void HttpResponse::setBody(const std::string& body) {
    body_ = body;
    // 自动计算并设置 Content-Length
    headers_["Content-Length"] = std::to_string(body_.size());
}

std::string HttpResponse::serialize() const {
    // reasonPhrase_ 可能因为 setStatusCode 未刷新而失真，兜底按状态码重算
    const std::string reason = reasonPhrase_.empty() ? reasonFromCode(statusCode_) : reasonPhrase_;

    std::ostringstream oss;
    oss << "HTTP/1.1 " << statusCode_ << " " << reason << "\r\n";

    bool hasContentLength = false;
    for (const auto& [k, v] : headers_) {
        if (k == "Content-Length") {
            hasContentLength = true;
        }
        oss << k << ": " << v << "\r\n";
    }
    // 若从未 setBody（空 body），仍补一个 Content-Length: 0，保证响应合法
    if (!hasContentLength) {
        oss << "Content-Length: " << body_.size() << "\r\n";
    }

    oss << "\r\n";  // 头部与 body 之间的空行
    oss << body_;
    return oss.str();
}

std::string HttpResponse::mimeFromExt(const std::string& ext) {
    static const std::unordered_map<std::string, std::string> mimeMap = {
        {"html", "text/html"},
        {"htm", "text/html"},
        {"css", "text/css"},
        {"js", "application/javascript"},
        {"png", "image/png"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"gif", "image/gif"},
        {"ico", "image/x-icon"},
        {"txt", "text/plain"},
        {"json", "application/json"},
        {"svg", "image/svg+xml"},
        {"pdf", "application/pdf"},
    };
    // 归一化：去掉可能的前导 '.'，并转小写
    std::string e = toLower(ext);
    if (!e.empty() && e.front() == '.') {
        e.erase(0, 1);
    }
    auto it = mimeMap.find(e);
    return it != mimeMap.end() ? it->second : "application/octet-stream";
}