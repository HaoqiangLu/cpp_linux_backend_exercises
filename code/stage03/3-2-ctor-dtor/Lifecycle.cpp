#include "Lifecycle.h"
#include <iostream>
#include <utility>

Lifecycle::Lifecycle(std::string t) : tag_(std::move(t)) {
    std::cout << "[构造] " << tag_ << std::endl;
}

Lifecycle::~Lifecycle() {
    std::cout << "[析构] " << tag_ << std::endl;
}