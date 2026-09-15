#include "RangeForExercises.h"
#include <iostream>

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    printVector(v1);
    doubleElements(v1);
    printVector(v1);

    std::map<std::string, int> m1 = {{"one", 1}, {"two", 2}, {"three", 3}};
    printMap(m1);

    std::vector<std::string> v2 = {"hello", "world", "total", "characters"};
    std::cout << "Total characters: " << totalChars(v2) << std::endl;
}