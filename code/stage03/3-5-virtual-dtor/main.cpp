#include "Base.h"
#include "Derived.h"

int main() {
    // 练习2：基类指针 new 派生类后 delete
    Base* p = new Derived(1, 2);
    delete p;   // 练习3：观察无虚析构函数时 Derived::~Derived 是否被调用
    /* 执行结果只有Base析构函数调用了
    [Base 构造]
    [Derived 构造]
    [Base 析构]
    */

    // 练习4：将 Base::~Base 改为 virtual 后重新验证
    /* 都被调用
    [Base 构造]
    [Derived 构造]
    [Derived 析构]
    [Base 析构]
    */
}