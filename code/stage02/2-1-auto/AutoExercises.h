#pragma once
#include <map>
#include <string>
#include <vector>

// 练习1：将显式迭代器改为 auto
void autoIteratorDemo(const std::vector<int>& v);

// 练习2：遍历 map 时使用 auto 接收键值对
void autoMapDemp(const std::map<std::string, int>& m);

// 练习3：函数返回 vector<int>，调用方用 auto 接收
std::vector<int> generateData(int n);