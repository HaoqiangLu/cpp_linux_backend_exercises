#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class WordFrequency {
private:
    std::unordered_map<std::string, int> freq_;

public:
    void loadFile(const std::string& filename);             // 逐词读取，转小写，freq_[word]++
    std::vector<std::pair<std::string, int>> top10() const; // 拷贝到 vector，排序取前 10
    void print() const;
};