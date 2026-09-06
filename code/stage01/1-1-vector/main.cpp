#include "GradeBook.h"
#include "VectorExercises.h"
#include <iostream>

int main() {
    std::vector<int> v{
        1, 2, 3, 4, 5,
        6, 7, 8, 9, 8,
        7, 6, 5, 4, 3,
        2, 1
    };

    reversePrint(v);

    statistics(v);

    int target;
    std::cout << "Enter a target value to remove: ";
    std::cin >> target;
    removeElement(v, target);

    emplaceInsertClear();

    std::cout << "==========================" << std::endl;

    GradeBook gb;
    gb.addGrade(85.5);
    gb.addGrade(90.0);
    gb.addGrade(78.0);
    gb.addGrade(58.5);

    std::cout << "All grades:" << std::endl;
    gb.printAll();

    std::cout << "Average grade: " << gb.average() << std::endl;

    gb.removeLast();
    std::cout << "After removing the last grade:" << std::endl;
    gb.printAll();
}