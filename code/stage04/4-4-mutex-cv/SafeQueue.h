#pragma once
#include <mutex>
#include <queue>

// 练习2:线程安全队列
template <typename T>
class SafeQueue {
public:
    void push(const T& val) {
        std::lock_guard<std::mutex> lock(mtx_);
        q_.push(val);
    }

    bool pop(T& val) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (q_.empty()) return false;
        val = q_.front();
        q_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return q_.empty();
    }

private:
    std::queue<T> q_;
    mutable std::mutex mtx_;
};