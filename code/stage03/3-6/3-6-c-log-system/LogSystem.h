#pragma once
#include "LogLevel.h"
#include "Logger.h"
#include <memory>
#include <string>
#include <vector>

class LogSystem {
public:
    void addLogger(std::unique_ptr<Logger> l);
    void setLevel(LogLevel lv) noexcept { miniLevel_ = lv; }
    void log(LogLevel lv, const std::string& msg);

private:
    std::vector<std::unique_ptr<Logger>> loggers_;
    LogLevel miniLevel_ = LogLevel::INFO;
};