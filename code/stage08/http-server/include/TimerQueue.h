#pragma once
#include <cstdint>
#include <functional>
#include <queue>
#include <vector>

struct TimerNode {
    int64_t expireMs;
    std::function<void()> cb;
    bool operator>(const TimerNode& o) const { return expireMs > o.expireMs; }
};

class TimerQueue {
public:
    void addTimer(int64_t expireMs, std::function<void()> cb);
    void processExpiredTimers();    // 在 EventLoop 线程调用
    int64_t nextExpireMs() const;  // 返回距离下一次超时的毫秒数，-1 表示无定时器
    bool empty() const { return heap_.empty(); }

private:
    std::priority_queue<TimerNode, std::vector<TimerNode>, std::greater<TimerNode>> heap_;
};