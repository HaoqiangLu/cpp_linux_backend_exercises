#pragma once
#include "Shape.h"
#include <memory>
#include <vector>

class ShapeManager {
public:
    void add(std::unique_ptr<Shape> s);
    void printAll() const;
    double totalArea() const;

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};