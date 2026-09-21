#include "LogSystem.h"
#include "Logger.h"
#include <memory>
#include <utility>

void LogSystem::addLogger(std::unique_ptr<Logger> l) {
    loggers_.push_back(std::move(l));
}

void LogSystem::log(LogLevel lv, const std::string& msg) {
    for (const auto& l : loggers_) {
        if (lv < miniLevel_) continue;
        l->write(msg);
    }
}