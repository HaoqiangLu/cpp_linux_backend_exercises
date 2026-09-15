#pragma once
#include <map>
#include <string>
#include <vector>

// 练习1：遍历 vector<int> 并打印
void printVector(const std::vector<int>& v);

// 练习2：范围 for 将所有元素乘以 2（需引用）
void doubleElements(std::vector<int>& v);

// 练习3：遍历 map 打印键值对
void printMap(const std::map<std::string, int>& m);

// 练习4：遍历 vector<string> 统计总字符数（const 引用避免拷贝）
int totalChars(const std::vector<std::string>& v);