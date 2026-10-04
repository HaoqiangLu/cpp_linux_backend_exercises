#pragma once

// 练习1：SO_REUSEADDR 避免重启报 "Address already in use"
void reuseAddrServer(int port);

// 练习2：SO_REUSEPORT 实现多进程负载均衡
void reusePortMultiProcess(int port);

// 练习3：TCP_NODELAY 对比小消息场景下的延迟
void tcpNoDelayTest(bool enable);

// 练习4：SO_KEEPALIVE 检测对端异常断开
void keepAliveServer(int port);

// 练习5：SO_LINGER 控制 close 行为
void lingerTest(bool enableLinger, int lingerSec);