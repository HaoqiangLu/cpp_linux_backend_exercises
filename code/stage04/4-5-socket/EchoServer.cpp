#include "EchoServer.h"
#include "NetConstants.h"
#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void echoServer() {
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

    std::array<char, BUF_SIZE> buffer{};
    while (true) {
        ssize_t n = ::recv(client_fd, buffer.data(), buffer.size(), 0);
        if (n < 0) {
            std::cerr << "Error reading from socket" << std::endl;
            return;
        } else if (n == 0) {
            std::cerr << "Client disconnected" << std::endl;
            return;
        }
        std::cout << "Received: " << std::string(buffer.data(), n) << std::endl;

        ::send(client_fd, buffer.data(), n, 0);
    }

    ::close(client_fd);
    ::close(server_fd);
}