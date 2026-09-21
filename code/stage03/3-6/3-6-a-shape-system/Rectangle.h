#pragma once
#include "Shape.h"

class Rectangle : public Shape {
public:
    Rectangle(double w, double h);
    double area() const override;
    double perimeter() const override;
    void printInfo() const override;

private:
    double w_ = 0.0;
    double h_ = 0.0;
};