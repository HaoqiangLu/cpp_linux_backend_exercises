#include "EpollChatServer.h"
#include "NetConstants.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_set>
#include <array>
#include <string>

void epollChatServer() {
    // 维护客户端列表，收到消息后转发给其他所有客户端
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

    int epoll_fd = ::epoll_create1(0);
    if (epoll_fd < 0) {
        std::cerr << "Error creating epoll instance" << std::endl;
        return;
    }

    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
    if (ret < 0) {
        std::cerr << "Error adding socket to epoll" << std::endl;
        return;
    }

    std::unordered_set<int> clients;
    std::array<struct epoll_event, MAX_EVENTS> events{};
    while (true) {
        int n = ::epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
        if (n < 0) {
            std::cerr << "Error in epoll_wait" << std::endl;
            return;
        }

        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == server_fd) {
                int client_fd = ::accept(events[i].data.fd, nullptr, nullptr);
                if (client_fd < 0) {
                    std::cerr << "Error accepting connection" << std::endl;
                    continue;
                }
                clients.insert(client_fd);
                ev.data.fd = client_fd;
                ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                if (ret < 0) {
                    std::cerr << "Error adding client socket to epoll" << std::endl;
                    ::close(client_fd);
                    clients.erase(client_fd);
                    continue;
                }
                std::cout << "[Client " << client_fd << "] connected. Total: " << clients.size() << std::endl;
            } else {
                int sender_fd = events[i].data.fd;
                std::array<char, BUF_SIZE> buf{};
                ssize_t len = ::recv(sender_fd, buf.data(), buf.size(), 0);

                if (len <= 0) {
                    // 客户端断开
                    ::epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sender_fd, nullptr);
                    ::close(sender_fd);
                    clients.erase(sender_fd);
                    std::cout << "[Client " << sender_fd << "] disconnected. Total: " << clients.size() << std::endl;
                } else {
                    // 收到消息，广播
                    std::string msg(buf.data(), len);
                    std::cout << "[Client " << sender_fd << "]" << msg;

                    for (int fd : clients) {
                        if (fd != sender_fd) {
                            ::send(fd, buf.data(), len, 0);
                        }
                    }
                }
            }
        }
    }

    ::close(epoll_fd);
    ::close(server_fd);
}
