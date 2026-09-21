#include "Programmer.h"
#include "Employee.h"
#include <iostream>

Programmer::Programmer(std::string n, int h, double r)
    : Employee(std::move(n)), hours_(h), rate_(r) {
}

double Programmer::calcSalary() const {
    return hours_ * rate_;
}

void Programmer::printInfo() const {
    std::cout << "Programmer: " << name_ << ", Salary = " << calcSalary() << std::endl;
}