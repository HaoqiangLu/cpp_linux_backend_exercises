#pragma once
#include <string>

// 练习3：员工工资系统
class Employee {
public:
    explicit Employee(std::string n);
    virtual ~Employee() = default;

    virtual double calcSalary() const = 0;  // 纯虚函数
    virtual void printInfo() const;

protected:
    std::string name_;
};