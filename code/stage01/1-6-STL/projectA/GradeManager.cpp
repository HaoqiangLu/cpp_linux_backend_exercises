#include "GradeManager.h"
#include "Student.h"
#include <algorithm>
#include <iostream>
#include <numeric>

void GradeManager::add(const std::string& name, double score) {
    student_.emplace_back(name, score);
}

void GradeManager::remove(const std::string& name) {
    // std::erase_if(student_, [&name] (const Student& stu) { return stu.getName() == name; });
    student_.erase(std::remove_if(student_.begin(), student_.end(),
        [&name] (const Student& stu) { return stu.getName() == name; }
    ),
    student_.end()
    );
}

Student* GradeManager::find(const std::string& name) {
    auto it = std::find_if(student_.begin(), student_.end(),
                [&name] (const Student& stu) { return stu.getName() == name; }
            );
    return it != student_.end() ? &(*it) : nullptr;
}

void GradeManager::updateScore(const std::string& name, double score) {
    auto stu = find(name);
    if (stu != nullptr) {
        stu->setScore(score);
    }
}

void GradeManager::sortByScore() {
    std::sort(student_.begin(), student_.end(),
        [] (const Student& a, const Student& b) {
            return a.getScore() < b.getScore();
        }
    );
}

double GradeManager::average() const {
    if (student_.empty())
        return 0.0;

    double sum = std::accumulate(student_.begin(), student_.end(), 0.0,
        [] (double acc, const Student& stu) {
            return acc + stu.getScore();
        }
    );

    return sum / static_cast<double>(student_.size());
}

double GradeManager::highest() const {
    if (student_.empty())
        return 0.0;

    auto it = std::max_element(student_.begin(), student_.end(),
        [] (const Student& a, const Student& b) {
            return a.getScore() < b.getScore();
        }
    );
    return it != student_.end() ? it->getScore() : 0.0;
}

double GradeManager::lowest() const {
    if (student_.empty())
        return 0.0;

    auto it = std::min_element(student_.begin(), student_.end(),
        [] (const Student& a, const Student& b) {
            return a.getScore() < b.getScore();
        }
    );
    return it != student_.end() ? it->getScore() : 0.0;
}

void GradeManager::printAll() const {
    for (const auto& stu : student_) {
        std::cout << stu << std::endl;
    }
}