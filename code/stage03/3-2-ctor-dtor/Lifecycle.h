#pragma once
#include <string>

// 练习1：构造/析构打印日志
class Lifecycle {
public:
    explicit Lifecycle(std::string t);
    ~Lifecycle();

private:
    std::string tag_;
};