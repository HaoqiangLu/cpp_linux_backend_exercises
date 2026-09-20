#pragma once
#include <string>

// 练习1：Animal 基类
class Animal {
public:
    Animal(std::string n, int a);
    virtual ~Animal();

    virtual void printInfo() const; // 打印名字、年龄

protected:
    std::string name_;
    int age_ = 0;
};