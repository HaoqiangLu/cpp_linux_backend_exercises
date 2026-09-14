#include "AutoExercises.h"
#include <iostream>

void autoIteratorDemo(const std::vector<int>& v) {
    // 用 auto it = v.begin() 遍历打印
    // 对比：std::vector<int>::iterator it = v.begin()
    for (auto it = v.begin(); it != v.end(); it++) {
        std::cout << *it << " ";
    }
}

void autoMapDemp(const std::map<std::string, int>& m) {
    // for (const auto& [key, val] : m) 打印键值对
    for (const auto& [key, val] : m) {
        std::cout << "[" << key << "]: " << val <<std::endl;
    }
}

std::vector<int> generateData(int n) {
    std::vector<int> result;
    // 填充 1~n 并返回
    for (int i = 1; i < n; i++) {
        result.push_back(i);
    }
    return result;
}