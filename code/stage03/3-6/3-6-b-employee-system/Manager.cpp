#include "Manager.h"
#include "Employee.h"
#include <iostream>

Manager::Manager(std::string n, double base)
    : Employee(std::move(n)), baseSalary_(base) {

}

double Manager::calcSalary() const {
    return baseSalary_ * 1.2;
}

void Manager::printInfo() const {
    std::cout << "Manager: " << name_ << ", Salary = " << calcSalary() << std::endl;
}