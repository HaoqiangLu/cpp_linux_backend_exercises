#include "DaemonExercises.h"
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>

void daemonHeartbeat() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid > 0) {
        _exit(0);
    }

    int ret = setsid();
    if (ret < 0) {
        perror("setsid");
        return;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork2");
        return;
    }
    if (pid > 0) {
        _exit(0);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    int devnull = open("/dev/null", O_RDWR);
    if (devnull >= 0) {
        dup2(devnull, STDIN_FILENO);
        dup2(devnull, STDOUT_FILENO);
        dup2(devnull, STDERR_FILENO);
        if (devnull > STDERR_FILENO) {
            close(devnull);
        }
    }

    chdir("/");
    umask(0);

    const char* logPath = "/tmp/heartbeat.log";
    while (true) {
        int fd = open(logPath, O_WRONLY|O_CREAT|O_APPEND, 0644);
        if (fd >= 0) {
            time_t now = time(nullptr);
            char buf[64];
            int len = snprintf(buf, 64, "[heartbeat] %s", ctime(&now));
            write(fd, buf, static_cast<size_t>(len));
            close(fd);
        }
        sleep(1);
    }
}

static int g_pidLockFd = -1;

bool acquirePidLock(const char* pidFile) {
    g_pidLockFd = open(pidFile, O_CREAT|O_RDWR, 0644);
    if (g_pidLockFd < 0) {
        perror("open");
        return false;
    }
    int ret = flock(g_pidLockFd, LOCK_EX|LOCK_NB);
    if (ret < 0) {
        perror("flock");
        close(g_pidLockFd);
        return false;
    }
    ftruncate(g_pidLockFd, 0);
    std::string pid = std::to_string(getpid());
    write(g_pidLockFd, pid.data(), pid.size());
    // 注意：fd 不能 close，否则 flock 锁会释放
    return true;
}

void releasePidLock(const char* pidFile) {
    if (g_pidLockFd < 0) return;
    int ret = flock(g_pidLockFd, LOCK_UN);
    if (ret < 0) {
        perror("flock");
    }
    close(g_pidLockFd);
    g_pidLockFd = -1;
    unlink(pidFile);
}

void processPool() {
    const int N = 4;
    int taskPipe[2];

    int ret = pipe(taskPipe);
    if (ret < 0) {
        perror("pipe");
        return;
    }

    // fork N 个 worker，每个 worker 循环从读端读任务并执行
    // master 从写端写入任务，实现分发
    // master 关闭写端，waitpid 等待所有 worker 退出
    for (int i = 0; i < N; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            close(taskPipe[0]);
            close(taskPipe[1]);
            return;
        } else if (pid == 0) {
            close(taskPipe[1]);
            int task;
            while (read(taskPipe[0], &task, sizeof(task)) > 0) {
                printf("[Worker %d] 处理任务 #%d\n", getpid(), task);
                sleep(1);
                printf("[Worker %d] 任务 #%d 完成\n", getpid(), task);
            }
            close(taskPipe[0]);
            _exit(0);
        }
    }

    close(taskPipe[0]);

    const int NUM_TASKS = 8;
    for (int t = 1; t <= NUM_TASKS; ++t) {
        printf("[Master] 分发任务 #%d\n", t);
        write(taskPipe[1], &t, sizeof(t));
    }

    close(taskPipe[1]);

    for (int i = 0; i < N; ++i) {
        waitpid(-1, nullptr, 0);
    }
    printf("[Master] 所有 worker 已退出\n");
}

void preventOrphan() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        prctl(PR_SET_PDEATHSIG, SIGTERM);
        printf("[Child %d] 运行中，父进程 PID=%d\n", getpid(), getppid());
        while (true) {
            sleep(1);
        }
        _exit(0);
    }

    sleep(5);
    printf("[Parent] 即将退出，子进程 PID=%d 应自动退出...\n", pid);
    return;
}