#include "FileLogger.h"
#include <fstream>
#include <utility>

FileLogger::FileLogger(std::string f)
    : filename_(f) {

}

void FileLogger::log(const std::string& msg) const {
    std::ofstream ofs(filename_, std::ios::app);
    ofs << msg << std::endl;
}