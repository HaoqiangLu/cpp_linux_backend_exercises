#include "MapExercises.h"
#include "PhoneBook.h"
#include "StudentLookup.h"
#include <iostream>

int main() {
    std::cout << "练习1 统计文章中每个单词出现次数" << std::endl;
    std::string text{"hello Hello heLLo word Word era Era year"};
    std::unordered_map<std::string, int> wordFreq = wordCount(text);
    for (const auto& pair : wordFreq) {
        std::cout << "[" << pair.first << "]:" << pair.second << std::endl;
    }

    std::cout << "========================\n";

    std::cout << "练习2 统计数字出现频率" << std::endl;
    std::vector<int> nums = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    std::unordered_map<int, int> freq = countFrequency(nums);
    for (const auto& pair : freq) {
        std::cout << "[" << pair.first << "]: " << pair.second << std::endl;
    }

    std::cout << "========================\n";

    std::cout << "练习3 统计登录次数" << std::endl;
    std::vector<std::string> logLines = {
        "[2024-01-15 10:30:00] user:alice login",
        "[2024-01-15 11:00:00] user:bob login",
        "[2024-01-15 12:15:00] user:alice login",
        "[2024-01-15 13:00:00] user:charlie login",
        "[2024-01-15 14:30:00] user:alice login",
        "[2024-01-15 15:00:00] user:bob login",
    };
    std::unordered_map<std::string, int> loginCount = countLogins(logLines);
    for (const auto& pair : loginCount) {
        std::cout << "[" << pair.first << "]:" << pair.second << std::endl;
    }
}