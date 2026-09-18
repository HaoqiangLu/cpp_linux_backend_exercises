#include "MoveExercises.h"
#include "MyString.h"
#include "Resource.h"
#include <iostream>
#include <utility>

int main() {
    // ---- 练习1&2：Resource 四件套 ----
    std::cout << "===== Resource =====" << std::endl;

    std::cout << "\n[构造]" << std::endl;
    Resource r1(5);

    std::cout << "\n[拷贝构造]" << std::endl;
    Resource r2 = r1;

    std::cout << "\n[拷贝赋值]" << std::endl;
    Resource r3(3);
    r3 = r1;

    std::cout << "\n[移动构造]" << std::endl;
    Resource r4 = std::move(r1);

    std::cout << "\n[移动赋值]" << std::endl;
    Resource r5(2);
    r5 = std::move(r2);

    std::cout << "\n[析构开始]" << std::endl;

    // ---- 练习3&4：moveDemo / benchmark ----
    std::cout << "\n===== moveDemo =====" << std::endl;
    moveDemo();

    std::cout << "\n===== benchmark =====" << std::endl;
    benchmark();

    // ---- 练习5：MyString 四件套 ----
    std::cout << "\n===== MyString =====" << std::endl;

    MyString s1("hello");
    std::cout << "s1 = " << s1.c_str() << std::endl;

    MyString s2 = s1;                        // 拷贝构造
    std::cout << "拷贝构造 s2 = " << s2.c_str() << std::endl;

    MyString s3("world");
    s3 = s1;                                 // 拷贝赋值
    std::cout << "拷贝赋值 s3 = " << s3.c_str() << std::endl;

    MyString s4 = std::move(s1);             // 移动构造
    std::cout << "移动构造 s4 = " << s4.c_str()
              << ", s1 = " << (s1.c_str() ? s1.c_str() : "(null)") << std::endl;

    MyString s5("temp");
    s5 = std::move(s2);                      // 移动赋值
    std::cout << "移动赋值 s5 = " << s5.c_str()
              << ", s2 = " << (s2.c_str() ? s2.c_str() : "(null)") << std::endl;
}