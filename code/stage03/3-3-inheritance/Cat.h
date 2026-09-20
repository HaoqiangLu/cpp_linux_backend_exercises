#pragma once
#include "Animal.h"
#include <string>

class Cat : public Animal {
public:
    Cat(std::string n, int a, bool indoor);
    ~Cat() override;
    void printInfo() const override;    // 打印信息 + 室内/室外
    void meow() const;                  // 打印“喵喵”

private:
    bool isIndoor_ = false;
};
