#pragma once
#include "Employee.h"
#include <string>

class Programmer : public Employee {
public:
    Programmer(std::string n, int h, double r);
    double calcSalary() const override;
    void printInfo() const override;

private:
    int hours_ = 0;
    double rate_ = 0.0;
};