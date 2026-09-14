#include "WordFrequency.h"
#include <iostream>

int main() {
    // 测试 WordFrequency
    const std::string filename = "test.txt";
    WordFrequency wf;
    wf.loadFile(filename);

    std::cout << "=== Top 10 高频词 ===" << std::endl;
    auto top = wf.top10();
    for (const auto& [word, count] : top) {
        std::cout << word << ": " << count << std::endl;
    }

    std::cout << "\n=== 全部词频 ===" << std::endl;
    wf.print();
}