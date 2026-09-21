#include "PolyDemo.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Triangle.h"
#include <iostream>
#include <memory>
#include <vector>

void polyDemo() {
    std::vector<std::unique_ptr<Shape>> shapes;
    // shapes.push_back(std::make_unique<Circle>(1.0)); ...
    auto circle = std::make_unique<Circle>(1.0);
    auto rectangle = std::make_unique<Rectangle>(2.0, 3.0);
    auto triangle = std::make_unique<Triangle>(4.0, 5.0);

    shapes.push_back(std::move(circle));
    shapes.push_back(std::move(rectangle));
    shapes.push_back(std::move(triangle));

    // 遍历调用 area()，观察多态行为
    for (const auto& shape : shapes) {
        std::cout << shape->area() << std::endl;
    }
}