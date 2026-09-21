#pragma once

// 练习1：基类/派生类均动态分配资源
class Base {
public:
    explicit Base(int val);

    /*
    先不加 virtual，观察问题
    然后改为 virtual ~Base();
    */
    virtual ~Base();

protected:
    int* data_ = nullptr;
};