#pragma once
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

class EventLoop;
class Acceptor;
class TcpConnection;
class ThreadPool;

class HttpServer {
public:
    HttpServer(int port, int subReactorCount = 3, int threadPoolSize = 4);
    ~HttpServer();
    void start();
    void stop();
    void setDocumentRoot(const std::string& root) { docRoot_ = root; }
    void setConnectionTimeoutMs(int64_t timeoutMs) { connTimeoutMs_ = timeoutMs; }
    void setStaticFileStrategy(const std::string& s) { staticStrategy_ = s; }

private:
    void onNewConnection(int fd);
    void onRequest(const std::shared_ptr<TcpConnection>& conn, const std::string& msg);
    void onClose(const std::shared_ptr<TcpConnection>& conn);
    // 空闲超时：挂载自校验定时器（捕获 weak_ptr + 身份校验），
    // 避免陈旧定时器误删后来复用同一 fd 的新连接（旧实现的崩溃根因）
    void scheduleIdleClose(EventLoop* loop, int fd, std::weak_ptr<TcpConnection> weak, int64_t delayMs);

    int port_;
    std::string docRoot_{"./www"};
    std::string staticStrategy_{"readwrite"};   // 静态文件发送策略: readwrite | mmap
    int64_t connTimeoutMs_{30 * 1000};  // 连接空闲超时，可由配置注入
    std::unique_ptr<EventLoop> mainLoop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::vector<std::unique_ptr<EventLoop>> subLoops_;
    std::vector<std::thread> subThreads_;
    std::unique_ptr<ThreadPool> threadPool_;
    std::atomic<int> nextSub_{0};

    std::mutex connMtx_;
    std::unordered_map<int, std::shared_ptr<TcpConnection>> connections_;
};