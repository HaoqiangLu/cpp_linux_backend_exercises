#pragma once
#include <string>

struct Task {
    int id = 0;
    std::string title;
    int priority = 1;   // 1: low, 2: medium, 3: high
    bool done = false;
};