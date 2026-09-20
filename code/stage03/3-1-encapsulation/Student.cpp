#include "Student.h"
#include <iostream>

Student::Student(std::string n, int a, double s)
    : name_(n), age_(a), score_(s) {}

void Student::printInfo() const {
    std::cout << "Name:" << name_
        << ", Age:" << age_
        << ", Score:" << score_
        << std::endl;
}