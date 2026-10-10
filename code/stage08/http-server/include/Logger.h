#pragma once
#include <fstream>
#include <mutex>
#include <string>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static Logger& instance();
    void setLevel(LogLevel lv) { level_ = lv; }
    void setOutput(const std::string& path);
    void log(LogLevel lv, const char* file, int line, const char* fmt, ...);

private:
    Logger() = default;

    LogLevel level_{LogLevel::INFO};
    std::mutex mtx_;
    std::ofstream ofs_;
};

#define LOG_DEBUG(fmt, ...) \
    Logger::instance().log(LogLevel::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) \
    Logger::instance().log(LogLevel::INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) \
    Logger::instance().log(LogLevel::WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) \
    Logger::instance().log(LogLevel::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
