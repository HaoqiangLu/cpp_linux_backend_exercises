#include "EchoClient.h"
#include "NetConstants.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <array>

void echoClient() {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = ::connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    if (ret < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return;
    }

    std::string message = "Hello, server!";
    ret = ::send(fd, message.data(), message.size(), 0);
    if (ret < 0) {
        std::cerr << "Error sending data" << std::endl;
        return;
    }

    std::array<char, BUF_SIZE> buffer{};
    ret = ::recv(fd, buffer.data(), buffer.size(), 0);
    if (ret < 0) {
        std::cerr << "Error receiving data" << std::endl;
        return;
    }

    std::cout << "Received: " << std::string(buffer.data(), ret) << std::endl;
    ::close(fd);
}