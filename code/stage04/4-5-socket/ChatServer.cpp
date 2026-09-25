#include "ChatServer.h"
#include "NetConstants.h"
#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

void chatServer() {
    // accept 后循环 recv 并打印，同时接收用户输入 send 给客户端
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = ::bind(server_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    if (ret < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return;
    }

    ret = ::listen(server_fd, SOMAXCONN);
    if (ret < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        return;
    }

    int client_fd = ::accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        return;
    }

    std::thread client_thread([&client_fd]() {
        std::array<char, BUF_SIZE> buf{};
        while (true) {
            ssize_t n = ::recv(client_fd, buf.data(), buf.size(), 0);
            if (n < 0) {
                std::cerr << "Error receiving data" << std::endl;
                return;
            }
            if (n == 0) {
                std::cout << "Client disconnected" << std::endl;
                return;
            }
            std::cout << "Received: " << std::string(buf.data(), n) << std::endl;
        }
    });

    std::string line;
    while (std::getline(std::cin, line)) {
        ::send(client_fd, line.data(), line.size(), 0);
    }

    client_thread.join();   // join(): 主线程在这里等着，直到子线程结束，然后主线程继续往下走

    ::close(client_fd);
    ::close(server_fd);
}

void chatClient() {
    // 同时 recv 显示消息 + 发送用户输入
    int client_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = ::connect(client_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    if (ret < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return;
    }

    std::thread client_thread([&client_fd]() {
        std::array<char, BUF_SIZE> buf{};
        while (true) {
            ssize_t n = ::recv(client_fd, buf.data(), buf.size(), 0);
            if (n < 0) {
                std::cerr << "Error receiving data" << std::endl;
                return;
            }
            if (n == 0) {
                std::cout << "Server disconnected" << std::endl;
                return;
            }
            std::cout << "Received: " << std::string(buf.data(), n) << std::endl;
        }
    });

    std::string line;
    while (std::getline(std::cin, line)) {
        ::send(client_fd, line.data(), line.size(), 0);
    }

    client_thread.join();

    ::close(client_fd);
}