#pragma once
#include <string>
#include <ostream>

class Student {
private:
    std::string name_;
    double score_ = 0.0;
public:
    Student() = default;
    Student(const std::string& name, double score)
        : name_(name), score_(score) {}

    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << "(" << student.name_ << ", " << student.score_ << ")";
        return os;
    }

    std::string getName() const {
        return name_;
    }
    double getScore() const {
        return score_;
    }

    void setName(const std::string& name) {
        this->name_ = name;
    }
    void setScore(double score) {
        this->score_ = score;
    }
};