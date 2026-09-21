#pragma once
#include "Shape.h"

class Triangle : public Shape {
public:
    Triangle(double b, double h);
    double area() const override;

private:
    double base_ = 0.0;
    double height_ = 0.0;
};