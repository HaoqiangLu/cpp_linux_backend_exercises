#include "ThreadPool.h"
#include <mutex>
#include <utility>

ThreadPool::ThreadPool(int size) {
    workers_.reserve(size);
    for (int i = 0 ; i < size; ++i) {
        workers_.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx_);
                    // 等待：被唤醒且（收到停止信号或有任务）
                    cv_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
                    // 停止且队列已排空 -> 退出线程（保证已投递任务执行完）
                    if (stop_ && tasks_.empty()) return;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task(); // 锁外执行，避免长时间持锁阻塞投递
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        stop_ = true;
    }
    cv_.notify_all();   // 唤醒所有 worker 退出
    for (std::thread& t : workers_) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void ThreadPool::addTask(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (stop_) return;  // 已停止则拒绝新任务，避免投给正在推出的池
        tasks_.emplace(std::move(task));
    }
    cv_.notify_one();   // 只需唤醒一个 worker 取任务
}