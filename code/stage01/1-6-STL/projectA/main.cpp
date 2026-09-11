#include "GradeManager.h"
#include <iostream>

int main() {
    GradeManager gradeManager;
    gradeManager.add("Alice", 95.5);
    gradeManager.add("Bob", 78.0);
    gradeManager.add("Charlie", 87.0);
    gradeManager.add("David", 92.0);

    std::cout << "[All Students]:" << std::endl;
    gradeManager.printAll();

    std::cout << "Average: " << gradeManager.average() << std::endl;
    std::cout << "Highest: " << gradeManager.highest() << std::endl;
    std::cout << "Lowest: " << gradeManager.lowest() << std::endl;

    std::cout << "[Updated Alice's Score(96.0)]:" << std::endl;
    gradeManager.updateScore("Alice", 96.0);
    gradeManager.printAll();

    std::cout << "[Sorted by Score]:" << std::endl;
    gradeManager.sortByScore();
    gradeManager.printAll();

    std::cout << "[Found Bob]:" << std::endl;
    Student* stu1 = gradeManager.find("Bob");
    std::cout << "Found: " << *stu1 << std::endl;

    std::cout << "[Removed Bob]:" << std::endl;
    gradeManager.remove("Bob");
    gradeManager.printAll();

    std::cout << "[Found Bob Again]:" << std::endl;
    Student* stu2 = gradeManager.find("Bob");
    std::cout << "Not Found: " << (stu2 == nullptr ? "nullptr" : "still there") << std::endl;
}
