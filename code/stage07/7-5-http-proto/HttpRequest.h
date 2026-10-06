#pragma once
#include <string>
#include <unordered_map>

enum class ParseState {
    RequestLine,
    Headers,
    Body,
    Complete,
    Error
};

class HttpRequest {
public:
    const std::string& method() const { return method_; }
    const std::string& uri() const { return uri_; }
    const std::string& version() const { return version_; }
    const std::string& header(const std::string& key) const;
    const std::string& body() const { return body_; }
    ParseState state() const { return state_; }

    bool parseRequestLine(const std::string& line);
    bool parseHeaderLine(const std::string& line);
    void setBody(const std::string& b) { body_ = b; }
    void setState(ParseState s) { state_ = s; }

    static std::string urlDecode(const std::string& s);

private:
    std::string method_, uri_, version_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
    ParseState state_{ParseState::RequestLine};
};