#include "Triangle.h"

Triangle::Triangle(double b, double h) : base_(b), height_(h) {

}

double Triangle::area() const {
    return 0.5 * base_ * height_;
}