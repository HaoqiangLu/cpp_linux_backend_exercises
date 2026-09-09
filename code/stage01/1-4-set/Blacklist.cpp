#include "Blacklist.h"
#include <iostream>

void Blacklist::add(int id) {
    auto it = ids_.insert(id);
    if (it.second) {
        std::cout << id << " added to blacklist." << std::endl;
    } else {
        std::cout << id << " already exists in blacklist." << std::endl;
    }
}

void Blacklist::remove(int id) {
    auto it = ids_.find(id);
    if (it != ids_.end()) {
        ids_.erase(it);
        std::cout << id << " removed from blacklist." << std::endl;
    } else {
        std::cout << id << " not found in blacklist." << std::endl;
    }
}

bool Blacklist::contains(int id) const {
    if (ids_.find(id) != ids_.end()) {
        return true;
    }
    return false;
}