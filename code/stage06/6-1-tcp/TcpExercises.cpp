#include "TcpExercises.h"
#include <arpa/inet.h>
#include <array>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <format>
#include <string>

void startEchoServer() {
    // socket + bind + listen
    // accpet 循环，read 后 write 回去
    // 注意：客户端断开时不要主动 close，观察 CLOSE_WAIT
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::perror("socket");
        return;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = ::bind(server_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr));
    if (ret < 0) {
        std::perror("bind");
        return;
    }

    ret = ::listen(server_fd, SOMAXCONN);
    if (ret < 0) {
        std::perror("listen");
        return;
    }

    while (true) {
        struct sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = ::accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_len);
        if (client_fd < 0) {
            std::perror("accept");
            return;
        }

        // 循环 recv 直到返回 0：对端发来 FIN，本端由内核自动进入 CLOSE_WAIT
        std::array<char, 1024> buf{};
        ssize_t n = 0;
        while ((n = ::recv(client_fd, buf.data(), buf.size() - 1, 0)) > 0) {
            ::write(client_fd, buf.data(), n);
            std::cout << std::format("echo {} bytes (cfd={})", n, client_fd) << std::endl;
        }

        // 关键：对端已关闭，这里故意不 close，连接会一直停在 CLOSE_WAIT
        // 注意：必须保持本进程存活再去执行 ss，进程一退出内核会自动关 fd 补发 FIN
        std::cout << std::format("recv returned {}, peer sent FIN, now in CLOSE_WAIT (cfd={})", n, client_fd) << std::endl;
        std::cout << "请另开终端执行: ss -tan state close-wait" << std::endl;
        std::cout << "观察完毕后按回车 close 并继续 accept..." << std::endl;
        std::cin.get();
        ::close(client_fd);
    }
}

void startStressEchoServer() {
    // 练习3 专用：accept 循环 + 回显 + 立即 close
    // 与 startEchoServer 的区别：每条连接处理完立刻 close，让客户端（主动关闭方）走完四次挥手进入 TIME_WAIT
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::perror("socket");
        return;
    }

    // SO_REUSEADDR：允许 Ctrl+C 后立即重启而不报 Address already in use
    int opt = 1;
    ::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = ::bind(server_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr));
    if (ret < 0) {
        std::perror("bind");
        ::close(server_fd);
        return;
    }

    ret = ::listen(server_fd, SOMAXCONN);
    if (ret < 0) {
        std::perror("listen");
        ::close(server_fd);
        return;
    }

    std::cout << "压测回声服务端已启动 (监听 8080)，Ctrl+C 停止" << std::endl;
    long long conn_count = 0;
    while (true) {
        int client_fd = ::accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            std::perror("accept");
            break;
        }

        std::array<char, 1024> buf{};
        ssize_t n = ::recv(client_fd, buf.data(), buf.size() - 1, 0);
        if (n > 0) {
            ::write(client_fd, buf.data(), n);
        }

        // 关键：回显后立即 close，服务端作为被动关闭方不会进入 TIME_WAIT
        ::close(client_fd);
        std::cout << std::format("\r[{}] handled", ++conn_count) << std::flush;
    }
    std::cout << std::endl;
    ::close(server_fd);
}

void startBrokenClient() {
    int cfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (cfd < 0) {
        std::perror("socket");
        return;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = ::htons(8080);
    ::inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    int ret = ::connect(cfd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr));
    if (ret < 0) {
        std::perror("connect");
        return;
    }

    std::string msg = "Hello!";
    ::send(cfd, msg.c_str(), msg.size(), 0);

    // 先收完回显再退出，避免本端 FIN/RST 与服务端回显交叉的竞态
    std::array<char, 1024> buf{};
    ssize_t n = ::recv(cfd, buf.data(), buf.size() - 1, 0);
    std::cout << std::format("recv echo {} bytes: {}", n, std::string(buf.data(), n > 0 ? n : 0)) << std::endl;

    // 发完直接退出：_exit 不会阻止内核关闭 fd，FIN 照样会发出去
    // 区别在于本端不发 close、不等四次挥手，服务端若不 close 就会停在 CLOSE_WAIT
    _exit(0);
}

void shortConnectionStress(int count) {
    // 循环 count 次，每次 connect + send + recv + close
    // 用 ss -tan state time-wait 观察 TIME_WAIT 堆积
    // 提示用户用 sysctl 调整 net.ipv4.tcp_tw_reuse

    for (int i = 0; i < count; ++i) {
        int cfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (cfd < 0) {
            std::perror("socket");
            continue;
        }

        struct sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = ::htons(8080);
        ::inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

        int ret = ::connect(cfd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr));
        if (ret < 0) {
            std::perror("connect");
            ::close(cfd);
            continue;
        }

        std::string msg = "ping";
        ::send(cfd, msg.c_str(), msg.size(), 0);

        std::array<char, 1024> buf{};
        ssize_t n = ::recv(cfd, buf.data(), buf.size() - 1, 0);

        // 客户端主动 close → 走完整四次挥手 → 本端进入 TIME_WAIT（持续 2MSL）
        ::close(cfd);

        std::cout << std::format("[{}] short conn done, recv {} bytes", i, n) << std::endl;
    }

    std::cout << "观察: ss -tan state time-wait | wc -l" << std::endl;
    std::cout << "缓解: sudo sysctl -w net.ipv4.tcp_tw_reuse=1" << std::endl;
}

void printTcpStats() {
    // system("ss -s") 或 popen 读取输出
    std::system("ss -s");
}
