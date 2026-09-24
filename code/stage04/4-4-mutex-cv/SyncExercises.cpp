#include "SyncExercises.h"
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

void safeAccumulate() {
    int sum = 0;
    std::mutex mtx;
    auto add = [&]() {
        for (int i = 0; i < 100000; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            ++sum;
        }
    };
    // 创建多个 std::thread 调用 add，join 后打印 sum
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(add);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << sum << std::endl;
}

void producerConsumer() {
    std::queue<int> buffer;                 // 缓冲区
    constexpr std::size_t MAX_SIZE = 10;    // 缓冲区大小
    std::mutex mtx;                         // 互斥锁
    std::condition_variable cv;             // 条件变量
    bool done = false;                      // 生产者是否完成的标志

    // 生产者
    auto producer = [&]() {
        for (int i = 0; i < 50; ++i) {                                              // 生产 50 个元素
            std::unique_lock<std::mutex> lock(mtx);                             // 上锁
            cv.wait(lock, [&] { return buffer.size() < MAX_SIZE; });    // 等待缓冲区不满
            buffer.push(i);                                                     // 生产元素
            cv.notify_one();                                                        // 通知消费者
        }
        {
            std::lock_guard<std::mutex> lock(mtx);  // 上锁
            done = true;                                // 设置完成标志
        }
        cv.notify_all();    // 通知消费者
    };

    // 消费者
    auto consumer = [&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);                             // 上锁
            cv.wait(lock, [&] { return !buffer.empty() || done; });     // 等待缓冲区非空或生产者完成
            if (done && buffer.empty()) break;  // 如果生产者完成且缓冲区为空，则退出
            int val = buffer.front();           // 获取元素
            buffer.pop();                       // 消费元素
            cv.notify_one();                    // 通知生产者
            lock.unlock();                      // 解锁
            std::cout << val << std::endl;      // 输出元素
        }
    };

    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();
}
