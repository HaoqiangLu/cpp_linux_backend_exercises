#pragma once
#include <vector>

// 练习1：输入若干整数存入vector，逆序输出
void reversePrint(std::vector<int> v);

// 练习2：统计vector中的最大值、最小值、平均值
void statistics(const std::vector<int>& v);

// 练习3：删除vector中所有等于target的元素（注意迭代器失效）
void removeElement(std::vector<int>& v, int target);

// 练习5：演示 emplace_back、insert、clear 的用法
void emplaceInsertClear();