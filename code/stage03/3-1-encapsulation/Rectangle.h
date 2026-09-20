#pragma once
class Rectangle {
private:
    double width_ = 0.0;
    double height_ = 0.0;

public:
    Rectangle(double w, double h);
    double area() const;
    double perimeter() const;
};