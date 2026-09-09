#include "Blacklist.h"
#include "SetExercises.h"
#include <iostream>


int main() {
    std::cout << "练习1 输入一串数字，去重后升序输出" << std::endl;
    std::set<int> dedup = deduplicateAndSort({1, 1, 5, 6, 7, 9, 1, 5, 2, 3, 4, 7, 5, 6});
    for (int num : dedup) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "练习2 判断两个数组是否存在交集" << std::endl;
    std::cout << hasIntersection({1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}) << std::endl;
    std::cout << hasIntersection({1, 2, 3, 4, 5}, {5, 6, 7, 8, 9}) << std::endl;

    std::cout << "练习3 统计文章中不同单词的数量" << std::endl;
    std::cout << uniqueWordCount("This is a sample text. This text is just a sample.") << std::endl;

    std::cout << "练习4 从一组数字中找出前K个不重复的数" << std::endl;
    std::vector<int> topK = topKUnique({1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5}, 3);
    for (int num : topK) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "练习5 黑名单系统" << std::endl;
    Blacklist blacklist;
    blacklist.add(1);
    blacklist.add(2);
    blacklist.add(3);
    std::cout << "Blacklist contains 2: " << (blacklist.contains(2) ? "true" : "false") << std::endl;
    blacklist.remove(2);
    std::cout << "Blacklist contains 2 after removal: " << (blacklist.contains(2) ? "true" : "false") << std::endl;
}