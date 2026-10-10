#include "HttpRequest.h"
#include "HttpResponse.h"
#include <cassert>
#include <iostream>

void testParseRequestLine() {
    HttpRequest req;
    assert(req.parseRequestLine("GET /index.html HTTP/1.1"));
    assert(req.method() == "GET");
    assert(req.uri() == "/index.html");
    assert(req.version() == "HTTP/1.1");
    std::cout << "[PASS] testParseRequestLine\n";
}

void testUrlDecode() {
    assert(HttpRequest::urlDecode("hello%20world") == "hello world");
    assert(HttpRequest::urlDecode("a+b") == "a b");
    assert(HttpRequest::urlDecode("%E4%BD%A0%E5%A5%BD") == "你好");
    std::cout << "[PASS] testUrlDecode\n";
}

void testMime() {
    assert(HttpResponse::mimeFromExt("html") == "text/html");
    assert(HttpResponse::mimeFromExt("png") == "image/png");
    assert(HttpResponse::mimeFromExt("css") == "text/css");
    assert(HttpResponse::mimeFromExt("js") == "application/javascript");
    assert(HttpResponse::mimeFromExt("unknown_ext") == "application/octet-stream");
    std::cout << "[PASS] testMime\n";
}

void testResponseSerialize() {
    HttpResponse resp(200);
    resp.setHeader("Content-Type", "text/plain");
    resp.setBody("hello");
    std::string out = resp.serialize();
    assert(out.find("HTTP/1.1 200 OK\r\n") != std::string::npos);
    assert(out.find("Content-Length: 5\r\n") != std::string::npos);
    assert(out.find("\r\n\r\nhello") != std::string::npos);
    std::cout << "[PASS] testResponseSerialize\n";
}

void testKeepAlive() {
    HttpRequest req;
    req.parseRequestLine("GET / HTTP/1.1");
    req.parseHeader("Connection: keep-alive");
    assert(req.keepAlive() == true);

    HttpRequest req2;
    req2.parseRequestLine("GET / HTTP/1.1");
    req2.parseHeader("Connection: close");
    assert(req2.keepAlive() == false);
    std::cout << "[PASS] testKeepAlive\n";
}

int main() {
    testParseRequestLine();
    testUrlDecode();
    testMime();
    testResponseSerialize();
    testKeepAlive();
    std::cout << "\n=== all tests passed ===\n";
    return 0;
}