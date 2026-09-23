#pragma once

// 练习1：fork创建子进程，分别打印 PID
void forkDemo();

// 练习2：父进程 wait 等待子进程结束
void waitDemo();

// 练习3：子进程 exec 执行 ls 命令
void execDemo();

// 练习5：验证 fork 后父子进程变量独立性
void forkIndependence();