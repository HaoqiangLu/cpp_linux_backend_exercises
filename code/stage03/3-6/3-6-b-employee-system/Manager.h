#pragma once
#include "Employee.h"
#include <string>

class Manager : public Employee {
public:
    Manager(std::string n, double base);
    double calcSalary() const override;
    void printInfo() const override;

private:
    double baseSalary_ = 0.0;
};