#include "SignalExercises.h"
#include <iostream>

int main() {
    std::cout << "===== 信号练习 =====\n"
              << "1. 优雅退出 (sigaction 注册 SIGINT/SIGTERM)\n"
              << "2. 忽略 SIGPIPE (socketpair 验证)\n"
              << "3. SIGCHLD 回收子进程 (waitpid 防僵尸)\n"
              << "4. signalfd + epoll (信号纳入事件循环)\n"
              << "请选择: ";
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1: installGracefulQuitHandler(); break;
        case 2: ignoreSigpipeAndTest();       break;
        case 3: reapChildrenWithSigchld();    break;
        case 4: signalFdWithEpoll();          break;
        default: std::cout << "无效选项\n";    break;
    }
    return 0;
}