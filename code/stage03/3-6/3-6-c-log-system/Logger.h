#pragma once
#include <string>

class Logger {
public:
    virtual ~Logger() = default;
    virtual void write(const std::string& msg) const = 0;
};