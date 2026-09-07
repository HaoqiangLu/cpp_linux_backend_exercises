#pragma once
#include <string>
#include <unordered_map>

// 练习2 学生姓名->分数，按姓名查询
class StudentLookup {
public:
    void add(const std::string& name, int score);
    int query(const std::string& name) const;

private:
    std::unordered_map<std::string, int> records_;
};