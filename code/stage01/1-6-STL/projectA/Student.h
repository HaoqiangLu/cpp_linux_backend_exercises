#pragma once
#include <string>
#include <iostream>

class Student {
private:
    std::string name_;
    double score_ = 0.0;

public:
    Student() = default;
    Student(const std::string& name, double score)
        : name_(name), score_(score) {}

    std::string getName() const { return this->name_; }
    double getScore() const { return this->score_; }

    void setName(const std::string& name) { this->name_ = name; }
    void setScore(double score) { this->score_ = score; }

    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << "(" << student.name_ << ", " << student.score_ << ")";
        return os;
    }
};