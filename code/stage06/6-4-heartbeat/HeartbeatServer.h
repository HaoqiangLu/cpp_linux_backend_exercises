#pragma once
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

// 最小堆定时器：每个连接记录最后活跃时间
struct TimerNode {
    int fd;
    int64_t expireMs;   // 绝对过期时间（毫秒）

    bool operator>(const TimerNode& o) const {
        return expireMs > o.expireMs;
    }
};

class HeartbeatServer {
public:
    HeartbeatServer(int port, int heartbeatIntervalSec = 10, int timeoutSec = 30);
    void start();
    void gracefulShutdown();    // 练习3

private:
    void onNewConnection(int fd);
    void onReadable(int fd);
    void onClose(int fd);
    void processMessage(int fd, const std::string& msg);
    void timerLoop();   // 练习2

    int port_;
    int heartbeatIntervalSec_;
    int timeoutSec_;
    std::atomic<bool> running_{true};
    int listenFd_{-1};
    int epollFd_{-1};
    int wakeupFd_{-1};  // eventfd，供信号处理函数唤醒 epoll_wait
    std::unordered_map<int, int64_t> lastActive_;   // fd -> 最后活跃时间
    std::priority_queue<TimerNode, std::vector<TimerNode>, std::greater<TimerNode>> timerHeap_;
};

// 练习4：shutdown 半关闭测试
void halfCloseDemo();