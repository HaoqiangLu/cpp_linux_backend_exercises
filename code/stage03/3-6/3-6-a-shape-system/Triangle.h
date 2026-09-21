#pragma once
#include "Shape.h"

class Triangle : public Shape {
public:
    Triangle(double a, double b, double c);
    double area() const override;
    double perimeter() const override;
    void printInfo() const override;

private:
    double a_ = 0.0;
    double b_ = 0.0;
    double c_ = 0.0;
};