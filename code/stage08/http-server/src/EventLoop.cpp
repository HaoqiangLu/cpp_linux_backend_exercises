#include "EventLoop.h"
#include "Channel.h"
#include "TimerQueue.h"
#include <array>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <utility>

EventLoop::EventLoop() : threadId_(std::this_thread::get_id()) {
    epollFd_ = epoll_create1(EPOLL_CLOEXEC);
    if (epollFd_ < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    wakeupFd_ = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (wakeupFd_ < 0) {
        perror("eventfd");
        close(epollFd_);
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = wakeupFd_;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, wakeupFd_, &ev) < 0) {
        perror("epoll_ctl add wakeupFd");
        close(wakeupFd_);
        close(epollFd_);
        exit(EXIT_FAILURE);
    }

    timerQueue_ = std::make_unique<TimerQueue>();
}

EventLoop::~EventLoop() {
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, wakeupFd_, nullptr);
    close(wakeupFd_);
    close(epollFd_);
}

void EventLoop::loop() {
    std::array<struct epoll_event, 1024> events{};
    while (!quit_) {
        int timeout = static_cast<int>(timerQueue_->nextExpireMs());
        int n = epoll_wait(epollFd_, events.data(), events.size(), timeout);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (auto& ev : events) {
            int fd = ev.data.fd;
            if (fd == wakeupFd_) {
                uint64_t val = 0;
                read(fd, &val, sizeof(val));
            } else {
                auto it = channels_.find(fd);
                if (it != channels_.end()) {
                    it->second->handleEvent(ev.events);
                }
            }
        }

        timerQueue_->processExpiredTimers();
        handlePendingTasks();
    }
}

void EventLoop::quit() {
    quit_ = true;
    if (!isInLoopThread()) {
        wakeup();
    }
}

void EventLoop::updateChannel(Channel* ch) {
    struct epoll_event ev{};
    ev.events = ch->events();
    ev.data.fd = ch->fd();

    int fd = ch->fd();
    if (channels_.find(fd) == channels_.end()) {
        if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev) < 0) {
            perror("epoll_ctl ADD");
        }
    } else {
        if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev) < 0) {
            perror("epoll_ctl MOD");
        }
    }
    channels_[fd] = ch;
}

void EventLoop::removeChannel(Channel* ch) {
    int fd = ch->fd();
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr);
    channels_.erase(fd);
}

void EventLoop::runInLoop(std::function<void()> fn) {
    if (isInLoopThread()) {
        fn();
    } else {
        queueInLoop(std::move(fn));
    }
}

void EventLoop::queueInLoop(std::function<void()> fn) {
    {
        std::lock_guard<std::mutex> lock{mtx_};
        pendingTasks_.push(std::move(fn));
    }
    wakeup();
}

void EventLoop::wakeup() {
    uint64_t val = 1;
    ssize_t r = write(wakeupFd_, &val, sizeof(val));
    if (r != sizeof(val)) {
        perror("write wakeupFd");
    }
}

void EventLoop::handlePendingTasks() {
    std::queue<std::function<void()>> tasks;
    {
        std::lock_guard<std::mutex> lock{mtx_};
        std::swap(tasks, pendingTasks_);
    }
    while (!tasks.empty()) {
        tasks.front()();
        tasks.pop();
    }
}