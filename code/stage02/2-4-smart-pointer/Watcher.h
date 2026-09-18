#pragma once
#include <string>

// 练习5：析构打印验证释放时机
struct Watcher {
    explicit Watcher(std::string n);
    ~Watcher();

    Watcher(const Watcher&) = delete;
    Watcher& operator=(const Watcher&) = delete;

    std::string name;
};