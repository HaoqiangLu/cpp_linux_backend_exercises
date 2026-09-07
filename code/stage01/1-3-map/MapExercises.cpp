#include "MapExercises.h"
#include <sstream>
#include <algorithm>

std::unordered_map<std::string, int> wordCount(const std::string& text) {
    std::unordered_map<std::string, int> freq;
    std::string cleaned{text};

    std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(),
        [] (unsigned char c) { return std::isalpha(c) ? std::tolower(c) : ' '; }
    );

    std::istringstream iss(cleaned);
    std::string word;
    while (iss >> word) {
        freq[word]++;
    }

    return freq;
}

std::unordered_map<int, int> countFrequency(const std::vector<int>& nums) {
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        freq[num]++;
    }
    return freq;
}

std::unordered_map<std::string, int> countLogins(const std::vector<std::string>& logLines) {
    std::unordered_map<std::string, int> loginCount;
    for (const std::string& logLine : logLines) {
        auto pos = logLine.find("user:");
        if (pos == std::string::npos) continue;
        auto startPos = pos + 5;
        auto endPos = logLine.find(' ', startPos);
        if (endPos == std::string::npos) endPos = logLine.size();
        loginCount[logLine.substr(startPos, endPos - startPos)]++;
    }
    return loginCount;
}