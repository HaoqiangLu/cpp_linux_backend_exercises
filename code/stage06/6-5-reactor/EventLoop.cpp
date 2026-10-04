#include "EventLoop.h"
#include "Channel.h"
#include <cstdlib>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <thread>
#include <unistd.h>
#include <array>
#include <utility>

EventLoop::EventLoop() : threadId_(std::this_thread::get_id()) {
    // epoll_create1(0) 创建 epoll 实例
    epollFd_ = epoll_create1(0);
    if (epollFd_ < 0) {
        std::perror("epoll_create1");
        std::exit(1);
    }
    // eventfd(0, EFD_NONBLOCK) 创建唤醒 fd
    wakeupFd_ = eventfd(0, EFD_NONBLOCK);
    if (wakeupFd_ < 0) {
        std::perror("eventfd");
        std::exit(1);
    }
    // 将 wakeupFd_ 注册到 epoll，监听 EPOLLIN
    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = wakeupFd_;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, wakeupFd_, &ev) < 0) {
        std::perror("epoll_ctl wakeup");
        std::exit(1);
    }
}

EventLoop::~EventLoop() {
    // close(epollFd_); close(wakeupFd_);
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, wakeupFd_, nullptr);
    ::close(wakeupFd_);
    ::close(epollFd_);
}

void EventLoop::loop() {
    std::array<struct epoll_event, 1024> events;
    while (!quit_) {
        int n = epoll_wait(epollFd_, events.data(), events.size(), -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            std::perror("epoll_wait");
            break;
        }
        for (auto& ev : events) {
            int fd = ev.data.fd;
            if (fd == wakeupFd_) {
                // 读取 eventfd 清除可读状态
                uint64_t val;
                ::read(fd, &val, sizeof(val));
                handlePendingTasks();
            } else {
                auto it = channels_.find(fd);
                if (it != channels_.end()) {
                    it->second->handleEvent(ev.events);
                }
            }
        }
    }
}

void EventLoop::quit() {
    quit_ = true;
    wakeup();  // 唤醒阻塞在 epoll_wait 的线程
}

void EventLoop::updateChannel(Channel* ch) {
    struct epoll_event ev{};
    ev.events = ch->events();
    ev.data.fd = ch->fd();

    if (channels_.find(ch->fd()) == channels_.end()) {
        // 新 Channel -> EPOLL_CTL_ADD
        if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, ch->fd(), &ev) < 0) {
            std::perror("epoll_ctl ADD");
        }
    } else {
        // 已存在 -> EPOLL_CTL_MOD
        if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, ch->fd(), &ev) < 0) {
            std::perror("epoll_ctl MOD");
        }
    }
    channels_[ch->fd()] = ch;
}

void EventLoop::removeChannel(Channel* ch) {
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, ch->fd(), nullptr);
    channels_.erase(ch->fd());
}

void EventLoop::wakeup() {
    uint64_t val = 1;
    ::write(wakeupFd_, &val, sizeof(val));
}

void EventLoop::runInLoop(std::function<void()> fn) {
    // 如果在 IO 线程直接执行，否则 queueInLoop
    if (std::this_thread::get_id() == threadId_) {
        /*
        因为 subLoops_ 在 EchoServer 构造函数中创建（主线程），所以 threadId_ = 主线程 ID
        就是说当前 “注册” 过程都在主线程中完成，因此只会走当前分支
        */
        fn();
    } else {
        /*
        在 start() 中，每个 subLoop 在其对应的子线程内部构造。这样 threadId_ = 子线程 ID
        这需要添加同步机制，因为 subLoop 构造移到子线程了，主线程必须等它构造完成才能继续
        否则 acceptor_->listen() 之后来连接时 subLoops_[i] 可能还是空的
        需要 std::promise / std::future 或条件变量做一次性同步。
        */
        queueInLoop(std::move(fn));
    }
}

void EventLoop::queueInLoop(std::function<void()> fn) {
    // 加锁 push 到 pendingTasks_, 然后 wakeup()
    {
        std::lock_guard lock(mtx_);
        pendingTasks_.push(std::move(fn));
    }
    wakeup();
}

void EventLoop::handlePendingTasks() {
    // 加锁 swap 出 pendingTasks_, 逐个执行
    std::queue<std::function<void()>> tasks;
    {
        std::lock_guard lock(mtx_);
        std::swap(tasks, pendingTasks_);
    }
    while (!tasks.empty()) {
        tasks.front()();
        tasks.pop();
    }
}
