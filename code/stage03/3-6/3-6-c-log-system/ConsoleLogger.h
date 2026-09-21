#pragma once
#include "Logger.h"

class ConsoleLogger : public Logger {
public:
    void write(const std::string& msg) const override;
};