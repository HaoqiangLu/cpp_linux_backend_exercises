#include "ConfigParser.h"
#include "FileIoExercises.h"

#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    // ===== 练习1：copyFile =====
    std::cout << "===== copyFile =====" << std::endl;
    copyFile("test.txt", "test_copy.txt");
    std::cout << "已复制 test.txt -> test_copy.txt" << std::endl;

    // ===== 练习2：catFile =====
    std::cout << "\n===== catFile (test_copy.txt) =====" << std::endl;
    catFile("test_copy.txt");

    // ===== 练习3：fileSize =====
    std::cout << "\n===== fileSize =====" << std::endl;
    auto size = fileSize("test.txt");
    std::cout << "test.txt 文件大小: " << size << " 字节" << std::endl;

    // ===== 练习4：readLines =====
    std::cout << "\n===== readLines (test.txt) =====" << std::endl;
    readLines("test.txt");

    // ===== 练习5：writeLog =====
    std::cout << "\n===== writeLog =====" << std::endl;
    writeLog("app.log", "[INFO] 程序启动");
    writeLog("app.log", "[WARN] 内存使用率较高");
    writeLog("app.log", "[ERROR] 连接数据库失败");
    std::cout << "已写入 3 条日志到 app.log" << std::endl;
    std::cout << "--- app.log 内容 ---" << std::endl;
    catFile("app.log");

    // ===== 练习6：parseConfig =====
    std::cout << "\n===== parseConfig =====" << std::endl;
    std::unordered_map<std::string, std::string> config = parseConfig("config.txt");
    std::cout << "解析 config.txt，共 " << config.size() << " 个配置项：" << std::endl;
    for (const auto& [key, value] : config) {
        std::cout << "  " << key << " = " << value << std::endl;
    }

    return 0;
}
