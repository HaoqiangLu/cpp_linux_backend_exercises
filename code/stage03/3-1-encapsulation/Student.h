#pragma once
#include <string>

// 练习1：Student 类
class Student {
private:
    std::string name_;
    int age_ = 0;
    double score_ = 0.0;

public:
    Student(std::string n, int a, double s);
    void printInfo() const;
};