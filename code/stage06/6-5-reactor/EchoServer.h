#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <map>

class EventLoop;
class Acceptor;
class Channel;

// 练习3：主从 Reactor 模型
struct TcpConnection {
    int fd;
    Channel* channel;
    EventLoop* loop;
    char buf[4096];
};

class EchoServer {
public:
    EchoServer(int port, int subReactorCount = 3);
    ~EchoServer();
    void start();
    void stop();

private:
    void onNewConnection(int fd);
    void handleRead(TcpConnection* conn);
    void removeConnection(TcpConnection* conn);

    std::unique_ptr<EventLoop> mainLoop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::vector<std::unique_ptr<EventLoop>> subLoops_;
    std::vector<std::thread> threads_;
    int nextSub_{0};
    std::mutex connMtx_;
    std::map<int, std::unique_ptr<TcpConnection>> connections_; // fd -> conn
};
