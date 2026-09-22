#include "ConfigParser.h"
#include <fstream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> parseConfig(const std::string& filename) {
    std::unordered_map<std::string, std::string> config;
    std::ifstream ifs(filename, std::ios::in);
    if (!ifs.is_open()) {
        return {};
    }

    std::string line;
    while (std::getline(ifs, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            config[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }

    return config;
}