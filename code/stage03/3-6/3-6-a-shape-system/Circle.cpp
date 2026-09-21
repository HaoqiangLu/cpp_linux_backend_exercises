#include "Circle.h"
#include <iostream>
#include <numbers>

Circle::Circle(double r) : radius_(r) {

}

double Circle::area() const {
    return std::numbers::pi * radius_ * radius_;
}

double Circle::perimeter() const {
    return 2 * std::numbers::pi * radius_;
}

void Circle::printInfo() const {
    std::cout << "Circle: radius = " << radius_ << std::endl;
    std::cout << "        Area = " << area() << std::endl;
    std::cout << "        Perimeter = " << perimeter() << std::endl;
}