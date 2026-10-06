#include "HttpRequest.h"
#include <cstddef>
#include <sstream>

static const std::string EMPTY;

const std::string& HttpRequest::header(const std::string& key) const {
    auto it = headers_.find(key);
    return it != headers_.end() ? it->second : EMPTY;
}

bool HttpRequest::parseRequestLine(const std::string& line) {
    std::istringstream iss(line);
    if (!(iss >> method_ >> uri_ >> version_)) {
        state_ = ParseState::Error;
        return false;
    }
    state_ = ParseState::Headers;
    return true;
}

bool HttpRequest::parseHeaderLine(const std::string& line) {
    if (line.empty()) {
        state_ = ParseState::Body;
        return true;
    }
    auto pos = line.find(':');
    if (pos == std::string::npos) {
        state_ = ParseState::Error;
        return false;
    }
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    while (!value.empty() && value.front() == ' ') {
        value.erase(value.begin());
    }
    headers_[key] = value;
    return true;
}

std::string HttpRequest::urlDecode(const std::string& s) {
    std::string result;
    result.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '+') {
            result += ' ';
        } else if (s[i] == '%' && i + 2 < s.size()) {
            auto hex = s.substr(i + 1, 2);
            char ch = static_cast<char>(std::stoul(hex, nullptr, 16));
            result += ch;
            i += 2;
        } else {
            result += s[i];
        }
    }
    return result;
}