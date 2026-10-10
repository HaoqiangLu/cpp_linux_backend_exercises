#pragma once
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

class Channel;
class TimerQueue;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void updateChannel(Channel* ch);
    void removeChannel(Channel* ch);
    void runInLoop(std::function<void()> fn);
    void queueInLoop(std::function<void()> fn);
    TimerQueue* timerQueue() { return timerQueue_.get(); }
    bool isInLoopThread() const { return std::this_thread::get_id() == threadId_; }

private:
    void wakeup();
    void handlePendingTasks();

    int epollFd_;
    int wakeupFd_;  // eventfd
    std::atomic<bool> quit_{false};
    std::thread::id threadId_;  // 所属线程 ID
    std::mutex mtx_;
    std::queue<std::function<void()>> pendingTasks_;
    std::map<int, Channel*> channels_;  // fd -> Channel
    std::unique_ptr<TimerQueue> timerQueue_;
};