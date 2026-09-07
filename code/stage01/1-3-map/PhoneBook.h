#pragma once
#include <string>
#include <unordered_map>

// 练习4 简易电话本
class PhoneBook {
public:
    void add(const std::string& name, const std::string& phone);
    void remove(const std::string& name);
    std::string lookup(const std::string& name) const;
    void printAll() const;

private:
    std::unordered_map<std::string, std::string> contacts_; // name -> phone
};