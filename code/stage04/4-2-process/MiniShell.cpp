#include "MiniShell.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

void miniShell() {
    std::string cmd;
    while (true) {
        std::cout << "\nmyshell> ";
        std::getline(std::cin, cmd);

        if (cmd == "exit") break;

        std::istringstream iss(cmd);
        std::string token;
        std::vector<std::string> args; // 用 string 存储每个参数，避免悬垂指针
        while (iss >> token) {
            args.push_back(token);
        }
        if (args.empty()) continue;

        // 构建 char* 数组供 execvp 使用
        std::vector<char*> argv;
        for (auto& s : args) {
            argv.push_back(s.data());
        }
        argv.push_back(nullptr);

        pid_t pid = ::fork();
        if (pid == 0) {
            ::execvp(argv[0], argv.data());
            // execvp 不会返回，若执行到这里说明替换失败
            std::cerr << "Command not found: " << argv[0] << std::endl;
            ::exit(1); // 子进程必须退出，否则会继续执行 while 循环
        } else {
            int status;
            ::wait(&status);
            if (WIFEXITED(status)) {
                ::usleep(100000); // 等待 100ms，给终端时间渲染子进程输出
            }
        }
    }
}