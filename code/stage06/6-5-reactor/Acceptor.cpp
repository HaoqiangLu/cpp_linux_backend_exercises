#include "Acceptor.h"
#include "EventLoop.h"
#include "Channel.h"
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

Acceptor::Acceptor(EventLoop* loop, int port) : loop_(loop) {
    // socket(AF_INET, SOCK_STREAM, 0) 创建监听 socket
    listenFd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd_ < 0) {
        std::perror("socket");
        std::exit(1);
    }

    // setsockopt SO_REUSEADDR
    int opt = 1;
    ::setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // bind 到 0.0.0.0:port
    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(listenFd_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::perror("bind");
        ::close(listenFd_);
        std::exit(1);
    }

    // fcntl 设置非阻塞
    int flags = ::fcntl(listenFd_, F_GETFL, 0);
    ::fcntl(listenFd_, F_SETFL, flags | O_NONBLOCK);

    acceptChannel_ = new Channel(loop_, listenFd_);
    acceptChannel_->setReadCallback([this]() { this->handleRead(); });
}

Acceptor::~Acceptor() {
    loop_->removeChannel(acceptChannel_);
    delete acceptChannel_;
    ::close(listenFd_);
}

void Acceptor::listen() {
    ::listen(listenFd_, SOMAXCONN);
    acceptChannel_->enableReading();
}

void Acceptor::handleRead() {
    // ET 模式下循环 accept 直到 EAGAIN
    while (true) {
        struct sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        int connFd = ::accept4(listenFd_, reinterpret_cast<struct sockaddr*>(&clientAddr), &addrLen, SOCK_NONBLOCK);
        if (connFd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            if (errno == EINTR) continue;
            std::perror("accept4");
            break;
        }
        if (newConnCb_) {
            newConnCb_(connFd);
        }
    }
}
