#include "ConsoleLogger.h"
#include <iostream>


void ConsoleLogger::write(const std::string& msg) const {
    std::cout << msg << std::endl;
}