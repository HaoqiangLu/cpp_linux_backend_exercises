#include "MiniShell.h"
#include "ProcessExercises.h"
#include <iostream>
#include <unistd.h>

int main() {
    // 练习1：fork 创建子进程，分别打印 PID
    std::cout << "========== 练习1：forkDemo ==========" << std::endl;
    forkDemo();

    std::cout.flush() << std::endl;

    // 练习2：父进程 wait 等待子进程结束
    std::cout << "========== 练习2：waitDemo ==========" << std::endl;
    waitDemo();

    std::cout.flush() << std::endl;

    // 练习3：子进程 exec 执行 ls 命令
    std::cout << "========== 练习3：execDemo ==========" << std::endl;
    execDemo();

    std::cout.flush() << std::endl;

    // 练习5：验证 fork 后父子进程变量独立性
    std::cout << "========== 练习5：forkIndependence ==========" << std::endl;
    forkIndependence();

    std::cout.flush() << std::endl;

    ::sleep(5);
    // 练习4：简易 shell（交互式，放在最后）
    std::cout << "========== 练习4：miniShell ==========" << std::endl;
    miniShell();

    return 0;
}