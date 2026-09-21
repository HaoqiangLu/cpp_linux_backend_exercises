#include "Rectangle.h"

Rectangle::Rectangle(double w, double h) : width_(w), height_(h) {

}

double Rectangle::area() const {
    return width_ * height_;
}