#pragma once
#include <string>

struct Resource {
    std::string name;
    std::string type;   // file / network / buffer
    int priority = 0;
};