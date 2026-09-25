#include "CmdServer.h"
#include "NetConstants.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <array>
#include <unistd.h>

void cmdServer() {
    // recv 命令字符串，popen 执行，读取结果 send 回客户端
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
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

    std::array<char, BUF_SIZE> buf{};
    ssize_t n = ::recv(client_fd, buf.data(), buf.size(), 0);
    if (n < 0) {
        std::cerr << "Error receiving data" << std::endl;
        return;
    }
    std::cout << "Received: " << std::string(buf.data(), n) << std::endl;

    std::string cmd(buf.data(), n);
    FILE* fp = ::popen(cmd.data(), "r");
    if (fp == nullptr) {
        std::cerr << "Error executing command" << std::endl;
        return;
    }

    while (::fgets(buf.data(), buf.size(), fp) != nullptr) {
        size_t len = std::strlen(buf.data());
        n = ::send(client_fd, buf.data(), len, 0);
        if (n < 0) {
            std::cerr << "Error sending data" << std::endl;
            return;
        }
    }

    ::pclose(fp);
    ::close(client_fd);
    ::close(server_fd);
}