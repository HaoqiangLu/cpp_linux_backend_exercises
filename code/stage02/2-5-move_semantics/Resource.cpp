#include "Resource.h"
#include <algorithm>
#include <iostream>

Resource::Resource(std::size_t n) : data_(new int[n]{}), size_(n) {
    std::cout << "构造\n";
}

Resource::~Resource() {
    delete[] data_;
    std::cout << "析构\n";
}

Resource::Resource(const Resource& other) : size_(other.size_) {
    // 深拷贝 other.data_ 到 data_
    data_ = new int[other.size_];
    std::copy(other.data_, other.data_ + other.size_, this->data_);
}

Resource& Resource::operator=(const Resource& other) {
    // 自赋值保护 + 释放旧资源 + 深拷贝
    if (this == &other)
        return *this;

    this->size_ = other.size_;
    delete[] data_;
    data_ = new int[other.size_];
    std::copy(other.data_, other.data_ + other.size_, this->data_);
    return *this;
}

Resource::Resource(Resource&& other) noexcept {
    // 窃取 other.data_，并将 other 重置为安全空状态
    this->data_ = other.data_;
    this->size_ = other.size_;
    other.data_ = nullptr;
    other.size_ = 0;
}

Resource& Resource::operator=(Resource&& other) noexcept {
    // 自赋值保护 + 释放就资源 + 窃取
    if (this == &other)
        return *this;
    this->data_ = other.data_;
    this->size_ = other.size_;
    other.data_ = nullptr;
    other.size_ = 0;
    return *this;
}