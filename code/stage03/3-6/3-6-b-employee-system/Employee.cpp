#include "Employee.h"
#include <iostream>
#include <utility>

Employee::Employee(const std::string n) : name_(n) {

}

void Employee::printInfo() const {
    std::cout << "Employee: " << name_ << ", Salary: " << calcSalary() << std::endl;
}
