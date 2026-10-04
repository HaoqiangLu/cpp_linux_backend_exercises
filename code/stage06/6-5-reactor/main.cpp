#include "EchoServer.h"
#include <csignal>

EchoServer* g_server = nullptr;

void signalHandler(int) {
    if (g_server) g_server->stop();
}

int main() {
    EchoServer server(8888, 3);

    g_server = &server;
    signal(SIGINT, signalHandler);

    server.start(); // 内部 mainLoop_->loop() 阻塞
    // Ctrl+C → handler → quit() → wakeup → epoll_wait 返回 → while(!quit_) 退出
    // start() 返回 → server 析构 → ~EchoServer() quit 所有 subLoop + join
}
