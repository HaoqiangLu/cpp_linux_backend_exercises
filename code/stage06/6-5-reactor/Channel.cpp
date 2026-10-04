#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>

Channel::Channel(EventLoop* loop, int fd) : loop_(loop), fd_(fd) {
}

void Channel::enableReading(bool on) {
    // 设置 events_ |= EPOLLIN | EPOLLET (或清除), 然后调用 loop_->updateChannel(this)
    events_ = on ? (events_ | EPOLLIN | EPOLLET) : (events_ & ~(EPOLLIN | EPOLLET));
    loop_->updateChannel(this);
}

void Channel::enableWriting(bool on) {
    // 设置 events_ |= EPOLLOUT (或清除), 然后调用 loop_->updateChannel(this)
    events_ = on ? (events_ | EPOLLOUT) : (events_ & ~EPOLLOUT);
    loop_->updateChannel(this);
}

void Channel::handleEvent(int revents) {
    // 根据 revents 位调用对应回调
    if (revents & (EPOLLERR | EPOLLHUP)) {
        if (errorCb_) errorCb_();
    }
    if (revents & EPOLLRDHUP) {
        if (closeCb_) closeCb_();
    }
    if (revents & EPOLLIN) {
        if (readCb_) readCb_();
    }
    if (revents & EPOLLOUT) {
        if (writeCb_) writeCb_();
    }
}
