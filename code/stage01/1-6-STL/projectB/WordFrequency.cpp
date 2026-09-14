#include "WordFrequency.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

void WordFrequency::loadFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) return;

    std::string word;
    char c;
    while (ifs.get(c)) {
        if (std::isalpha(c)) {
            word += std::tolower(c);
        } else {
            if (!word.empty()) {
                freq_[word]++;
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        freq_[word]++;
    }
}

std::vector<std::pair<std::string, int>> WordFrequency::top10() const {
    auto n = std::min(freq_.size(), size_t(10));
    std::vector<std::pair<std::string, int>> vec(freq_.begin(), freq_.end());

    std::nth_element(vec.begin(), vec.begin() + n, vec.end(),
        [] (const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        }
    );
    vec.resize(n);
    std::sort(vec.begin(), vec.end(),
        [] (const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        }
    );

    return vec;
}

void WordFrequency::print() const {
    int i = 0;
    for (const auto& p : freq_) {
        std::cout << "[" << ++i << "]. " << p.first << ", " << p.second << std::endl;
    }
}