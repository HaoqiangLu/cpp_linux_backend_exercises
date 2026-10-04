#pragma once
#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void updateChannel(Channel* ch);
    void removeChannel(Channel* ch);
    // 练习2：跨线程投递任务
    void runInLoop(std::function<void()> fn);
    void queueInLoop(std::function<void()> fn);

private:
    void wakeup();  // 练习2：eventfd 唤醒
    void handlePendingTasks();

    int epollFd_;
    int wakeupFd_;  // eventfd
    std::atomic<bool> quit_{false};
    std::thread::id threadId_;  // 所属线程 ID
    std::mutex mtx_;
    std::queue<std::function<void()>> pendingTasks_;
    std::map<int, Channel*> channels_;  // fd -> Channel
};
