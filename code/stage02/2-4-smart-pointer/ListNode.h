#pragma once
#include <memory>


// 练习3：unique_ptr 实现链表节点
struct ListNode {
    int val = 0;
    std::unique_ptr<ListNode> next;

    explicit ListNode(int v) : val(v), next(nullptr) {}
};