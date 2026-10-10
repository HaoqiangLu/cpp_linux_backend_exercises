#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Acceptor::Acceptor(EventLoop* loop, int port) : loop_(loop) {
    listenFd_ = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (listenFd_ < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (::setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR");
        close(listenFd_);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(listenFd_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("bind");
        close(listenFd_);
        exit(EXIT_FAILURE);
    }

    acceptChannel_ = new Channel(loop_, listenFd_);
    acceptChannel_->setReadCallback([this]() { handleRead(); });
}

Acceptor::~Acceptor() {
    loop_->removeChannel(acceptChannel_);
    delete acceptChannel_;
    close(listenFd_);
}

void Acceptor::listen() {
    if (::listen(listenFd_, SOMAXCONN) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    acceptChannel_->enableReading();
}

void Acceptor::handleRead() {
    while (true) {
        struct sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        int connFd = ::accept4(listenFd_, reinterpret_cast<struct sockaddr*>(&clientAddr), &addrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
        if (connFd < 0) {
            if (errno == EAGAIN) break;
            if (errno == EINTR) continue;
            perror("accept4");
            break;
        }
        if (newConnCb_) {
            newConnCb_(connFd);
        }
    }
}