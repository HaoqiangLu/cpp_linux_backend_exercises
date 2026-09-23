#include "FileIoExercises.h"

#include <fstream>
#include <iostream>
#include <string>

void copyFile(const std::string& src, const std::string& dst) {
    std::ifstream ifs(src, std::ios::binary);
    std::ofstream ofs(dst, std::ios::binary | std::ios::trunc);

    ofs << ifs.rdbuf();
}

void catFile(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    std::cout << ifs.rdbuf();
}

std::streamsize fileSize(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary | std::ios::in);
    ifs.seekg(0, std::ios::end);
    return static_cast<std::streamsize>(ifs.tellg());
}

void readLines(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::in);
    std::string line;
    while (std::getline(ifs, line)) {
        std::cout << line << std::endl;
    }
}

void writeLog(const std::string& filename, const std::string& msg) {
    std::ofstream ofs(filename, std::ios::out | std::ios::app);
    ofs << msg << std::endl;
}