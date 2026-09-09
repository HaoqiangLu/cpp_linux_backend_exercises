#include "PhoneBook.h"
#include <cstddef>
#include <iostream>

void PhoneBook::add(const std::string& name, const std::string& phone) {
    auto ret = contacts_.insert({name, phone});
    if (ret.second) {
        std::cout << "{" << name << ", " << phone << "}" << " added successfully" << std::endl;
    } else {
        std::cout << "{" << name << ", " << phone << "}" << " already exists" << std::endl;
    }
}

void PhoneBook::remove(const std::string& name) {
    std::size_t ret = contacts_.erase(name);
    if (ret > 0) {
        std::cout << "[" << name << "]" << " removed successfully" << std::endl;
    } else {
        std::cout << "[" << name << "]" << " does not exist" << std::endl;
    }
}

std::string PhoneBook::lookup(const std::string& name) const {
    auto it = contacts_.find(name);
    if (it != contacts_.end()) {
        return it->second;
    }
    return "";
}

void PhoneBook::printAll() const {
    for (const auto& [name, phone] : contacts_) {
        std::cout << "[" << name << "]: " << phone << std::endl;
    }
}