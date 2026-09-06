#pragma once
#include <vector>

class GradeBook {
public:
    void addGrade(double g);    // 添加成绩
    void removeLast();          // 删除末尾成绩
    void clear();               // 清空所有成绩
    void printAll() const;      // 打印全部成绩
    double average() const;     // 计算并返回平均分

private:
    std::vector<double> grades_;
};