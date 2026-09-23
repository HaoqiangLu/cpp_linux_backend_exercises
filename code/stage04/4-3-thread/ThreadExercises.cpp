#include "ThreadExercises.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

void printNumbers(int id, int start, int end) {
    // 循环打印 id 和 数字
    for (int i = start; i <= end; ++i) {
        std::cout << "[id " << id << "]" << ": " << i << std::endl;
    }
}

void multiPrintDemo() {
    // --- join 演示：主线程等待子线程结束才继续 ---
    std::cout << "[join] Main: thread starting..." << std::endl;
    std::thread t1([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "[join] Thread: done!" << std::endl;
    });
    t1.join(); // 阻塞，等 t1 执行完毕
    std::cout << "[join] Main: continues after join (thread already done)" << std::endl;

    std::cout << std::endl;

    // --- detach 演示：主线程不等待，子线程后台独立运行 ---
    std::cout << "[detach] Main: thread starting..." << std::endl;
    std::thread t2([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "[detach] Thread: done!" << std::endl;
    });
    t2.detach(); // 分离，t2 在后台独立运行
    std::cout << "[detach] Main: continues immediately (thread still running)" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 等待 detached 线程完成，防止 main 退出时线程仍在运行
}

void raceDemo() {
    int counter = 0;
    auto increment = [&counter] () {
        for (int i = 0; i < 100000; ++i) {
            ++counter;  // 观察无保护时的数据竞争
        }
    };
    // 创建多个线程调用 increment，观察最终 counter 值
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(increment);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Final counter value: " << counter << std::endl;
}

void downloadSimulator(const std::string& filename, int numChunks, int numThreads) {
    // 每个线程负责下载一部分 chunk
    // 打印每个线程的进度
    std::vector<std::thread> threads;
    int chunksPerThread = numChunks / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunksPerThread;
        int end = (i == numThreads - 1) ? numChunks : (i + 1) * chunksPerThread;

        threads.emplace_back([filename, start, end, i] () {
            for (int chunk = start; chunk < end; ++chunk) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "[Thread " << i << "] download " << filename << " chunk " << chunk + 1 << "/" << end - start << std::endl;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << filename << " downloaded successfully." << std::endl;
}