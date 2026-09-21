#pragma once
#include "Base.h"

class Derived : public Base {
public:
    Derived(int val, int extraVal);
    ~Derived();

private:
    int* extra_ = nullptr;
};