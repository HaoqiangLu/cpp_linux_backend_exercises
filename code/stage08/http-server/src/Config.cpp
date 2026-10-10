#include "Config.h"
#include <fstream>
#include <string>

namespace {
// 去除首位空白（含 \r）
std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) {
        return "";
    }
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}
}   // namespace

bool Config::load(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) {
        return false;   // 打不开直接失败
    }

    std::string line;
    while (std::getline(ifs, line)) {
        std::string entry = trim(line);
        // 跳过空行与 '#' 注释行
        if (entry.empty() || entry.front() == '#') continue;

        // 按第一个 '=' 分割 key=value
        auto pos = entry.find('=');
        if (pos == std::string::npos) continue; // 无 '=' 的非法行忽略

        std::string key = trim(entry.substr(0, pos));
        std::string value = trim(entry.substr(pos + 1));
        if (key.empty()) continue;

        data_[key] = value; // 同名 key 后者覆盖前者
    }
    return true;
}

int Config::getInt(const std::string& key, int def) const {
    auto it = data_.find(key);
    if (it == data_.end()) {
        return def;
    }
    try {
        return std::stoi(it->second);   // 值非数字时兜底返回默认值，避免抛异常崩溃
    } catch (...) {
        return def;
    }
}

std::string Config::getString(const std::string& key, const std::string& def) const {
    auto it = data_.find(key);
    return it == data_.end() ? def : it->second;
}