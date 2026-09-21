#include "Employee.h"
#include <iostream>
#include <utility>

Employee::Employee(std::string n) : name_(std::move(n)) {

}

void Employee::printInfo() const {
    std::cout << "Employee: " << name_ << ", " << calcSalary() << std::endl;
}