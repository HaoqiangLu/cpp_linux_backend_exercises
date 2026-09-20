#include "DynArray.h"
#include "Lifecycle.h"
#include "MyString.h"

#include <iostream>

int main() {
    // ===== 练习1：在构造/析构中打印日志，观察调用时机 =====
    std::cout << "===== 练习1：构造/析构日志 =====" << std::endl;
    {
        Lifecycle a("对象A");
        Lifecycle b("对象B");
    } // a, b 离开作用域，析构顺序：b 先于 a（与构造顺序相反）
    std::cout << std::endl;

    // ===== 练习2：创建对象数组，观察构造/析构顺序 =====
    std::cout << "===== 练习2：对象数组 =====" << std::endl;
    {
        Lifecycle arr[3] = {Lifecycle("数组[0]"), Lifecycle("数组[1]"), Lifecycle("数组[2]")};
    } // 析构顺序：[2] -> [1] -> [0]
    std::cout << std::endl;

    // ===== 练习3：管理动态数组的类，析构时自动释放 =====
    std::cout << "===== 练习3：DynArray =====" << std::endl;
    {
        DynArray d1(5); // 分配5个int的数组，析构时自动释放
        std::cout << "DynArray d1(5) 已创建" << std::endl;
    }
    std::cout << std::endl;

    // ===== 练习4：对比深拷贝与浅拷贝 =====
    std::cout << "===== 练习4：深拷贝 vs 浅拷贝 =====" << std::endl;
    {
        DynArray original(3);
        DynArray copied(original); // 深拷贝：copied 拥有独立的内存副本
        std::cout << "深拷贝完成，两个对象各自持有独立数据" << std::endl;
        // 如果是浅拷贝（只复制指针），两个对象析构时会 double free
    }
    std::cout << std::endl;

    // ===== 练习5：MyString 类 =====
    std::cout << "===== 练习5：MyString =====" << std::endl;
    {
        MyString s1("Hello");
        std::cout << "s1 = " << s1.c_str() << std::endl;

        MyString s2(s1); // 深拷贝
        std::cout << "s2 = " << s2.c_str() << std::endl;

        MyString s3("World");
        s3 = s1; // 拷贝赋值
        std::cout << "s3 = " << s3.c_str() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "所有练习完成！" << std::endl;
    return 0;
}