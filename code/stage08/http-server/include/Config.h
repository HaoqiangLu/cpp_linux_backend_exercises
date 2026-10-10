#pragma once
#include <string>
#include <unordered_map>

class Config {
public:
    bool load(const std::string& path);
    int getInt(const std::string& key, int def = 0) const;
    std::string getString(const std::string& key, const std::string& def = "") const;

private:
    std::unordered_map<std::string, std::string> data_;
};