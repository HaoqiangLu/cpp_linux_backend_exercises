#pragma once

// 练习2：写一个客户端中途 kill 的场景，观察服务端的 CLOSE_WAIT 堆积
void startEchoServer();     // 服务端（故意不 close），用于练习2 观察 CLOSE_WAIT
void startBrokenClient();   // 客户端，故意不发 FIN
void startStressEchoServer(); // 练习3 专用：echo 后立即 close 的循环服务端

// 练习3：短连接压测脚本，观察 TIME_WAIT 堆积
void shortConnectionStress(int count);

// 练习4：用 ss -s 查看系统级 TCP 统计
void printTcpStats();