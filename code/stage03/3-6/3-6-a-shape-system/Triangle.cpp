#include "Triangle.h"
#include <cmath>
#include <iostream>

Triangle::Triangle(double a, double b, double c) : a_(a), b_(b), c_(c) {

}

double Triangle::area() const {
    // Heron's formula
    double s = (a_ + b_ + c_) / 2;
    return std::sqrt(s * (s - a_) * (s - b_) * (s - c_));
}

double Triangle::perimeter() const {
    return a_ + b_ + c_;
}

void Triangle::printInfo() const {
    std::cout << "Triangle: a = " << a_ << ", b = " << b_ << ", c = " << c_ << std::endl;
    std::cout << "        Area = " << area() << std::endl;
    std::cout << "        Perimeter = " << perimeter() << std::endl;
}
