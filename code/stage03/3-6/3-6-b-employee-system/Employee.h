#pragma once
#include <string>

class Employee {
public:
    explicit Employee(const std::string n);
    virtual ~Employee() = default;

    virtual double calcSalary() const = 0;
    virtual void printInfo() const;

protected:
    std::string name_;
};