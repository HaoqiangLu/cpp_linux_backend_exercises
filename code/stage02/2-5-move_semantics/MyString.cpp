#include "MyString.h"
#include <algorithm>
#include <cstring>

// 构造
MyString::MyString(const char* s) {
    if (s == nullptr) {
        this->data_ = nullptr;
        return;
    }
    std::size_t len = std::strlen(s);
    this->data_ = new char[len + 1];
    std::strcpy(this->data_, s);
}

// 析构
MyString::~MyString() {
    delete[] this->data_;
}

// 拷贝构造
MyString::MyString(const MyString& other) {
    if (other.data_ == nullptr) {
        this->data_ = nullptr;
        return;
    }
    auto len = std::strlen(other.data_);
    this->data_ = new char[len + 1];
    std::strcpy(this->data_, other.data_);
}

// 拷贝赋值
MyString& MyString::operator=(const MyString& other) {
    if (this == &other) {
        return *this;
    }
    delete[] this->data_;
    if (other.data_ == nullptr) {
        this->data_ = nullptr;
        return *this;
    }
    auto len = std::strlen(other.data_);
    this->data_ = new char[len + 1];
    std::strcpy(this->data_, other.data_);
    return *this;
}

// 移动构造
MyString::MyString(MyString&& other) noexcept {
    this->data_ = other.data_;
    other.data_ = nullptr;
}

// 移动赋值
MyString& MyString::operator=(MyString&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] this->data_;
    this->data_ = other.data_;
    other.data_ = nullptr;
    return *this;
}