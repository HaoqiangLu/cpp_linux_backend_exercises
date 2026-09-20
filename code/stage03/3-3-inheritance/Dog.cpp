#include "Dog.h"
#include <iostream>
#include <utility>

Dog::Dog(std::string n, int a, std::string b)
    : Animal(std::move(n), a), breed_(std::move(b)) {
    std::cout << "[构造] Dog 派生类: " << breed_ << std::endl;
}

Dog::~Dog() {
    std::cout << "[析构] Dog 派生类: " << breed_ << std::endl;
}

void Dog::printInfo() const {
    // 先调用 Animal::printInfo()，再打印 breed_
    Animal::printInfo();
    std::cout << "Breed: " << breed_ << std::endl;
}

void Dog::bark() const {
    std::cout << "汪汪" << std::endl;
}