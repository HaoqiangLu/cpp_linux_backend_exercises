#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>
#include <array>

int64_t TcpConnection::nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

TcpConnection::TcpConnection(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd), lastActiveMs_(nowMs()) {
    channel_ = new Channel(loop, fd);
    channel_->setReadCallback([this]() { handleRead(); });
    channel_->setWriteCallback([this]() { handleWrite(); });
    channel_->setCloseCallback([this]() { handleClose(); });
    channel_->enableReading();
}

TcpConnection::~TcpConnection() {
    loop_->removeChannel(channel_);
    delete channel_;
    close(fd_);
}

void TcpConnection::handleRead() {
    std::array<char, 65536> buf;
    bool eof = false;   // 对端关闭写侧(read 返回 0)：可能仍有已读数据待交付
    // ET 模式：必须循环读，直到把内核缓冲区读空(EAGAIN)
    while (true) {
        ssize_t n = ::read(fd_, buf.data(), buf.size());
        if (n > 0) {
            inputBuffer_.append(buf.data(), n);
            touch();   // 收到数据即视为活跃，刷新空闲超时计时
        } else if (n == 0) {
            // 对端正常关闭：不立即 return，先记下 EOF，将已读数据交付后再关闭。
            // 否则“请求与 FIN 同批到达”时请求会被丢弃（旧实现的丢请求/RST 隐患）。
            eof = true;
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;  // 内核缓冲区读空，退出
            if (errno == EINTR) continue;                        // 被信号中断，重试
            // 对端断开（ECONNRESET/EPIPE 等）属高并发常态，静默关闭避免 perror 刷屏
            if (errno != ECONNRESET && errno != EPIPE && errno != ENOTCONN && errno != ETIMEDOUT) {
                perror("read");
            }
            handleClose();
            return;
        }
    }

    // 先交付已读数据（可能是完整请求，也可能半包——由上层分帧决定）。
    // 必须在 handleClose 之前：onRequest 会把任务（持 conn 引用）投递线程池，
    // 保证连接在响应写出前不被析构。
    if (!inputBuffer_.empty() && msgCb_) {
        msgCb_(shared_from_this(), inputBuffer_);
    }

    // 对端已关闭写侧：交付并触发响应后再关闭连接。
    if (eof) {
        handleClose();
    }
}

void TcpConnection::handleWrite() {
    // ET 模式：EPOLLOUT 是边缘触发，必须循环写直到 EAGAIN，否则剩余数据不会再触发写事件而卡住
    while (!outputBuffer_.empty()) {
        ssize_t n = ::write(fd_, outputBuffer_.data(), outputBuffer_.size());
        if (n > 0) {
            outputBuffer_.erase(0, n);
        } else if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;  // 内核发送缓冲区满，等下一次 EPOLLOUT
            if (errno == EINTR) continue;                        // 被信号中断，重试
            // 对端已断开导致写失败属常态，静默关闭避免 perror 刷屏
            if (errno != EPIPE && errno != ECONNRESET && errno != ENOTCONN) {
                perror("write");
            }
            handleClose();
            return;
        }
    }
    if (outputBuffer_.empty()) {
        channel_->enableWriting(false); // 缓冲区排空，取消 EPOLLOUT，避免空转
    }
}

void TcpConnection::handleClose() {
    // 幂等保护：handleRead(read==0) / handleWrite(写错误) / Channel closeCallback
    // 可能在同一轮事件里对同一连接多次触发关闭，若重复回调 onClose 会按 fd
    // 误删后来复用同一 fd 的新连接。用 closed_ 保证只关闭一次。
    if (closed_) return;
    closed_ = true;
    auto self = shared_from_this(); // 保活到函数结束，防止 closeCb_ 释放最后引用后 this 被析构
    if (closeCb_) {
        closeCb_(self);
    }
    loop_->removeChannel(channel_);
}

void TcpConnection::send(const std::string& data) {
    // 线程安全入口：把实际写操作 marshal 回连接所属的 IO 线程执行
    if (loop_->isInLoopThread()) {
        sendInLoop(data);
    } else {
        // 跨线程：捕获 shared_from_this 保活，避免任务执行前连接被析构
        loop_->runInLoop([self = shared_from_this(), data]() {
            self->sendInLoop(data);
        });
    }
}

void TcpConnection::sendInLoop(const std::string& data) {
    // 只在 IO 线程执行，独占访问 outputBuffer_ / channel_ / fd_，无数据竞争
    if (outputBuffer_.empty()) {
        // 缓冲区为空，尝试直接写入内核，省去一次拷贝
        ssize_t n = ::write(fd_, data.data(), data.size());
        if (n >= 0) {
            if (static_cast<size_t>(n) < data.size()) {
                // 内核缓冲区满，只写出去一部分，剩余入队并注册 EPOLLOUT
                outputBuffer_.append(data.data() + n, data.size() - n);
                channel_->enableWriting(true);
            }
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                // 暂时写不进去，全部入队等待写就绪
                outputBuffer_.append(data);
                channel_->enableWriting(true);
            } else {
                // 对端已断开属常态，静默关闭避免 perror 刷屏
                if (errno != EPIPE && errno != ECONNRESET && errno != ENOTCONN) {
                    perror("write");
                }
                handleClose();
            }
        }
    } else {
        // 已有待发数据，直接追加，保证发送顺序
        outputBuffer_.append(data);
    }
}

void TcpConnection::shutdown() {
    // 同样可能从工作线程调用（HttpHandler 里 !keepAlive 时），需 marshal 回 IO 线程
    if (loop_->isInLoopThread()) {
        shutdownInLoop();
    } else {
        loop_->runInLoop([self = shared_from_this()]() {
            self->shutdownInLoop();
        });
    }
}

void TcpConnection::shutdownInLoop() {
    if (outputBuffer_.empty()) {
        ::shutdown(fd_, SHUT_WR);   // 半关闭：不再发送，仍可接受
    }
}

void TcpConnection::setConnectionCallback(
    std::function<void(const std::shared_ptr<TcpConnection>&)> cb
) {
    connCb_ = std::move(cb);
}

void TcpConnection::setMessageCallback(
    std::function<void(const std::shared_ptr<TcpConnection>&, const std::string&)> cb
) {
    msgCb_ = std::move(cb);
}

void TcpConnection::setCloseCallback(
    std::function<void(const std::shared_ptr<TcpConnection>&)> cb
) {
    closeCb_ = std::move(cb);
}
