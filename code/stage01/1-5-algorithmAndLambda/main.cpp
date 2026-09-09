#include "AlgorithmExercises.h"
#include "Student.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "练习1 vector<int> 升序、降序排序" << std::endl;
    std::vector<int> v1 = {5, 2, 9, 1, 5, 6};
    sortDemo(v1);

    std::cout << "练习2 学生结构体按分数排序" << std::endl;
    std::vector<Student> students = {
        Student("Alice", 90),
        Student("Bob", 85),
        Student("Charlie", 95),
        Student("David", 88)
    };
    sortStudents(students);
    printAll(students);

    std::cout << "练习3 在vector中查找某元素" << std::endl;
    std::vector<int> v2 = {5, 2, 9, 1, 5, 6};
    std::cout << std::boolalpha << contains(v2, 3) << std::endl;
    std::cout << std::boolalpha << contains(v2, 5) << std::endl;

    std::cout << "练习4 for_each + Lambda 打印容器所有元素" << std::endl;
    printAll(v2);

    std::cout << "练习5 多种Lambda排序" << std::endl;
    std::vector<int> v3 = {5, 2, 9, 1, 5, 6};
    std::vector<std::string> v4 = {"apple", "banana", "cherry", "date"};
    customIntSorts(v3);
    customStringSorts(v4);
}