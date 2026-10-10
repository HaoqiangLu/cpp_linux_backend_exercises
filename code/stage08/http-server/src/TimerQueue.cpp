#include "TimerQueue.h"
#include <chrono>
#include <utility>

static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void TimerQueue::addTimer(int64_t expireMs, std::function<void()> cb) {
    heap_.push({expireMs, std::move(cb)});
}

void TimerQueue::processExpiredTimers() {
    int64_t now = nowMs();
    // 小根堆堆顶是最早到期的定时器，循环弹出所有已到期顶
    while (!heap_.empty() && heap_.top().expireMs <= now) {
        // top() 返回 const 引用，需 const_cast 才能 move 出回调
        auto cb = std::move(const_cast<TimerNode&>(heap_.top()).cb);
        heap_.pop();    // 先弹出再执行，避免回调里再动堆导致状态错乱
        if (cb) cb();
    }
}

int64_t TimerQueue::nextExpireMs() const {
    if (heap_.empty()) {
        return -1;  // 无定时器：epoll_wait 无限等待
    }
    int64_t remain = heap_.top().expireMs - nowMs();
    return remain > 0 ? remain : 0; // 已到期返回0，让 epoll_wait 立即返回
}