#pragma once
#include <set>
#include <unordered_set>
#include <string>
#include <vector>

// 练习1 输入一串数字，去重后升序输出
std::set<int> deduplicateAndSort(const std::vector<int>& nums);

// 练习2 判断两个数组是否存在交集
bool hasIntersection(const std::vector<int>& a, const std::vector<int>& b);

// 练习3 统计文章中不同单词的数量
int uniqueWordCount(const std::string& text);

// 练习4 从一组数字中找出前K个不重复的数
std::vector<int> topKUnique(const std::vector<int>& nums, int k);