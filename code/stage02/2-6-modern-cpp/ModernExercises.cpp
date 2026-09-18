#include "ModernExercises.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

void smartPtrRewrite() {
    // 旧代码:
    // int* arr = new int[10]; ... delete[] arr;
    // TODO: 改为 auto arr = std::make_unique<int[]>(10);
    auto arr = std::make_unique<int[]>(10);
    for (int i = 0; i < 10; ++i) {
        arr[i] = i * 10;
    }

    std::for_each(arr.get(), arr.get() + 10, [] (int& n) {
        std::cout << n << ' ';
    });
    std::cout << std::endl;
}

void lambdaRewrite() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    // 旧代码: for (std::size_t i = 0; i < v.size(); ++i) std::cout << v[i] << ' ';
    // TODO: 改为 std::for_each + Lambda
    std::for_each(v.begin(), v.end(),
        [] (const int& n){
            std::cout << n << " ";
        }
    );
    std::cout << std::endl;
}