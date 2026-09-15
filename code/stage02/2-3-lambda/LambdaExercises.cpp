#include "LambdaExercises.h"
#include <algorithm>
#include <iterator>

void sortDescending(std::vector<int>& v) {
    std::sort(v.begin(), v.end(),
        [] (const int& a, const int& b) {
            return a > b;
        }
    );
}

std::vector<int> filterEven(const std::vector<int>& v) {
    std::vector<int> result;
    std::copy_if(v.begin(), v.end(), std::back_inserter(result),
        [] (const int& a) {
            return a % 2 == 0;
        }
    );
    return result;
}

int countAbove(const std::vector<int>& v, int threshold) {
    return std::count_if(v.begin(), v.end(),
        [threshold] (const int& a) {
            return a > threshold;
        }
    );
}

void sortStudents(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(),
        [] (const Student& a, const Student& b) {
            return a.score > b.score;
        }
    );
}