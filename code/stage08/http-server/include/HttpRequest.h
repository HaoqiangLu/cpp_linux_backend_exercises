#pragma once
#include <string>
#include <unordered_map>

class HttpRequest {
public:
    enum class State {
        ParsingRequestLine,
        ParsingHeaders,
        ParsingBody,
        Complete,
        Error
    };

    HttpRequest() : state_(State::ParsingRequestLine) {}

    bool parseRequestLine(const std::string& line);
    bool parseHeader(const std::string& line);
    void parseBody(const std::string& body);
    static std::string urlDecode(const std::string& str);

    const std::string& method() const { return method_; }
    const std::string& uri() const { return uri_; }
    const std::string& version() const { return version_; }
    const std::string& body() const { return body_; }
    std::string header(const std::string& key) const;
    bool keepAlive() const;
    State state() const { return state_; }
    void setState(State s) { state_ = s; }

private:
    State state_;
    std::string method_;
    std::string uri_;
    std::string version_;
    std::string body_;
    std::unordered_map<std::string, std::string> headers_;
};