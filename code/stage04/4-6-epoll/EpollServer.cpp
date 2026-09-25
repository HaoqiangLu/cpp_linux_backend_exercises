#include "EpollServer.h"
#include "NetConstants.h"
#include <arpa/inet.h>
#include <array>
#include <cerrno>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

void epollServerLT() {
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

    std::array<struct epoll_event, MAX_EVENTS> events{};
    while (true) {
        int n = ::epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
        if (n < 0) {
            std::cerr << "Error in epoll_wait" << std::endl;
            return;
        }

        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == server_fd) {
                // accept 新连接，epoll_ctl 添加 client_fd
                int client_fd = ::accept(events[i].data.fd, nullptr, nullptr);
                if (client_fd < 0) {
                    std::cerr << "Error accepting connection" << std::endl;
                    return;
                }
                ev.data.fd = client_fd;
                ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                if (ret < 0) {
                    std::cerr << "Error adding client socket to epoll" << std::endl;
                    return;
                }
            } else {
                // recv 数据，转发或回声
                std::array<char, BUF_SIZE> buf{};
                ssize_t len = ::recv(events[i].data.fd, buf.data(), buf.size(), 0);
                if (len <= 0) {
                    ::close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                } else {
                    std::cout << "Received: " << std::string(buf.data(), len) << std::endl;

                    std::string response = "Echo(resp): " + std::string(buf.data(), len);
                    ::send(events[i].data.fd, response.data(), response.size(), 0);
                }
            }
        }
    }

    ::close(epoll_fd);
    ::close(server_fd);
}

void epollServerET() {
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
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = server_fd;

    ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
    if (ret < 0) {
        std::cerr << "Error adding socket to epoll" << std::endl;
        return;
    }

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
                    return;
                }
                // 把客户端 socket 设为非阻塞（ET 模式的前提）
                int flags = ::fcntl(client_fd, F_GETFL, 0);
                fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

                ev.data.fd = client_fd;
                ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                if (ret < 0) {
                    std::cerr << "Error adding client socket to epoll" << std::endl;
                    return;
                }
            } else {
                std::array<char, BUF_SIZE> buf{};
                for (;;) {
                    ssize_t len = ::recv(events[i].data.fd, buf.data(), buf.size(), 0);
                    if (len > 0) {
                        std::cout << "Received: " << std::string(buf.data(), len) << std::endl;
                        std::string response = "Echo(resp): " + std::string(buf.data(), len);
                        ::send(events[i].data.fd, response.data(), response.size(), 0);
                    } else if (len == 0) {  // 对方主动调了 close，发了 FIN
                        ::close(events[i].data.fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                        break;
                    } else {
                        // 没有数据可读（非阻塞模式下），连接正常，只是数据读完了
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        // 真正的错误，连接异常
                        ::close(events[i].data.fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                        break;
                    }
                }
            }
        }
    }

    ::close(epoll_fd);
    ::close(server_fd);
}

void nonBlockingEpollServer() {
    // 所有 fd 设置 O_NONBLOCK
    // ET 模式下循环 read/write 直到 EAGAIN
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // server_fd 设置为非阻塞
    int flags = ::fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

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
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = server_fd;

    ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
    if (ret < 0) {
        std::cerr << "Error adding socket to epoll" << std::endl;
        return;
    }

    std::array<struct epoll_event, MAX_EVENTS> events{};
    while (true) {
        int n = ::epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
        if (n < 0) {
            std::cerr << "Error in epoll_wait" << std::endl;
            return;
        }

        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == server_fd) {
                for (;;) {
                    int client_fd = ::accept(events[i].data.fd, nullptr, nullptr);
                    if (client_fd < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        std::cerr << "Error accepting connection" << std::endl;
                        break;
                    }

                    int client_flags = ::fcntl(client_fd, F_GETFL, 0);
                    ::fcntl(client_fd, F_SETFL, client_flags | O_NONBLOCK);

                    ev.data.fd = client_fd;
                    ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                    if (ret < 0) {
                        std::cerr << "Error adding client socket to epoll" << std::endl;
                        ::close(client_fd);
                    }
                }
            } else {
                std::array<char, BUF_SIZE> buf{};
                for (;;) {
                    ssize_t len = ::recv(events[i].data.fd, buf.data(), buf.size(), 0);
                    if (len > 0) {
                        std::cout << "Received: " << std::string(buf.data(), len) << std::endl;
                        std::string response = "Echo(resp): " + std::string(buf.data(), len);
                        ::send(events[i].data.fd, response.data(), response.size(), 0);
                    } else if (len == 0) {
                        ::close(events[i].data.fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                        break;
                    } else {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        // 真正错误
                        ::close(events[i].data.fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                        break;
                    }
                }
            }
        }
    }

    ::close(epoll_fd);
    ::close(server_fd);
}