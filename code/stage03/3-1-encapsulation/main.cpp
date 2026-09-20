#include "BankAccount.h"
#include "Rectangle.h"
#include "Student.h"

#include <iostream>

int main() {
    // ===== 练习1：Student 类测试 =====
    std::cout << "===== Student =====" << std::endl;
    Student s1("Alice", 20, 95.5);
    s1.printInfo();

    Student s2("Bob", 22, 88.0);
    s2.printInfo();

    // ===== 练习2&4：BankAccount 类测试 =====
    std::cout << "\n===== BankAccount =====" << std::endl;
    BankAccount acc1(1000.0);  // 初始余额 1000
    std::cout << "Initial balance: " << acc1.getBalance() << std::endl;

    // 存款测试
    acc1.deposit(500.0);
    std::cout << "After deposit 500: " << acc1.getBalance() << std::endl;

    // 存款负数测试（不应生效）
    bool ok = acc1.deposit(-100.0);
    std::cout << "Deposit -100 returned: " << ok
              << ", balance: " << acc1.getBalance() << std::endl;

    // 取款测试
    acc1.withdraw(300.0);
    std::cout << "After withdraw 300: " << acc1.getBalance() << std::endl;

    // 余额不足取款测试
    ok = acc1.withdraw(99999.0);
    std::cout << "Withdraw 99999 returned: " << ok
              << ", balance: " << acc1.getBalance() << std::endl;

    // 默认参数构造测试
    BankAccount acc2;
    std::cout << "Default account balance: " << acc2.getBalance() << std::endl;

    // ===== 练习3：Rectangle 类测试 =====
    std::cout << "\n===== Rectangle =====" << std::endl;
    Rectangle r1(5.0, 3.0);
    std::cout << "Rectangle(5, 3) area: " << r1.area() << std::endl;
    std::cout << "Rectangle(5, 3) perimeter: " << r1.perimeter() << std::endl;

    Rectangle r2(10.0, 4.0);
    std::cout << "Rectangle(10, 4) area: " << r2.area() << std::endl;
    std::cout << "Rectangle(10, 4) perimeter: " << r2.perimeter() << std::endl;

    return 0;
}