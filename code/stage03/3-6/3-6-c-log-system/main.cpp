#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "LogSystem.h"
#include <memory>

int main() {
    LogSystem system;

    // 注册多种日志输出
    system.addLogger(std::make_unique<ConsoleLogger>());
    system.addLogger(std::make_unique<FileLogger>("app.log"));

    // 设置最低日志级别为 INFO
    system.setLevel(LogLevel::WARN);

    // 发送不同级别的日志
    system.log(LogLevel::INFO, "程序启动");
    system.log(LogLevel::WARN, "内存使用率超过 80%");
    system.log(LogLevel::ERROR, "数据库连接失败");

    return 0;
}