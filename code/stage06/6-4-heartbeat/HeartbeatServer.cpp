#include "HeartbeatServer.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <print>
#include <array>

static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

HeartbeatServer::HeartbeatServer(int port, int hb, int to)
    : port_(port), heartbeatIntervalSec_(hb), timeoutSec_(to) {
}

void HeartbeatServer::start() {
    // socket + setsockopt(SO_REUSEADDR) + bind + listen
    listenFd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd_ < 0) {
        std::perror("socket");
        return;
    }

    int opt = 1;
    ::setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = ::bind(listenFd_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    if (ret < 0) {
        std::perror("bind");
        ::close(listenFd_);
        return;
    }

    ret = ::listen(listenFd_, SOMAXCONN);
    if (ret < 0) {
        std::perror("listen");
        ::close(listenFd_);
        return;
    }

    // 设置 listenFd 为非阻塞，否则 accept 循环会在没有更多连接时阻塞
    int flags = ::fcntl(listenFd_, F_GETFL, 0);
    if (flags < 0) {
        std::perror("fcntl F_GETFL");
        ::close(listenFd_);
        return;
    }
    if (::fcntl(listenFd_, F_SETFL, flags | O_NONBLOCK) < 0) {
        std::perror("fcntl F_SETFL");
        ::close(listenFd_);
        return;
    }

    // epoll_create1 + 把 listenFd 加入监听
    epollFd_ = ::epoll_create1(0);
    if (epollFd_ < 0) {
        std::perror("epoll_create1");
        ::close(listenFd_);
        return;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listenFd_;

    ret = ::epoll_ctl(epollFd_, EPOLL_CTL_ADD, listenFd_, &ev);
    if (ret < 0) {
        std::perror("epoll_ctl");
        ::close(listenFd_);
        ::close(epollFd_);
        return;
    }

    // eventfd 用于信号唤醒（比 self-pipe 更简单可靠）
    wakeupFd_ = ::eventfd(0, EFD_NONBLOCK);
    if (wakeupFd_ < 0) {
        std::perror("eventfd");
        ::close(listenFd_);
        ::close(epollFd_);
        return;
    }

    ev.events = EPOLLIN;
    ev.data.fd = wakeupFd_;
    ret = ::epoll_ctl(epollFd_, EPOLL_CTL_ADD, wakeupFd_, &ev);
    if (ret < 0) {
        std::perror("epoll_ctl wakeup");
        ::close(wakeupFd_);
        ::close(listenFd_);
        ::close(epollFd_);
        return;
    }

    std::println("HeartbeatServer listening on port {} (timeout={}s)", port_, timeoutSec_);

    // 事件循环
    std::array<struct epoll_event, 64> events{};
    while (running_) {
        // 计算 epoll_wait 超时 = 堆顶定时器的剩余时间
        int timeout = -1;   // 默认无阻塞
        if (!timerHeap_.empty()) {
            timeout = static_cast<int>(timerHeap_.top().expireMs - nowMs());
            if (timeout < 0) timeout = 0;
        }

        // epoll_wait
        int n = ::epoll_wait(epollFd_, events.data(), static_cast<int>(events.size()), timeout);
        if (n < 0) {
            if (errno == EINTR) continue;   // 被信号中断，重新等待
            std::perror("epoll_wait");
            break;
        }

        // 处理事件
        for (int i = 0; i < n; ++i) {
            int fd = events[i].data.fd;
            if (fd == listenFd_) {
                // LT 模式下 循环 accept 所有待处理连接
                while (true) {
                    struct sockaddr_in clientAddr{};
                    socklen_t len = sizeof(clientAddr);
                    int clientFd = ::accept(listenFd_, reinterpret_cast<struct sockaddr*>(&clientAddr), &len);
                    if (clientFd < 0) break;    // EAGAIN / 无更多连接
                    onNewConnection(clientFd);
                }
            } else if (fd == wakeupFd_) {
                // eventfd 唤醒：读取并丢弃计数器值
                uint64_t val;
                ::read(wakeupFd_, &val, sizeof(val));
            } else if (events[i].events & EPOLLIN) {
                // 优先处理可读事件：即使同时有 EPOLLHUP/EPOLLERR，也要先读完缓冲区数据
                onReadable(fd);
            } else if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                onClose(fd);
            }
        }

        // 扫描超时连接
        timerLoop();
    }

    std::println("[server] shutting down...");

    // 清理所有残留连接
    for (auto& [fd, _] : lastActive_) {
        ::close(fd);
    }
    lastActive_.clear();
    while (!timerHeap_.empty()) {
        timerHeap_.pop();
    }

    ::close(wakeupFd_);
    ::close(epollFd_);
    ::close(listenFd_);
    wakeupFd_ = -1;
    epollFd_ = -1;
    listenFd_ = -1;
}

