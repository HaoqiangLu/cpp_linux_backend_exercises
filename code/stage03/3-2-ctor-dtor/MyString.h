#pragma once

// 练习5：MyString 类
class MyString {
public:
    explicit MyString(const char* s);
    ~MyString();
    MyString(const MyString& other);
    MyString& operator=(const MyString& other);

    const char* c_str() const noexcept { return data_; }

private:
    char* data_ = nullptr;
};