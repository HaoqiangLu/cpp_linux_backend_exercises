#pragma once
#include <string>
#include <unordered_map>

// 练习6：简易配置文件解析器（key=value）
std::unordered_map<std::string, std::string> parseConfig(const std::string& filename);