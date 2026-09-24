#include "SafeQueue.h"
#include "SimpleThreadPool.h"
#include "SyncExercises.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

int main() {
    // ===== 练习1：mutex 保护多线程累加 =====
    std::cout << "===== 练习1：safeAccumulate =====" << std::endl;
    safeAccumulate();  // 10个线程各累加100000次，期望输出 1000000

    // ===== 练习2：线程安全队列 =====
    std::cout << "\n===== 练习2：SafeQueue =====" << std::endl;
    {
        SafeQueue<int> sq;
        // 多线程并发 push
        std::thread t1([&] { for (int i = 1; i <= 5; ++i) sq.push(i); });
        std::thread t2([&] { for (int i = 6; i <= 10; ++i) sq.push(i); });
        t1.join();
        t2.join();
        // 主线程 pop 所有元素
        int val;
        while (sq.pop(val)) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        std::cout << "empty: " << (sq.empty() ? "true" : "false") << std::endl;
    }

    // ===== 练习3：完整生产者消费者 =====
    std::cout << "\n===== 练习3：producerConsumer =====" << std::endl;
    producerConsumer();  // 生产者生产0~49，消费者逐个打印

    // ===== 练习4&5：简易线程池 =====
    std::cout << "\n===== 练习4&5：SimpleThreadPool =====" << std::endl;
    {
        SimpleThreadPool pool(3);  // 3个工作线程
        // 提交5个任务，每个任务打印任务编号和线程id
        for (int i = 1; i <= 5; ++i) {
            pool.submit([i] {
                std::ostringstream oss;
                oss << "Task " << i
                    << " running on thread "
                    << std::this_thread::get_id() << "\n";
                std::cout << oss.str();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            });
        }
    }  // pool 析构 → notify_all + join，等待所有任务执行完毕

    std::cout << "\nAll done." << std::endl;
    return 0;
}