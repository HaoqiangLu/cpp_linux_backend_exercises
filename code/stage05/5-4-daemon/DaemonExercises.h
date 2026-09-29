#pragma once

// 练习1：实现守护进程化的心跳程序（每秒写时间戳到日志文件）
void daemonHeartbeat();

// 练习2：实现 PID 文件锁，保证同一时刻只能运行一个实例
bool acquirePidLock(const char* pidFile);
void releasePidLock(const char* pidFile);


// 练习3：实现 master + N workers 的进程池模型
void processPool();

// 练习4：用 prctl(PR_SET_PDEATHSIG) 让子进程在父进程退出时自动退出
void preventOrphan();