#pragma once
#include <string>
#include <unordered_map>


class HttpResponse {
public:
    explicit HttpResponse(int statusCode = 200);

    void setStatusCode(int code) { statusCode_ = code; }
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    std::string serialize() const;
    static std::string mimeFromExt(const std::string& ext);
    int statusCode() const { return statusCode_; }
    const std::string& body() const { return body_; }

private:
    int statusCode_;
    std::string reasonPhrase_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
};