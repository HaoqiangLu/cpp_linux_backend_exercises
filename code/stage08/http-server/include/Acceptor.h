#pragma once
#include <functional>

class EventLoop;
class Channel;

class Acceptor {
public:
    using NewConnCallback = std::function<void(int fd)>;
    Acceptor(EventLoop* loop, int port);
    ~Acceptor();
    void setNewConnCallback(NewConnCallback cb) { newConnCb_ = std::move(cb); }
    void listen();

private:
    void handleRead();
    EventLoop* loop_;
    Channel* acceptChannel_;
    int listenFd_;
    NewConnCallback newConnCb_;
};