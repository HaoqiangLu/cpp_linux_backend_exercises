#include "HeartbeatServer.h"
#include <csignal>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>

static HeartbeatServer* g_server = nullptr;

static void onSignal(int) {
    if (g_server) g_server->gracefulShutdown();
}

int main () {
    std::cout << "请选择测试项\n";
    std::cout << "1. 心跳超时检测服务器（练习1~3）\n";
    std::cout << "2. 半关闭测试（练习4）\n";
    std::cout << "请选择(1/2):" << std::flush;

    std::string choice{};
    std::getline(std::cin, choice);

    switch (std::stoi(choice)) {
        case 1: {
                HeartbeatServer server{8888, 10, 15};
                g_server = &server;

                // 注册信号处理，支持 Ctrl+C 优雅退出
                std::signal(SIGINT, onSignal);
                std::signal(SIGTERM, onSignal);

                // 在独立线程运行服务器（主线程可监听输入或等待）
                std::thread serverThread([&server]() {
                    server.start();
                });

                std::cout << "Press Ctrl+C to stop the server..." << std::endl;
                serverThread.join();

                std::cout << "Server exited." << std::endl;
            }
            break;
        case 2:
            halfCloseDemo();
            break;
        default:
            std::cout << "无效选择" << std::endl;
    }
}