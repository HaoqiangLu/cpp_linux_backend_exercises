#include "ModernExercises.h"
#include "ResourceManager.h"
#include <iostream>

int main() {
    // 练习A：智能指针重写旧代码
    std::cout << "===== smartPtrRewrite =====" << std::endl;
    smartPtrRewrite();

    // 练习B：Lambda + 算法重写循环
    std::cout << "\n===== lambdaRewrite =====" << std::endl;
    lambdaRewrite();

    // 练习C：ResourceManager
    std::cout << "\n===== ResourceManager =====" << std::endl;
    ResourceManager mgr;

    mgr.add("config.json",  "file",    3);
    mgr.add("socket_1",     "network", 1);
    mgr.add("temp_buf",     "buffer",  5);
    mgr.add("log.txt",      "file",    2);
    mgr.add("socket_2",     "network", 4);

    std::cout << "\n[按优先级排序]" << std::endl;
    mgr.printByPriority();

    std::cout << "\n[按类型过滤: file]" << std::endl;
    mgr.filterByType("file");

    std::cout << "\n[按类型过滤: network]" << std::endl;
    mgr.filterByType("network");

    std::cout << "\n[删除 socket_1 后]" << std::endl;
    mgr.remove("socket_1");
    mgr.printByPriority();
}