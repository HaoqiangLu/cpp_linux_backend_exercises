#include "LambdaExercises.h"
#include <iostream>

int main() {
    // 练习1：Lambda 计算两数之和
    auto add = [](int a, int b) { return a + b; };
    std::cout << "Sum: " << add(3, 4) << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sortDescending(vec);
    for (int i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::vector<int> evenVec = filterEven(vec);
    for (int i : evenVec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    int count = countAbove(vec, 5);
    std::cout << "Count above 5: " << count << std::endl;

    std::vector<Student> students = {
        {"Alice", 85},
        {"Bob", 92},
        {"Charlie", 78}
    };
    sortStudents(students);
    for (Student& s : students) {
        std::cout << s.name << " " << s.score << std::endl;
    }
}