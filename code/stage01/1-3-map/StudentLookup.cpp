#include "StudentLookup.h"
#include <iostream>

void StudentLookup::add(const std::string& name, double score) {
    auto ret =  records_.insert({name, score});
    if (ret.second) {
        std::cout << "[" << name << ", " << score << "] added successfully" << std::endl;
    } else {
        std::cout << "[" << name << ", " << score << "] already exists" << std::endl;
    }
}

double StudentLookup::query(const std::string& name) const {
    auto it = records_.find(name);
    if (it != records_.end()) {
        return it->second;
    }
    return -1;
}