#pragma once

// 练习2&4：BankAccount 类
class BankAccount {
private:
    double balance_ = 0.0;

public:
    explicit BankAccount(double init = 0.0);

    bool deposit(double amount);    // 存款，返回是否成功
    bool withdraw(double amount);   // 取款，余额不足返回 false
    double getBalance() const;      // 查询余额
};