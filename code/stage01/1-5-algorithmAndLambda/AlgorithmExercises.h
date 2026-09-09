#pragma once
#include "Student.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

// 练习1 vector<int> 升序、降序排序
void sortDemo(std::vector<int>& v);

// 练习2 学生结构体按分数排序
void sortStudents(std::vector<Student>& students);

// 练习3 在vector中查找某元素
bool contains(const std::vector<int>& v, int target);

// 练习4 for_each + Lambda 打印容器所有元素
template <typename T>
void printAll(const std::vector<T>& v) {
    std::for_each(v.begin(), v.end(),
        [] (const T& x) {
            std::cout << x << ' ';
        }
    );
    std::cout << std::endl;
}

// 练习5 多种Lambda排序
void customIntSorts(std::vector<int>& v);
void customStringSorts(std::vector<std::string>& v);