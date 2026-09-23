#include "ProcessExercises.h"
#include <cstdlib>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

void forkDemo() {
    pid_t pid = ::fork();
    if (pid < 0) {
        // 错误处理
        std::cerr << "Fork failed!" << std::endl;
    } else if (pid == 0) {
        // 子进程打印自己的 PID 和父进程 PID
        std::cout << "[Child Process] " << "Child PID: " << ::getpid() << ", Parent PID: " << ::getppid() << std::endl;
        ::_Exit(0);
    } else {
        // 父进程打印自己的 PID 和子进程 PID
        std::cout << "[Parent Process] " << "Parent PID: " << ::getpid() << ", Child PID: " << pid << std::endl;
    }
}

void waitDemo() {
    pid_t pid = ::fork();
    if (pid == 0) {
        // 子进程：sleep(2) 后 exit(42)
        ::sleep(2);
        ::_Exit(42);
    } else {
        // wait(&status)，打印子进程退出码
        int status;
        ::wait(&status);
        std::cout << "Child Process exit with code:" << WEXITSTATUS(status) << std::endl;
    }
}

void execDemo() {
    pid_t pid = ::fork();
    if (pid == 0) {
        ::execlp("ls", "ls", "-l", nullptr);
        ::_Exit(1); // execlp 失败时子进程必须退出
    } else {
        ::wait(nullptr);
    }
}

void forkIndependence() {
    int x = 100;
    pid_t pid = ::fork();
    if (pid == 0) {
        // 子进程修改 x，打印
        x = 200;
        std::cout << "[Child Process] x = " << x << std::endl;
        ::_Exit(0);
    } else {
        // 父进程 sleep 后打印 x，验证未被子进程修改
        ::sleep(1);
        std::cout << "[Parent Process] x = " << x << std::endl;
        ::wait(nullptr);
    }
}