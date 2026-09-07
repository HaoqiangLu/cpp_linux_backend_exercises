#pragma once
#include <string>
#include <unordered_map>
#include <vector>

// 练习1 统计文章中每个单词出现次数
std::unordered_map<std::string, int> wordCount(const std::string& text);

// 练习3 统计数组中每个数字的出现频次
std::unordered_map<int, int> countFrequency(const std::vector<int>& nums);

// 练习5 解析日志，统计每个用户登录次数
std::unordered_map<std::string, int> countLogins(const std::vector<std::string>& logLines);