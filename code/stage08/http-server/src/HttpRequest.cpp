#include "HttpRequest.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <unordered_set>

namespace {
// 内部工具：转小写
std::string toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );
    return r;
}

// 内部工具：去除首尾空白字符（含 \r）
std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) {
        return "";
    }
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}
}   // namespace

bool HttpRequest::parseRequestLine(const std::string& line) {
    std::istringstream iss(line);
    std::string method, uri, version;

    // 按空格分割 "METHOD URI VERSION", 三段缺一不可
    if (!(iss >> method >> uri >> version)) {
        state_ = State::Error;
        return false;
    }

    // 校验方法：仅支持 GET/POST/HEAD/OPTIONS
    static const std::unordered_set<std::string> SUPP{"GET", "POST", "HEAD", "OPTIONS"};
    if (!SUPP.contains(method)) {
        state_ = State::Error;
        return false;
    }
    method_ = method;
    uri_ = urlDecode(uri);  // URI 需 URL 解码
    version_ = version;
    state_ = State::ParsingHeaders; // 状态转移：开始解析 Header

    return true;
}

bool HttpRequest::parseHeader(const std::string& line) {
    std::string content = trim(line);
    if (content.empty()) {
        // 空行表示 Header 结束
        std::string cl = header("content-length");
        long contentLength = 0;
        if (!cl.empty()) {
            try {
                contentLength = std::stol(cl);
            } catch (...) {
                contentLength = 0;
            }
        }
        // 有 Body 则进入 ParsingBody，否则直接完成
        state_ = (contentLength > 0) ? State::ParsingBody : State::Complete;
        return true;
    }

    auto pos = content.find(':');
    if (pos == std::string::npos) {
        state_ = State::Error;  // 非法 Header 行
        return false;
    }
    std::string key = toLower(content.substr(0, pos));  // key 统一转小写
    std::string value = trim(content.substr(pos + 1));
    headers_[key] = value;

    return true;
}

void HttpRequest::parseBody(const std::string& body) {
    body_ = body;
    state_ = State::Complete;
}

std::string HttpRequest::urlDecode(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '%' && i + 2 < str.size() &&
            std::isxdigit(str[i + 1]) &&
            std::isxdigit(str[i + 2])) {
            // %xx 两位十六进制转字符
            std::string hex = str.substr(i + 1, 2);
            result.push_back(std::stoi(hex, nullptr, 16));
            i += 2;
        } else if (str[i] == '+') {
            result.push_back(' ');   // + 转为空格
        } else {
            result.push_back(str[i]);
        }
    }
    return result;
}

std::string HttpRequest::header(const std::string& key) const {
    auto it = headers_.find(toLower(key));
    return it != headers_.end() ? it->second : std::string();
}

bool HttpRequest::keepAlive() const {
    std::string conn = toLower(header("connection"));
    if (version_ == "HTTP/1.1") {
        // HTTP/1.1 默认长连接，除非显示 Connection: close
        return conn != "close";
    }
    // HTTP/1.0 默认短连接，除非显示 Connection: keep-alive
    return conn == "keep-alive";
}