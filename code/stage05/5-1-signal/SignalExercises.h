#pragma once

// 练习1：用 sigaction 注册 SIGINT/SIGTERM 处理器，实现优雅退出标志位
void installGracefulQuitHandler();

// 练习2：忽略 SIGPIPE，验证向已关闭 socket 写数据不会崩溃
void ignoreSigpipeAndTest();

// 练习3：父进程 fork 多个子进程，用 SIGCHLD + waitpid 循环回收避免僵尸
void reapChildrenWithSigchld();

// 练习4：用 signalfd + epoll 把信号处理纳入事件循环
void signalFdWithEpoll();