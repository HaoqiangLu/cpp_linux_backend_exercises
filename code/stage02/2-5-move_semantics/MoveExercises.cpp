#include "MoveExercises.h"
#include <chrono>
#include <iostream>
#include <utility>
#include <vector>

void moveDemo() {
    std::vector<int> v = { 1, 2, 3};
    std::cout << "v 移动前: ";
    for (int i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    auto v2 = std::move(v);
    std::cout << "移动后\nv2: ";
    for (int i : v2) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "v 大小: " << v.size() << std::endl;
}

void benchmark() {
    std::vector<int> src(1000000, 1);
    // 使用 std::chrono 计时拷贝 vs 移动
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> v1 = src;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "拷贝: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " 微秒" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::vector<int> v2 = std::move(src);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "移动: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " 微秒" << std::endl;
}