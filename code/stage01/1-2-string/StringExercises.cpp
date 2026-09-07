#include "StringExercises.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>

int countWords(const std::string& sentence) {
    int count = 0;
    std::istringstream iss(sentence);
    std::string word;
    while (iss >> word) {
        count++;
    }
    return count;
}

std::string replaceSpaces(std::string s) {
    /*
    std::replace方法没有替换成字符串的，下面写法是错误的
    std::replace(s.begin(), s.end(), ' ', "%20");

    std::string::find 函数返回的不是迭代器，而是索引位置
    std::string::replace(索引, 长度, 替换字符串);
    */
     auto pos = s.find(' ');
     while (pos != std::string::npos) {
        s.replace(pos, 1, "%20");
        pos = s.find(' ');
     }
     return s;
}

bool isPalindrome(const std::string& s) {
    for (auto l = s.cbegin(), r = s.cend() - 1; l <= r; l++, r--) {
        if (*l != *r) {
            return false;
        }
    }
    return true;
}

std::string mostFrequentWord(std::string s) {
    std::unordered_map<std::string, int> freq;
    std::string word;
    std::istringstream iss(s);
    while (iss >> word) {
        freq[word]++;
    }

    /*
    方式一 手动查找次数最多的单词
    */
    // int max = -1;
    // for (const auto& p : freq) {
    //     if (p.second > max) {
    //         word = p.first;
    //     }
    // }
    // return word;

    /*
    方式二 使用 std::max_element
    */
    auto it = std::max_element(freq.begin(), freq.end(),
                                [] (const auto& a, const auto& b) {
                                    return a.second < b.second;
                                });
    return it->first;
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;

    /*
    方式一 用 find + substr
    */
    // std::string str{s};
    // while (!str.empty()) {
    //     auto pos = str.find(delim);
    //     if (pos == std::string::npos) {
    //         result.push_back(str);
    //         return result;
    //     }
    //     result.push_back(str.substr(0, pos));
    //     str = str.substr(pos + 1);
    // }

    /*
    方式二 std::istringstream 按 delim 分割
    */
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, delim)) {
        result.push_back(token);
    }

    return result;
}