void HeartbeatServer::onNewConnection(int fd) {
    // 加入 epoll 监听可读事件
    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev);
    if (ret < 0) {
        std::perror("epoll_ctl add client");
        ::close(fd);
        return;
    }

    // 记录最后活跃时间
    int64_t now = nowMs();
    lastActive_[fd] = now;

    // 加入定时器堆，过期时间 = 当前 + timeoutSec 秒
    timerHeap_.push({fd, now + static_cast<int64_t>(timeoutSec_) * 1000});

    std::println("[server] new connection fd={}", fd);
}

void HeartbeatServer::onReadable(int fd) {
    std::array<char, 1024> buf{};
    ssize_t n = ::read(fd, buf.data(), buf.size());
    if (n <= 0) {
        // n == 0: 对端正常关闭(FIN)；n<0: 读取出错
        onClose(fd);
        return;
    }

    // 构造消息并处理
    std::string msg(buf.data(), n);
    processMessage(fd, msg);

    // 心跳续期：更新活跃时间 + 重新入堆
    int64_t now = nowMs();
    lastActive_[fd] = now;
    timerHeap_.push({fd, now + static_cast<int64_t>(timeoutSec_) * 1000});
}

void HeartbeatServer::onClose(int fd) {
    ::epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr);
    ::close(fd);
    lastActive_.erase(fd);
    std::println("[server] closed fd={}", fd);
}

void HeartbeatServer::processMessage(int fd, const std::string& msg) {
    std::println("[server] fd={} recv: {}", fd, msg);

    // echo 回包
    std::string reply = "ECHO: " + msg;
    ::send(fd, reply.data(), reply.size(), 0);
}

void HeartbeatServer::gracefulShutdown() {
    // 注意：此函数从信号处理函数调用，只能使用 async-signal-safe 操作
    running_ = false;

    // 往 eventfd 写一个值，唤醒 server 线程的 epoll_wait
    if (wakeupFd_ >= 0) {
        uint64_t val = 1;
        ::write(wakeupFd_, &val, sizeof(val));
    }
}

void HeartbeatServer::timerLoop() {
    int64_t now = nowMs();

    while (!timerHeap_.empty() && timerHeap_.top().expireMs <= now) {
        TimerNode node = timerHeap_.top();
        timerHeap_.pop();

        // 惰性检查：fd 已关闭则跳过
        auto it = lastActive_.find(node.fd);
        if (it == lastActive_.end()) continue;

        // 惰性检查：fd 已续期（实际过期时间 > 当前时间）则跳过
        if (it->second + static_cast<int64_t>(timeoutSec_) * 1000 > now) continue;

        // 确认超时，关闭连接
        std::println("[server] fd={} heartbeat timeout, closing", node.fd);
        onClose(node.fd);
    }
}

// 练习4：半关闭测试
void halfCloseDemo() {
    int sv[2];
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {
        std::perror("socketpair");
        return;
    }

    // 端 0 发送数据，端 1 关闭写端
    const char* msg = "hello form sv[0]";
    ::send(sv[0], msg, std::strlen(msg), 0);

    // 端 1 关闭写端，发送FIN
    ::shutdown(sv[1], SHUT_WR);
    std::println("[halfClose] sv[1] shutdown(SHUT_WR)");

    // 端 0 读取直到 EOF
    char buf[64];
    ssize_t n;
    while ((n = ::read(sv[0], buf, sizeof(buf))) > 0) {
        std::println("[halfClose] sv[0] read: {}", std::string(buf, n));
    }
    std::println("[halfClose] sv[0] read returned {} (EOF)", n);

    // 验证：端 0 仍然可以写，端 1 仍能读
    const char* reply = "still writing after EOF";
    ::send(sv[0], reply, std::strlen(reply), 0);

    n = ::read(sv[1], buf, sizeof(buf));
    if (n > 0) {
        std::println("[halfClose] sv[1] read after shutdown: {}", std::string(buf, n));
    }

    ::close(sv[0]);
    ::close(sv[1]);
}