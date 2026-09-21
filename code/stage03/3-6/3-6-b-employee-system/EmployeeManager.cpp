#include "EmployeeManager.h"
#include "Employee.h"
#include <algorithm>

void EmployeeManager::add(std::unique_ptr<Employee> e) {
    employees_.push_back(std::move(e));
}

void EmployeeManager::printAll() const {
    for (const auto& e : employees_) {
        e->printInfo();
    }
}

void EmployeeManager::sortBySalary() {
    std::sort(employees_.begin(), employees_.end(),
        [] (const std::unique_ptr<Employee>& a, const std::unique_ptr<Employee>& b) {
            return a->calcSalary() < b->calcSalary();
        }
    );
}