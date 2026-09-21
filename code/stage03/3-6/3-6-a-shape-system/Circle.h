#pragma once
#include "Shape.h"

class Circle : public Shape {
public:
    explicit Circle(double r);
    double area() const override;
    double perimeter() const override;
    void printInfo() const override;

private:
    double radius_ = 0.0;
};