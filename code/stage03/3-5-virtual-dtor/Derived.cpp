#include "Derived.h"
#include "Base.h"
#include <iostream>

Derived::Derived(int val, int extraVal)
    : Base(val), extra_(new int(extraVal)) {
    std::cout << "[Derived 构造]\n";
}

Derived::~Derived() {
    delete extra_;
    std::cout << "[Derived 析构]\n";
}