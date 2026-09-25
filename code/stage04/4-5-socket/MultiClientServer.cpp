#include "MultiClientServer.h"
#include "NetConstants.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <array>

void multiClientServer() {
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

    while (true) {
        int client_fd = ::accept(server_fd, nullptr, nullptr);
        // 为每个客户端创建新 std::thread 处理
        std::thread client_thread([client_fd]() {
            std::array<char, BUF_SIZE> buf{};
            while (true) {
                ssize_t n = ::recv(client_fd, buf.data(), buf.size(), 0);
                if (n < 0) {
                    std::cerr << "[Client " << client_fd << "] Error receiving data" << std::endl;
                    ::close(client_fd);
                    return;
                }
                if (n == 0) {
                    std::cout << "[Client " << client_fd << "] Client disconnected" << std::endl;
                    ::close(client_fd);
                    return;
                }
                std::cout << "[Client " << client_fd << "] Received: " << std::string(buf.data(), n) << std::endl;
            }
        });
        // 对于这种"服务器永远运行、客户端来了就创建线程"的场景，应该用 detach() 而不是 join()
        // detach() 后线程仍然正常运行，只是主线程不再跟踪它。子线程函数 return 后，系统自动回收资源，不会泄漏。
        client_thread.detach();
    }

    ::close(server_fd);
}