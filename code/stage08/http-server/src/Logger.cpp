#include "Logger.h"
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

namespace {
const char* levelName(LogLevel lv) {
    switch (lv) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}
}   // namespace

Logger& Logger::instance() {
    static Logger inst; // C++11 起局部静态初始化线程安全(magic static)
    return inst;
}

void Logger::setOutput(const std::string& path) {
    std::lock_guard<std::mutex> lk(mtx_);
    if (ofs_.is_open()) {
        ofs_.close();
    }
    ofs_.open(path, std::ios::app); // 追加模式，保留历史日志
}

void Logger::log(LogLevel lv, const char* file, int line, const char* fmt, ...) {
    if (lv < level_) return;    // 低于阈值的日志直接丢弃

    // 1 时间戳（精确到毫秒），localtime_r 线程安全
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_r(&t, &tm);
    char timeBuf[64];
    std::snprintf(timeBuf, sizeof(timeBuf), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec, static_cast<int>(ms));

    // 2 线程 ID
    std::ostringstream tidss;
    tidss << std::this_thread::get_id();

    // 3 变参格式化：两次 vsnprintf，先测长度再分配，避免定长缓冲区阶段
    va_list args;
    va_start(args, fmt);
    va_list argsCopy;
    va_copy(argsCopy, args);
    int len = std::vsnprintf(nullptr, 0, fmt, argsCopy);
    va_end(argsCopy);
    std::vector<char> msgBuf(static_cast<size_t>(len) + 1);
    std::vsnprintf(msgBuf.data(), msgBuf.size(), fmt, args);
    va_end(args);
    std::string message(msgBuf.data(), len);

    // 4 加锁输出到文件或 stdout（保证多线程日志不交错）
    std::lock_guard<std::mutex> lk(mtx_);
    std::ostream& os = ofs_.is_open() ? static_cast<std::ostream&>(ofs_) : std::cout;
    os << '[' << timeBuf << "] [" << levelName(lv) << "] [tid:" << tidss.str()
       << "] [" << file << ':' << line << "] " << message << '\n';
    os.flush();
}