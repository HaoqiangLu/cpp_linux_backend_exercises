#include "ConsoleLogger.h"
#include "Developer.h"
#include "FileLogger.h"
#include "Logger.h"
#include "Manager.h"
#include "PolyDemo.h"
#include <iostream>
#include <memory>
#include <vector>

int main() {
    // 练习1&2：图形面积多态体系
    std::cout << "=== 图形面积多态 ===" << std::endl;
    polyDemo();

    // 练习3：员工工资系统
    std::cout << "\n=== 员工工资系统 ===" << std::endl;
    std::vector<std::unique_ptr<Employee>> employees;
    employees.push_back(std::make_unique<Manager>("Alice", 10000.0));
    employees.push_back(std::make_unique<Developer>("Bob", 160, 50.0));
    employees.push_back(std::make_unique<Manager>("Charlie", 15000.0));
    employees.push_back(std::make_unique<Developer>("Dave", 120, 80.0));

    for (const auto& emp : employees) {
        emp->printInfo();
    }

    // 练习4：日志系统
    std::cout << "\n=== 日志系统 ===" << std::endl;
    std::vector<std::unique_ptr<Logger>> loggers;
    loggers.push_back(std::make_unique<ConsoleLogger>());
    loggers.push_back(std::make_unique<FileLogger>("test.log"));

    for (const auto& logger : loggers) {
        logger->log("Hello, polymorphism!");
    }

    return 0;
}