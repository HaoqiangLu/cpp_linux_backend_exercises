#include "Manager.h"
#include "Employee.h"
#include <utility>


Manager::Manager(std::string n, double base)
    : Employee(std::move(n)), baseSalary_(base) {

}

double Manager::calcSalary() const {
    return baseSalary_ * 1.2;
}