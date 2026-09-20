#pragma once
#include "Shape.h"

class Rectangle : public Shape {
public:
    Rectangle(double w, double h);
    double area() const override;

private:
    double width_ = 0.0;
    double height_ = 0.0;
};