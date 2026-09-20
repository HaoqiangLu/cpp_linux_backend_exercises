#include "MyString.h"
#include <cstring>

MyString::MyString(const char* s) {
    std::size_t len = std::strlen(s);
    data_ = new char[len + 1];
    std::strcpy(data_, s);
}

MyString::~MyString() {
    delete[] data_;
}

MyString::MyString(const MyString& other) {
    std::size_t len = std::strlen(other.data_);
    data_ = new char[len + 1];
    std::strcpy(data_, other.data_);
}

MyString& MyString::operator=(const MyString& other) {
    if (this == &other) {
        return *this;
    }
    delete[] data_;
    std::size_t len = std::strlen(other.data_);
    data_ = new char[len + 1];
    std::strcpy(data_, other.data_);
    return *this;
}
