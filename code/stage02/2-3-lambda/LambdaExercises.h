#pragma once
#include <vector>
#include "Student.h"

// 练习2：Lambda 降序排序
void sortDescending(std::vector<int>& v);

// 练习3：Lambda 筛选偶数
std::vector<int> filterEven(const std::vector<int>& v);

// 练习4：捕获外部阈值，统计大于该值的元素个数
int countAbove(const std::vector<int>& v, int threshold);

// 练习5：sort+Lambda对学生按分数排序
void sortStudents(std::vector<Student>& students);