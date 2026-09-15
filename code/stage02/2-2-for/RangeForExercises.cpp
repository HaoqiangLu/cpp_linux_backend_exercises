#include "RangeForExercises.h"
#include <iostream>

void printVector(const std::vector<int>& v) {
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

void doubleElements(std::vector<int>& v) {
    for (int& x : v) {
        x *= 2;
    }
}

void printMap(const std::map<std::string, int>& m) {
    for (const auto& [key, val] : m) {
        std::cout << "[" << key << "]: " << val << std::endl;
    }
}

int totalChars(const std::vector<std::string>& v) {
    int total = 0;
    for (const auto& s : v) {
        total += static_cast<int>(s.size());
    }
    return total;
}