#include "Developer.h"
#include "Employee.h"
#include <utility>

Developer::Developer(std::string n, int h, double rate)
    : Employee(std::move(n)), hours_(h), hourlyRate_(rate) {

}

double Developer::calcSalary() const {
    return hours_ * hourlyRate_;
}