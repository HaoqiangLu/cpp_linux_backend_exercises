#include "EmployeeManager.h"
#include "Manager.h"
#include "Programmer.h"
#include <iostream>

int main() {
    EmployeeManager manager;

    // 添加多种员工
    manager.add(std::make_unique<Manager>("Alice", 10000.0));
    manager.add(std::make_unique<Programmer>("Bob", 160, 50.0));
    manager.add(std::make_unique<Manager>("Charlie", 15000.0));
    manager.add(std::make_unique<Programmer>("Dave", 120, 80.0));

    // 测试 printAll：输出所有员工信息
    std::cout << "=== 排序前 ===" << std::endl;
    manager.printAll();

    // 测试 sortBySalary：按工资排序
    manager.sortBySalary();

    std::cout << "\n=== 按工资排序后 ===" << std::endl;
    manager.printAll();

    return 0;
}