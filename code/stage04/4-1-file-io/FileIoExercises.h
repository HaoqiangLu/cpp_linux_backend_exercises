#pragma once
#include <string>
#include <sys/types.h>

// 练习1：简化版 cp（复制文件）
void copyFile(const std::string& src, const std::string& dst);

// 练习2：简化版 cat（打印文件内容）
void catFile(const std::string& filename);

// 练习3：统计文件大小
off_t fileSize(const std::string& filename);

// 练习4：逐行读取文本文件
void readLines(const std::string& filename);

// 练习5：日志写入（追加模式）
void writeLog(const std::string& filename, const std::string& msg);