#include "SetExercises.h"
#include <iterator>
#include <sstream>
#include <algorithm>

std::set<int> deduplicateAndSort(const std::vector<int>& nums) {
    std::set<int> result(nums.begin(), nums.end());
    return result;
}

bool hasIntersection(const std::vector<int>& a, const std::vector<int>& b) {
    std::unordered_set<int> aSet(a.begin(), a.end());
    for (int num : b) {
        if (aSet.count(num) > 0) {
            return true;
        }
    }
    return false;
}

int uniqueWordCount(const std::string& text) {
    std::string cleaned{text};
    std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(),
        [] (unsigned char c) { return std::isalpha(c) ? std::tolower(c) : ' '; }
    );

    std::istringstream iss(cleaned);
    std::unordered_set<std::string> words;
    std::string word;
    while (iss >> word) {
        words.insert(word);
    }
    return static_cast<int>(words.size());
}

std::vector<int> topKUnique(const std::vector<int>& nums, int k) {
    std::unordered_set<int> uniqueNums(nums.begin(), nums.end());
    auto begin = uniqueNums.begin();
    auto end = std::next(begin, k);
    return std::vector<int>(begin, end);
}