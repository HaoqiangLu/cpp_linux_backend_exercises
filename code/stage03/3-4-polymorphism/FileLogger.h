#pragma once
#include "Logger.h"
#include <string>

class FileLogger : public Logger {
public:
    explicit FileLogger(std::string f);
    void log(const std::string& msg) const override;

private:
    std::string filename_;
};