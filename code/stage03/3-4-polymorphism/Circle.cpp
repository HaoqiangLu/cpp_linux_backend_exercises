#include "Circle.h"
#include <cmath>

Circle::Circle(double r) : radius_(r) {

}

double Circle::area() const {
    return M_PI * radius_ * radius_;
}