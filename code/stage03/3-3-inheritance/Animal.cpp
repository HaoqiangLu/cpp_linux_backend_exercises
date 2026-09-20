#include "Animal.h"
#include <iostream>
#include <utility>

Animal::Animal(std::string n, int a) : name_(std::move(n)), age_(a) {
    std::cout << "[构造] Animal 基类: " << name_ << std::endl;
}

Animal::~Animal() {
    std::cout << "[析构] Animal 基类: " << name_ << std::endl;
}

void Animal::printInfo() const {
    std::cout << "Name: " << name_ << ", Age: " << age_ << std::endl;
}