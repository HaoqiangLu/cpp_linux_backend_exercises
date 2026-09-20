#include "DynArray.h"
#include <algorithm>

DynArray::DynArray(std::size_t n) {
    data_ = new int[n]{};
    size_ = n;
}

DynArray::~DynArray() {
    delete[] data_;
}

DynArray::DynArray(const DynArray& other) {
    data_ = new int[other.size_];
    std::copy(other.data_, other.data_ + other.size_, data_);
    size_ = other.size_;
}

DynArray& DynArray::operator=(const DynArray& other) {
    if (this == &other) {
        return *this;
    }
    delete[] data_;
    data_ = new int[other.size_];
    std::copy(other.data_, other.data_ + other.size_, data_);
    size_ = other.size_;
    return *this;
}