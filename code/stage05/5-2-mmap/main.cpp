#include "MmapExercises.h"
#include <iostream>

int main() {
    std::cout << "===== 练习1 =====" << std::endl;
    mmapReadFile("../../../README.md");

    std::cout << "\n===== 练习2 =====" << std::endl;
    anonymousMmapCounter();

    std::cout << "\n===== 练习3 =====" << std::endl;
    posixSharedMemory();

    std::cout << "\n===== 练习4 =====" << std::endl;
    sharedMemoryLog();
}