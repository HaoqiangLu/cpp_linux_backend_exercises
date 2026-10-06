#include "HttpResponse.h"
#include <string>
#include <unordered_map>

HttpResponse::HttpResponse(int code) : statusCode_(code) {
    static const std::unordered_map<int, std::string> reasons = {
        {200, "OK"}, {204, "No Content"},
        {301, "Moved Permanently"}, {302, "Found"}, {304, "Not Modified"},
        {400, "Bad Request"}, {403, "Forbidden"}, {404, "Not Found"},
        {500, "Internal Server Error"}, {502, "Bad Gateway"}, {503, "Service Unavailable"}
    };
    auto it = reasons.find(code);
    reason_ = (it != reasons.end()) ? it->second : "Unknown";
}

void HttpResponse::setHeader(const std::string& k, const std::string& v) {
    headers_[k] = v;
}

void HttpResponse::setBody(const std::string& body) {
    body_ = body;
    setHeader("Content-Length", std::to_string(body.size()));
}

void HttpResponse::setContentType(const std::string& mime) {
    setHeader("Content-Type", mime);
}

std::string HttpResponse::serialize() const {
    std::string result = "HTTP/1.1 " + std::to_string(statusCode_) + " " + reason_ + "\r\n";
    for (const auto& [key, value] : headers_) {
        result += key + ": " + value + "\r\n";
    }
    result += "\r\n";
    result += body_;
    return result;
}

std::string HttpResponse::mimeFromExt(const std::string& ext) {
    static const std::unordered_map<std::string, std::string> mimeMap = {
        {"html", "text/html"},
        {"htm", "text/html"},
        {"css", "text/css"},
        {"js", "application/javascript"},
        {"json", "application/json"},
        {"png", "image/png"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"gif", "image/gif"},
        {"txt", "text/plain"},
        {"xml", "application/xml"},
        {"pdf", "application/pdf"}
    };
    auto it = mimeMap.find(ext);
    return (it != mimeMap.end()) ? it->second : "application/octet-stream";
}