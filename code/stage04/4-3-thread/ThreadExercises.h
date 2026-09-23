#pragma once
#include <string>

// 练习1：多线程打印数字
void printNumbers(int id, int start, int end);
void multiPrintDemo();

// 练习3：多线程累加计数器，观察数据竞争
void raceDemo();

// 练习5：下载模拟器
void downloadSimulator(const std::string& filename, int numChunks, int numThreads);
