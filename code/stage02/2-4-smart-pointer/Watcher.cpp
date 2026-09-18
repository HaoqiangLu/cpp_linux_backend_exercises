#include "Watcher.h"
#include <iostream>
#include <utility>

Watcher::Watcher(std::string n) : name(std::move(n)) {
    std::cout << name << " created\n";
}

Watcher::~Watcher() {
    std::cout << name << " destroyed\n";
}