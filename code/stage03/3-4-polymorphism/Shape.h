#pragma once

// 练习3：Shape 体系抽象基类
class Shape {
public:
    virtual ~Shape() = default;

    virtual double area() const = 0;
    virtual void printInfo() const;
};