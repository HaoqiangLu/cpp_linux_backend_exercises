#include "FileLogger.h"
#include <fstream>
#include <utility>

FileLogger::FileLogger(std::string f)
    : filename_(std::move(f)) {
}

void FileLogger::write(const std::string& msg) const {
    std::ofstream ofs(filename_, std::ios::app);
    ofs << msg << std::endl;
}