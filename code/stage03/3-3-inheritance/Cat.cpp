#include "Cat.h"
#include <iostream>
#include <utility>

Cat::Cat(std::string n, int a, bool indoor)
    : Animal(std::move(n), a), isIndoor_(indoor) {
    std::cout << "[构造] Cat 派生类: " << (isIndoor_ ? "室内" : "室外") << std::endl;
}

Cat::~Cat() {
    std::cout << "[析构] Cat 派生类" << std::endl;
}

void Cat::printInfo() const {
    Animal::printInfo();
    std::cout << (isIndoor_ ? "室内" : "室外") << std::endl;
}

void Cat::meow() const {
    std::cout << "喵喵" << std::endl;
}