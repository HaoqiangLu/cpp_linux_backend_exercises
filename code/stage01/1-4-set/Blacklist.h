#pragma once
#include <unordered_set>

// 练习5 黑名单系统
class Blacklist {
public:
    void add(int id);
    void remove(int id);
    bool contains(int id) const;

private:
    std::unordered_set<int> ids_;
};