#pragma once
#include <string>

// 练习4：日志系统
class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const std::string& msg) const = 0;
};