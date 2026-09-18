#pragma once

// 练习5：MyString 类
class MyString {
private:
    char* data_ = nullptr;

public:
    MyString(const char* s);
    ~MyString();

    MyString(const MyString& other);
    MyString& operator=(const MyString& other);
    MyString(MyString&& other) noexcept;
    MyString& operator=(MyString&& other) noexcept;

    const char* c_str() const noexcept { return data_; }
};