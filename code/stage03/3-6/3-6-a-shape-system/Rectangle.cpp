#include "Rectangle.h"
#include <iostream>

Rectangle::Rectangle(double w, double h)
    : w_(w), h_(h) {

}

double Rectangle::area() const {
    return w_ * h_;
}

double Rectangle::perimeter() const {
    return 2 * (w_ + h_);
}

void Rectangle::printInfo() const {
    std::cout << "Rectangle: width = " << w_ << ", height = " << h_ << std::endl;
    std::cout << "        Area = " << area() << std::endl;
    std::cout << "        Perimeter = " << perimeter() << std::endl;
}