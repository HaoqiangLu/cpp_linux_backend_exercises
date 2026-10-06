#pragma once
#include <string>
#include <unordered_map>

class HttpResponse {
public:
    explicit HttpResponse(int statusCode = 200);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    void setContentType(const std::string& mime);
    std::string serialize() const;

    static std::string mimeFromExt(const std::string& ext);

private:
    int statusCode_;
    std::string reason_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
};