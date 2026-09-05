#include "VectorExercises.h"
#include <algorithm>
#include <iostream>
#include <numeric>

void reversePrint() {
    std::vector<int> v;
    int x = 0;
    while (std::cin >> x) {
        v.push_back(x);
    }

    std::cout << "[rbegin/rend]\n";
    for (auto it = v.rbegin(); it != v.rend(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "[reverse]\n";
    std::reverse(v.begin(), v.end());
    for (auto it = v.begin(); it != v.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void statistics(const std::vector<int>& v) {

}

void removeElement(std::vector<int>& v, int target) {

}