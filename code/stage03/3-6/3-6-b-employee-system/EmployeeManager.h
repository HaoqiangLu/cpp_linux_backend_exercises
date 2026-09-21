#pragma once
#include "Employee.h"
#include <memory>
#include <vector>

class EmployeeManager {
public:
    void add(std::unique_ptr<Employee> e);
    void printAll() const;
    void sortBySalary();

private:
    std::vector<std::unique_ptr<Employee>> employees_;
};