#pragma once
#include "Employee.h"
#include <string>

class Developer : public Employee {
public:
    Developer(std::string n, int h, double rate);
    double calcSalary() const override;

private:
    int hours_ = 0;
    double hourlyRate_ = 0.0;
};