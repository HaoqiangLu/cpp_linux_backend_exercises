#include "ShapeManager.h"
#include <utility>

void ShapeManager::add(std::unique_ptr<Shape> s) {
    shapes_.push_back(std::move(s));
}

void ShapeManager::printAll() const {
    for (const auto& shape : shapes_) {
        shape->printInfo();
    }
}

double ShapeManager::totalArea() const {
    double sum = 0.0;
    for (const auto& shape : shapes_) {
        sum += shape->area();
    }
    return sum;
}