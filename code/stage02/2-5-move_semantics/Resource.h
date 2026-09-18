#pragma once
#include <cstddef>

// 练习1&2：四件套 + 日志打印
class Resource {
private:
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    explicit Resource(std::size_t n);
    ~Resource();

    Resource(const Resource& other);                // 拷贝构造（深拷贝）
    Resource& operator=(const Resource& other);     // 拷贝赋值
    Resource(Resource&& other) noexcept;            // 移动构造
    Resource& operator=(Resource&& other) noexcept; // 移动赋值

    std::size_t size() const noexcept { return this->size_; }
};