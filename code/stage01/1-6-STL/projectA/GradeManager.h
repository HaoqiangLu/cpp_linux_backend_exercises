#pragma once
#include <string>
#include <vector>
#include "Student.h"

class GradeManager {
private:
    std::vector<Student> student_;

public:
    void add(const std::string& name, double score);
    void remove(const std::string& name);
    Student* find(const std::string& name);
    void updateScore(const std::string& name, double score);
    void sortByScore();
    double average() const;
    double highest() const;
    double lowest() const;
    void printAll() const;
};