#include "SelectServer.h"
#include "NetConstants.h"
#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

void selectServer() {
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

    fd_set readfds{};
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    int max_fd = server_fd;
    while (true) {
        fd_set readfds_copy = readfds;

        int ret = ::select(max_fd + 1, &readfds_copy, nullptr, nullptr, nullptr);
        if (ret < 0) {
            std::cerr << "Error in select" << std::endl;
            return;
        }

        for (int fd = 0; fd <= max_fd; ++fd) {
            if (!FD_ISSET(fd, &readfds_copy)) continue;

            if (fd == server_fd) {
                int client_fd = ::accept(server_fd, nullptr, nullptr);
                FD_SET(client_fd, &readfds);
                if (client_fd > max_fd) {
                    max_fd = client_fd;
                }
            } else {
                std::array<char, BUF_SIZE> buf{};
                ssize_t n = ::recv(fd, buf.data(), buf.size(), 0);
                if (n <= 0) {
                    ::close(fd);
                    FD_CLR(fd, &readfds);
                } else {
                    std::cout << "Received: " << std::string(buf.data(), n) << std::endl;
                }
            }
        }
    }
}