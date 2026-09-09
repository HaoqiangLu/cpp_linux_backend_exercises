#pragma once
#include <string>
#include <unordered_map>

// 练习5 学生姓名->分数，按姓名查询
class StudentLookup {
public:
    void add(const std::string& name, double score);
    double query(const std::string& name) const;

private:
    std::unordered_map<std::string, double> records_;
};