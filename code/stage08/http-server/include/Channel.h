#pragma once
#include <functional>

class EventLoop;

// 封装 fd + 关注事件 + 回调
class Channel {
public:
    using EventCallback = std::function<void()>;
    Channel(EventLoop* loop, int fd);
    void setReadCallback(EventCallback cb) { readCb_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCb_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCb_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCb_ = std::move(cb); }
    void enableReading(bool on = true);
    void enableWriting(bool on = true);
    void handleEvent(int revents);  // 由 EventLoop 调用
    int fd() const { return fd_; }
    int events() const { return events_; }

private:
    EventLoop* loop_;
    int fd_;
    int events_{0}; // 当前关注的 epoll 事件掩码
    EventCallback readCb_, writeCb_, closeCb_, errorCb_;
};