#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

class EventLoop;
class Channel;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(EventLoop* loop, int fd);
    ~TcpConnection();
    void send(const std::string& data);
    void shutdown();
    int fd() const { return fd_; }
    void touch() { lastActiveMs_ = nowMs(); }   // 刷新最后活动时间（空闲超时判定用）
    int64_t lastActiveMs() const { return lastActiveMs_; }
    void setConnectionCallback(std::function<void(const std::shared_ptr<TcpConnection>&)> cb);
    void setMessageCallback(std::function<void(const std::shared_ptr<TcpConnection>&, const std::string&)> cb);
    void setCloseCallback(std::function<void(const std::shared_ptr<TcpConnection>&)> cb);
    void retrieveAllInput() { inputBuffer_.clear(); }
    void retrieveInput(size_t n) { inputBuffer_.erase(0, n); }   // 只消费前 n 字节（应用层分帧用）

private:
    static int64_t nowMs();
    void handleRead();
    void handleWrite();
    void handleClose();
    void sendInLoop(const std::string& data);   // 仅在 IO 线程执行的实际写逻辑
    void shutdownInLoop();  // 仅在 IO 线程执行的实际半关闭逻辑

    EventLoop* loop_;
    Channel* channel_;
    int fd_;
    bool closed_{false};    // 关闭幂等标志：仅在归属 IO 线程读写，无需加锁
    int64_t lastActiveMs_{0};   // 最后一次收到数据的时间，仅在归属 IO 线程读写，无需加锁
    std::string inputBuffer_;
    std::string outputBuffer_;
    std::function<void(const std::shared_ptr<TcpConnection>&)> connCb_;
    std::function<void(const std::shared_ptr<TcpConnection>&, const std::string&)> msgCb_;
    std::function<void(const std::shared_ptr<TcpConnection>&)> closeCb_;
};