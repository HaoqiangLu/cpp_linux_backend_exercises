#include "AlgorithmExercises.h"
#include "Student.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <sys/types.h>

void sortDemo(std::vector<int>& v) {
    std::sort(v.begin(), v.end());  // 升序
    printAll(v);

    std::sort(v.begin(), v.end(), std::greater<int>());
    printAll(v);
}

void sortStudents(std::vector<Student>& students) {
    // 按 score 降序
    std::sort(students.begin(), students.end(),
        [] (const Student& a, const Student& b) {
            return a.getScore() > b.getScore();
        }
    );
}

bool contains(const std::vector<int>& v, int target) {
    return std::find_if(v.begin(), v.end(),
                [&] (int x) {
                    return x == target;
                }
            ) != v.end();
}

void customIntSorts(std::vector<int>& v) {
    // 按绝对值排序(std::abs)
    std::sort(v.begin(), v.end(),
        [] (int a, int b) {
            return std::abs(a) < std::abs(b);
        }
    );
    printAll(v);
}

void customStringSorts(std::vector<std::string>& v) {
    // 按字符串长度排序(std::vector<std::string>)
    std::sort(v.begin(), v.end(),
        [] (const std::string& a, const std::string& b) {
            return a.size() < b.size();
        }
    );
    printAll(v);
}