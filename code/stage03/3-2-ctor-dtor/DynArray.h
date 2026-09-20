#pragma once
#include <cstddef>

// 练习3：管理动态数组的类
class DynArray {
public:
    explicit DynArray(std::size_t n);
    ~DynArray();
    DynArray(const DynArray& other);            // 深拷贝
    DynArray& operator=(const DynArray& other); // 拷贝赋值

private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
};