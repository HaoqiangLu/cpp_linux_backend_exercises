#include "HttpHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "StaticFileSender.h"
#include "TcpConnection.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

namespace {
constexpr size_t kMaxRequestLine = 8 * 1024;    // 请求行上限 8KB
constexpr int kMaxHeaderCount = 100;    // Header 数量上限
}   // namespace

HttpHandler::HttpHandler(const std::string& docRoot, const std::string& strategy)
    : docRoot_(docRoot), strategy_(strategy) {
}

void HttpHandler::onRequest(const std::shared_ptr<TcpConnection>& conn, const std::string& input) {
    HttpRequest req;
    HttpResponse resp;

    if (!parseRequest(input, req)) {
        handleError(conn, 400, "Bad Request");
        return;
    }

    LOG_INFO("%s %s %s", req.method().c_str(), req.uri().c_str(), req.version().c_str());

    const std::string& method = req.method();
    if (method == "GET" || method == "HEAD") {
        // 静态文件：header 与 body 分离发送，body 走 StaticFileSender 策略
        serveStatic(conn, req, method == "HEAD");
    } else {
        buildResponse(req, resp);   // POST / OPTIONS / 405
        conn->send(resp.serialize());
    }

    if (!req.keepAlive()) {
        conn->shutdown();   // 短连接：发送完毕后半关闭
    }
}

bool HttpHandler::parseRequest(const std::string& input, HttpRequest& req) {
    const std::string kDelim = "\r\n";
    size_t start = 0;

    // 1 请求行
    size_t end = input.find(kDelim, start);
    if (end == std::string::npos) {
        return false;
    }
    std::string requestLine = input.substr(start, end - start);
    if (requestLine.size() > kMaxRequestLine) {
        return false;   // 请求行过长
    }
    if (!req.parseRequestLine(requestLine)) {
        return false;
    }
    start = end + kDelim.size();

    // 2 Header 行，直到空行（空行也要交给 parseHeader 完成状态转移）
    int headerCount = 0;
    while (true) {
        end = input.find(kDelim, start);
        if (end == std::string::npos) {
            return false;   // 头部未正常以空行结束
        }
        std::string line = input.substr(start, end - start);
        start = end + kDelim.size();
        if (!req.parseHeader(line)) {
            return false;
        }
        if (line.empty()) break;    // 空行：头部结束
        if (++headerCount > kMaxHeaderCount) {
            return false;   // Header 过多
        }
    }

    // 3 Body（仅当状态机进入 ParsingBody）
    if (req.state() == HttpRequest::State::ParsingBody) {
        size_t contentLength = 0;
        try {
            contentLength = std::stoul(req.header("content-length"));
        } catch (...) {
            contentLength = 0;
        }
        req.parseBody(input.substr(start, contentLength));
    }

    return req.state() == HttpRequest::State::Complete;
}

void HttpHandler::buildResponse(const HttpRequest& req, HttpResponse& resp) {
    const std::string& method = req.method();

    // GET / HEAD 的静态文件已在 onRequest 中路由到 serveStatic，此处只处理非静态方法
    if (method == "POST") {
        resp.setBody(req.body());   // 回显 body
        resp.setHeader("Content-Type", "text/plain");
    } else if (method == "OPTIONS") {
        resp = HttpResponse(204);   // 用整体赋值，保证 reasonPhrase_ 正确
        resp.setHeader("Allow", "GET, POST, HEAD, OPTIONS");
    } else {
        resp = HttpResponse(405);
        resp.setHeader("Allow", "GET, POST, HEAD, OPTIONS");
    }

    resp.setHeader("Connection", req.keepAlive() ? "keep-alive" : "close");
}

void HttpHandler::serveStatic(const std::shared_ptr<TcpConnection>& conn,
                              const HttpRequest& req, bool headOnly) {
    const std::string& uri = req.uri();
    // 注意：uri 已在 HttpRequest::parseRequestLine 中 urlDecode 过，此处不可再解码
    std::string path = docRoot_ + uri;
    if (uri == "/" || uri.empty()) {
        path += "index.html";   // 默认首页
    }

    const char* connVal = req.keepAlive() ? "keep-alive" : "close";

    // 路径合法性检查：禁止目录穿越（解码后再查，可拦截 %2e%2e%2f）
    if (path.find("..") != std::string::npos) {
        HttpResponse resp(403);
        resp.setHeader("Content-Type", "text/html");
        resp.setHeader("Connection", connVal);
        resp.setBody("<html><body><h1>403 Forbidden</h1></body></html>");
        conn->send(resp.serialize());
        return;
    }

    // 用 stat 探测：不存在或非常规文件 -> 404
    struct stat st{};
    if (::stat(path.c_str(), &st) != 0 || !S_ISREG(st.st_mode)) {
        HttpResponse resp(404);
        resp.setHeader("Content-Type", "text/html");
        resp.setHeader("Connection", connVal);
        std::ifstream nf(docRoot_ + "/404.html", std::ios::binary);
        if (nf) {
            std::ostringstream ss;
            ss << nf.rdbuf();
            resp.setBody(ss.str());
        } else {
            resp.setBody("<html><body><h1>404 Not Found</h1></body></html>");
        }
        conn->send(resp.serialize());
        return;
    }

    // 构造 header-only 响应：Content-Length 用文件大小，body 留空随后单独发送
    HttpResponse resp(200);
    std::string ext;
    size_t dot = path.find_last_of('.');
    if (dot != std::string::npos) {
        ext = path.substr(dot + 1);
    }
    resp.setHeader("Content-Type", HttpResponse::mimeFromExt(ext));
    resp.setHeader("Content-Length", std::to_string(st.st_size));
    resp.setHeader("Connection", connVal);
    conn->send(resp.serialize());   // 只发 header + 空行（body 为空）

    if (headOnly) {
        return;   // HEAD 不返回 body，但保留 Content-Length
    }

    // body：按策略选择发送方式，均经 conn->send 保证线程安全与发送顺序
    if (strategy_ == "mmap") {
        StaticFileSender::sendWithMmap(conn, path);   // 优化：mmap 减少一次内核->用户拷贝
    } else {
        StaticFileSender::sendWithReadWrite(conn, path);   // 基线：普通 read/write
    }
}

void HttpHandler::handleError(const std::shared_ptr<TcpConnection>& conn, int code, const std::string& reason) {
    HttpResponse resp(code);    // 直接构造，reasonPhrase_ 自动正确
    resp.setHeader("Content-Type", "text/html");
    resp.setHeader("Connection", "close");
    resp.setBody("<html><body><h1>" + std::to_string(code) + " " + reason + "</h1></body></html>");
    conn->send(resp.serialize());
}