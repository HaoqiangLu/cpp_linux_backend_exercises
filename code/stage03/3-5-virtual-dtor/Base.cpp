#include "Base.h"
#include <iostream>

Base::Base(int val) : data_(new int(val)) {
    std::cout << "[Base 构造]\n";
}

Base::~Base() {
    delete data_;
    std::cout << "[Base 析构]\n";
}