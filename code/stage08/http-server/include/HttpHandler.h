#pragma once
#include <memory>
#include <string>

class TcpConnection;
class HttpRequest;
class HttpResponse;

class HttpHandler {
public:
    explicit HttpHandler(const std::string& docRoot = "./www",
                         const std::string& strategy = "readwrite");
    void onRequest(const std::shared_ptr<TcpConnection>& conn, const std::string& input);

private:
    bool parseRequest(const std::string& input, HttpRequest& req);
    void buildResponse(const HttpRequest& req, HttpResponse& resp);
    // 静态文件：header 与 body 分离发送，body 走 StaticFileSender 策略（8.6 压测对比）
    void serveStatic(const std::shared_ptr<TcpConnection>& conn, const HttpRequest& req, bool headOnly);
    void handleError(const std::shared_ptr<TcpConnection>& conn, int code, const std::string& reasion);

    std::string docRoot_;
    std::string strategy_;   // 静态文件发送策略: readwrite | mmap
};