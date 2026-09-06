#include "GradeBook.h"
#include <iostream>
#include <numeric>

void GradeBook::addGrade(double g) {
    grades_.emplace_back(g);
}

void GradeBook::removeLast() {
    if (!grades_.empty()) {
        grades_.pop_back();
    }
}

void GradeBook::printAll() const {
    for (std::size_t i = 0; i < grades_.size(); i++) {
        std::cout << "grade[" << i << "] = " << grades_[i] << std::endl;
    }
}

void GradeBook::clear() {
    grades_.clear();
}

double GradeBook::average() const {
    if (grades_.empty()) return 0.0;

    double avg = std::accumulate(grades_.begin(), grades_.end(), 0.0) / grades_.size();
    return avg;
}
