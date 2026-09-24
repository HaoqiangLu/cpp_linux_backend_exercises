#include "SimpleThreadPool.h"
#include <mutex>
#include <utility>

SimpleThreadPool::SimpleThreadPool(int numThreads) {
    // 创建 numThreads 个工作线程
    // 工作线程循环：std::unique_lock + cv_.wait + 取任务 + 执行
    for (int i = 0; i < numThreads; ++i) {
        workers_.emplace_back([this] () {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx_);
                    cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) return;
                    task = tasks_.front();
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

SimpleThreadPool::~SimpleThreadPool() {
    // stop_ = true; cv_.notify_all(); 并 join 所有线程
    stop_ = true;
    cv_.notify_all();
    for (auto& worker : workers_) {
        worker.join();
    }
}

void SimpleThreadPool::submit(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        tasks_.push(task);
    }
    cv_.notify_one();
}