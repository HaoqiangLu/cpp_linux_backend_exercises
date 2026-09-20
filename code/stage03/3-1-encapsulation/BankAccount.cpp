#include "BankAccount.h"

BankAccount::BankAccount(double init) : balance_(init) {

}

bool BankAccount::deposit(double amount) {
    if (amount > 0) {
        balance_ += amount;
    }
    return true;
}

bool BankAccount::withdraw(double amount) {
    if (amount > 0 && amount <= this->balance_) {
        balance_ -= amount;
        return true;
    }
    return false;
}

double BankAccount::getBalance() const {
    if (balance_ >= 0.0) {
        return balance_;
    }
    return 0.0;
}