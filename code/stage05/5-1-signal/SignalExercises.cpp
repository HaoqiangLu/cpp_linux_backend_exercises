#include "SignalExercises.h"
#include <array>
#include <atomic>
#include <csignal>
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <cerrno>

static std::atomic<bool> g_quit{false};

static void sigHandler(int sig, siginfo_t* info, void* /*ucontext*/) {
    g_quit = true;
    char buf[128] = {0};
    int len = snprintf(buf, sizeof(buf),
        "Caught signal %d from PID %d\n", sig, info->si_pid);
    write(STDERR_FILENO, buf, len);
}

void installGracefulQuitHandler() {
    // 用 sigaction 注册 SIGINT / SIGTERM 到 sigHandler
    // 主循环 while (!g_quit) { sleep(1); }
    struct sigaction sa;                            // 信号动作结构体
    memset(&sa, 0, sizeof(sa));             // 清零

    sa.sa_sigaction = sigHandler;                   // 设置处理函数
    sa.sa_flags = SA_SIGINFO;   // 不加 SA_SIGINFO，info 参数不会被填充，读到的就是垃圾值（-629628864）
    sigemptyset(&sa.sa_mask);                   // 处理信号时不阻塞其他信号

    sigaction(SIGINT, &sa, nullptr);    // 注册 Ctrl+C
    sigaction(SIGTERM, &sa, nullptr);   // 注册 kill 命令

    while (!g_quit) {
        sleep(1);
    }
}

// ==============================================================

void ignoreSigpipeAndTest() {
    // signal(SIGPIPE, SIG_IGN) 或 sigaction 忽略
    // 创建 socketpair，关闭一端，向另一端 write，验证进程未退出
    signal(SIGPIPE, SIG_IGN);

    std::array<int, 2> fds{};
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, fds.data());
    if (ret < 0) {
        perror("socketpair");
        return;
    }

    close(fds[1]);

    std::string msg = "test";
    ssize_t len = write(fds[0], msg.data(), msg.size());
    if (len < 0) {
        std::cout << "write failed as expected, errno: " << errno << " (" << strerror(errno) << ")\n";
        std::cout << "Process survived! SIGPIPE was ignored.\n";
    }

    close(fds[0]);
}

// ==============================================================

static void sigchldHandler(int /*sig*/, siginfo_t* /* info */, void* /* ucontext */) {
    // 循环回收所有退出子进程
    while (true) {
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if (pid <= 0) {
            break;
        }
        char buf[128] = {0};
        int len = snprintf(buf, sizeof(buf),
            "Reaped child %d with status %d\n", pid, status);
        write(STDERR_FILENO, buf, len);
    }
}

void reapChildrenWithSigchld() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_sigaction = sigchldHandler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGCHLD, &sa, nullptr);

    for (int i = 0; i < 5; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            std::cout << "Child " << getpid() << " exiting\n";
            sleep(2);
            _exit(0);
        }
    }
    // 子进程退出 → 内核自动发送 SIGCHLD 给父进程
    // 每个子进程退出，内核都会发送一次 SIGCHLD
    // 如果多个子进程几乎同时退出，信号可能被合并

    // 父进程 sleep 足够长时间，让用户有时间用 ps 验证
    std::cout << "Parent sleeping for 30 seconds...\n";
    std::cout << "Use 'ps aux | grep Z' in another terminal to verify no zombies\n";
    // 持续等待，确保所有子进程都被回收
    int waitCount = 0;
    while (waitCount < 5) {
        sleep(1);
        waitCount++;
        if (waitCount >= 10) break;  // 最多等 10 秒
    }

    std::cout << "All done.\n";
}

// ==============================================================

void signalFdWithEpoll() {
    // sigprocmask 阻塞 SIGINT / SIGTERM
    // signalfd(-1, &mask, SFD_NONBLOCK) 创建 signalfd
    // 创建 epoll fd，把 signalfd 加入监听 EPOLLIN
    // epoll_wait 循环，读到 signalfd 事件时解析 signalfd_siginfo 并退出

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    // 阻塞这两个信号，防止默认处理
    sigprocmask(SIG_BLOCK, &mask, nullptr);

    int sfd = signalfd(-1, &mask, SFD_NONBLOCK);

    int epfd = epoll_create1(0);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev);

    std::cout << "Waiting for signals via epoll...\n";

    std::array<struct epoll_event, 1> events;
    while (true) {
        int n = epoll_wait(epfd, events.data(), 1, -1);
        if (n <= 0) break;

        if (events[0].data.fd == sfd) {
            // signalfd 有数据可读 → 收到信号了
            struct signalfd_siginfo si;
            read(sfd, &si, sizeof(si)); // 读取信号信息
            std::cout << "Received signal " << si.ssi_signo
                    << " from PID " << si.ssi_pid << std::endl;
            if (si.ssi_signo == SIGINT || si.ssi_signo == SIGTERM) break;
        }
    }

    close(sfd);
    close(epfd);
}