#include "Cat.h"
#include "Circle.h"
#include "Dog.h"
#include "Rectangle.h"
#include "Triangle.h"

#include <iostream>
#include <memory>
#include <vector>

int main() {
    // ===== 练习1 & 2：Animal 基类 与 Dog / Cat 派生类 =====
    std::cout << "===== 练习1&2：Animal / Dog / Cat =====" << std::endl;
    {
        Dog dog("旺财", 3, "柴犬");
        dog.printInfo(); // 调用 Dog 重写版本：先 Animal::printInfo()，再打印品种
        dog.bark();      // Dog 特有行为
        std::cout << std::endl;

        Cat cat("咪咪", 2, true);
        cat.printInfo(); // 调用 Cat 重写版本：先 Animal::printInfo()，再打印室内/室外
        cat.meow();      // Cat 特有行为
    } // dog、cat 离开作用域析构
    std::cout << std::endl;

    // ===== 练习3：Shape 抽象基类 与 Circle / Rectangle / Triangle =====
    std::cout << "===== 练习3：Shape / Circle / Rectangle / Triangle =====" << std::endl;
    {
        Circle circle(2.0);
        circle.printInfo(); // 未重写，沿用 Shape::printInfo()
        std::cout << "Circle area = " << circle.area() << std::endl;

        Rectangle rect(3.0, 4.0);
        rect.printInfo();
        std::cout << "Rectangle area = " << rect.area() << std::endl;

        Triangle tri(6.0, 2.0);
        tri.printInfo();
        std::cout << "Triangle area = " << tri.area() << std::endl;
    }
    std::cout << std::endl;

    // ===== 多态：通过基类指针调用派生类重写的虚函数 =====
    std::cout << "===== 多态：Animal* / Shape* 调用虚函数 =====" << std::endl;
    {
        std::vector<std::unique_ptr<Animal>> animals;
        animals.push_back(std::make_unique<Dog>("大黄", 5, "金毛"));
        animals.push_back(std::make_unique<Cat>("小白", 1, false));
        std::cout << "-- 遍历 animals --" << std::endl;
        for (const auto &a : animals) {
            a->printInfo(); // 运行时按实际类型分派到 Dog / Cat 的 printInfo
        }

        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<Circle>(1.0));
        shapes.push_back(std::make_unique<Rectangle>(2.0, 3.0));
        shapes.push_back(std::make_unique<Triangle>(4.0, 5.0));
        std::cout << "-- 遍历 shapes 面积 --" << std::endl;
        for (const auto &s : shapes) {
            std::cout << "area = " << s->area() << std::endl; // area() 为纯虚函数，运行时分派
        }
    } // vector 析构，unique_ptr 经基类虚析构函数正确销毁派生对象
    std::cout << std::endl;

    // ===== 练习4：验证 基类构造 → 派生类构造 → 派生类析构 → 基类析构 顺序 =====
    std::cout << "===== 练习4：构造/析构顺序 =====" << std::endl;
    {
        std::cout << "-- 进入作用域，创建 Dog --" << std::endl;
        Dog dog("小黑", 4, "拉布拉多");
        std::cout << "-- Dog 已构造完成，即将离开作用域 --" << std::endl;
    } // 离开作用域：先 ~Dog（派生类），再 ~Animal（基类）
    std::cout << "-- 已离开作用域 --" << std::endl;
    std::cout << std::endl;

    std::cout << "所有练习完成！" << std::endl;
    return 0;
}
