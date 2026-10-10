#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>

Channel::Channel(EventLoop* loop, int fd) : loop_(loop), fd_(fd) {
}

void Channel::enableReading(bool on) {
    if (on) {
        events_ |= (EPOLLIN | EPOLLRDHUP | EPOLLET);
    } else {
        events_ &= ~(EPOLLIN | EPOLLRDHUP);
    }
    loop_->updateChannel(this);
}

void Channel::enableWriting(bool on) {
    if (on) {
        events_ |= EPOLLOUT;
    } else {
        events_ &= ~EPOLLOUT;
    }
    loop_->updateChannel(this);
}

void Channel::handleEvent(int revents) {
    if (revents & EPOLLERR) {
        if (errorCb_) errorCb_();
    }
    // 读事件优先，且把 EPOLLRDHUP/EPOLLHUP 也一并交给 readCb_ 处理：
    // handleRead 会循环排空内核接收缓冲（把对端半关闭前发来的残余请求读走并交付），
    // 再在 read()==0(EOF) 时关闭连接。
    // 若像旧实现那样在 RDHUP 时不读直接 close，内核接收缓冲里未读的请求数据
    // 会让 close() 发出 RST 而非 FIN —— 这正是 ab 高并发下 "Connection reset by peer"
    // 导致压测中止、QPS=N/A 的根因。
    // 注意：readCb_ 可能触发 handleClose 并析构本 Channel，调用后必须立即 return，
    // 不得再访问 this（否则 use-after-free）。
    if (revents & (EPOLLIN | EPOLLRDHUP | EPOLLHUP)) {
        if (readCb_) readCb_();
        return;
    }
    if (revents & EPOLLOUT) {
        if (writeCb_) writeCb_();
    }
}