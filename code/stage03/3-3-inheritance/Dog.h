#pragma once
#include "Animal.h"
#include <string>

// 练习2：Dog 派生
class Dog : public Animal {
public:
    Dog(std::string n, int a, std::string b);
    ~Dog() override;
    void printInfo() const override;    // 打印信息 + 品种
    void bark() const;                  // 打印“汪汪”

private:
    std::string breed_;
};