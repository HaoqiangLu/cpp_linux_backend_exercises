#include "DaemonExercises.h"
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <csignal>

static std::atomic<bool> g_quit(false);

static void sigHandler(int /*sig*/, siginfo_t* /*info*/, void* /*context*/) {
    g_quit = true;
}

static void installSignalHandler() {
    struct sigaction sa{};
    sa.sa_sigaction = sigHandler;
    sa.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);
}

int main() {
    std::cout << "=== Daemon 练习测试 ===\n"
              << "1. DaemonHeartbeat (守护进程心跳)\n"
              << "2. PidLock (PID 文件锁)\n"
              << "3. ProcessPool (进程池)\n"
              << "4. PreventOrphan (防孤儿进程)\n"
              << "请选择: ";

    int choice = 0;
    std::cin >> choice;

    switch (choice) {
        case 1:
            daemonHeartbeat();
            break;
        case 2: {
            const char* pidFile = "/tmp/daemon_test.pid";
            if (!acquirePidLock(pidFile)) {
                std::cerr << "获取 PID 锁失败，已有实例在运行\n";
                return 1;
            }
            installSignalHandler();
            std::cout << "PID 锁获取成功，PID=" << getpid() << "\n"
                      << "按 Ctrl+C 退出...\n";
            while (!g_quit) {
                sleep(1);
            }
            // 实际应在信号处理中调用 releasePidLock
            std::cout << "\n收到信号，正在清理...\n";
            releasePidLock(pidFile);
            break;
        }
        case 3:
            processPool();
            break;
        case 4:
            preventOrphan();
            break;
        default:
            std::cerr << "无效选项\n";
            return 1;
    }

}
