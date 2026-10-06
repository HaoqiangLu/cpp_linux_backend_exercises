#include "HttpRequest.h"
#include "HttpResponse.h"
#include <iostream>

int main() {
    std::cout << "=== 练习2: HTTP 请求解析 ===" << std::endl;
    HttpRequest req;
    req.parseRequestLine("GET /index.html HTTP/1.1");
    std::cout << "Method:  " << req.method() << std::endl;
    std::cout << "URI:     " << req.uri() << std::endl;
    std::cout << "Version: " << req.version() << std::endl;

    req.parseHeaderLine("Host: localhost");
    req.parseHeaderLine("User-Agent: curl/8.0");
    req.parseHeaderLine("Accept: text/html");
    req.parseHeaderLine("");
    std::cout << "Host:    " << req.header("Host") << std::endl;
    std::cout << "Accept:  " << req.header("Accept") << std::endl;

    std::cout << "\n=== 练习4: URL 解码 ===" << std::endl;
    std::cout << "%E4%BD%A0%E5%A5%BD -> " << HttpRequest::urlDecode("%E4%BD%A0%E5%A5%BD") << std::endl;
    std::cout << "hello+world       -> " << HttpRequest::urlDecode("hello+world") << std::endl;
    std::cout << "/search%3Fq%3Dtest -> " << HttpRequest::urlDecode("/search%3Fq%3Dtest") << std::endl;

    std::cout << "\n=== 练习4: MIME 映射 ===" << std::endl;
    std::cout << "html -> " << HttpResponse::mimeFromExt("html") << std::endl;
    std::cout << "css  -> " << HttpResponse::mimeFromExt("css") << std::endl;
    std::cout << "js   -> " << HttpResponse::mimeFromExt("js") << std::endl;
    std::cout << "png  -> " << HttpResponse::mimeFromExt("png") << std::endl;
    std::cout << "xyz  -> " << HttpResponse::mimeFromExt("xyz") << std::endl;

    std::cout << "\n=== 练习3: HTTP 响应构造 ===" << std::endl;
    HttpResponse resp(200);
    resp.setContentType("text/html");
    resp.setHeader("Connection", "keep-alive");
    resp.setBody("<h1>OK</h1>");
    std::cout << resp.serialize() << std::endl;

    std::cout << "\n=== 404 响应 ===" << std::endl;
    HttpResponse notFound(404);
    notFound.setContentType("text/html");
    notFound.setBody("<h1>404 Not Found</h1>");
    std::cout << notFound.serialize() << std::endl;
}