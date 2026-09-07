#include "StringExercises.h"
#include <iostream>
#include <string>

int main() {
    // std::cout << "练习1 统计单词个数" << std::endl;
    // std::cout << "请输入一句话：";
    // std::string sentence;
    // std::getline(std::cin, sentence);
    // std::cout << "单词个数为：" << countWords(sentence) << std::endl;

    // std::cout << "======================\n";

    // std::cout << "练习2 替换空格" << std::endl;
    // std::string str1(sentence);
    // std::cout << "替换空格后的字符串为：" << replaceSpaces(str1) << std::endl;

    // std::cout << "======================\n";

    // std::cout << "练习3 判断回文" << std::endl;
    // std::cout << "请输入一个字符串：";
    // std::string str2;
    // std::getline(std::cin, str2);
    // std::cout << "字符串" << (isPalindrome(str2) ? "是" : "不是") << "回文" << std::endl;

    // std::cout << "======================\n";

    // std::cout << "练习4 最频繁的单词" << std::endl;
    // std::cout << "请输入一个字符串：";
    // std::string str3;
    // std::getline(std::cin, str3);
    // std::cout << "最频繁的单词是：" << mostFrequentWord(str3) << std::endl;

    // std::cout << "======================\n";

    std::cout << "练习5 字符串分割" << std::endl;
    std::string str4{"123,456,789"};
    std::cout << "原字符串：" << str4 << std::endl;
    std::cout << "按逗号分割后的各子字符串为：" << std::endl;
    std::vector<std::string> result = split(str4, ',');
    for (const auto& s : result) {
        std::cout << s << std::endl;
    }
}