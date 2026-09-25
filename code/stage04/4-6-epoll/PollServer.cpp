#include "PollServer.h"
#include "NetConstants.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <vector>
#include <unistd.h>
#include <array>

void pollServer() {
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

    std::vector<struct pollfd> fds;
    fds.reserve(1024);
    fds.push_back({server_fd, POLLIN, 0});

    while (true) {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret < 0) {
            std::cerr << "Error in poll" << std::endl;
            return;
        }

        for (int i = fds.size() - 1; i >= 0; --i) {
            if (!(fds[i].revents & POLLIN)) continue;

            if (fds[i].fd == server_fd) {
                int client_fd = ::accept(server_fd, nullptr, nullptr);
                fds.push_back({client_fd, POLLIN, 0});
            } else {
                std::array<char, BUF_SIZE> buf{};
                ssize_t n = ::recv(fds[i].fd, buf.data(), buf.size(), 0);
                if (n <= 0) {
                    ::close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                } else {
                    std::cout << "Received: " << std::string(buf.data(), n) << std::endl;
                }
            }
        }
    }
}