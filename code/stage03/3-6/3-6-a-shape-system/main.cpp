#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "ShapeManager.h"
#include <iostream>
#include <memory>

int main() {
    ShapeManager manager;

    // 添加多种图形
    manager.add(std::make_unique<Circle>(2.0));
    manager.add(std::make_unique<Rectangle>(3.0, 4.0));
    manager.add(std::make_unique<Triangle>(3.0, 4.0, 5.0));
    manager.add(std::make_unique<Circle>(1.5));

    // 测试 printAll：输出所有图形信息
    std::cout << "=== 所有图形 ===" << std::endl;
    manager.printAll();

    // 测试 totalArea：计算总面积
    std::cout << "\n=== 总面积 ===" << std::endl;
    std::cout << manager.totalArea() << std::endl;

    return 0;
}
