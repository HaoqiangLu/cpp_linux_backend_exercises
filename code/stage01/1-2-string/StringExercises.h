#pragma once
#include <string>
#include <vector>

// 练习1 输入一句话，统计单词数量
int countWords(const std::string& sentence);

// 练习2 将句子中所有空格替换为 %20
std::string replaceSpaces(std::string s);

// 练习3 判断字符串是否是回文
bool isPalindrome(const std::string& s);

// 练习4 读取多行文本，找出出现频率最高的单词
std::string mostFrequentWord(std::string s);

// 练习5 将“123,456,789”按逗号分割为多个子串
std::vector<std::string> split(const std::string& s, char delime);