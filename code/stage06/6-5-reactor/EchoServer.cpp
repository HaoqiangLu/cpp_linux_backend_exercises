#include "EchoServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Channel.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <sys/socket.h>
#include <unistd.h>

EchoServer::EchoServer(int port, int subReactorCount) {
    mainLoop_ = std::make_unique<EventLoop>();

    // 创建 subLoop, 每个独立线程运行
    for (int i = 0; i < subReactorCount; ++i) {
        subLoops_.push_back(std::make_unique<EventLoop>());
    }

    acceptor_ = std::make_unique<Acceptor>(mainLoop_.get(), port);
    acceptor_->setNewConnCallback([this](int fd) { this->onNewConnection(fd); });
}

EchoServer::~EchoServer() {
    std::cout << "EchoServer shutting down..." << std::endl;
    // 清理仍活跃的连接
    for (auto& [fd, conn]: connections_) {
        conn->loop->removeChannel(conn->channel);   // epoll_ctl DEL
        delete conn->channel;
        conn->channel = nullptr;
        ::close(fd);
    }
    // 退出所有 sub-reactor 并 join 线程
    for (auto& loop : subLoops_) {
        loop->quit();
    }
    for (auto& t : threads_) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void EchoServer::start() {
    // 启动 sub-reactor 线程
    for (auto& loop : subLoops_) {
        threads_.emplace_back([&loop]() { loop->loop(); });
    }

    acceptor_->listen();
    std::cout << "EchoServer started on port, " << subLoops_.size() << " sub-reactors" << std::endl;
    mainLoop_->loop();  // main-reactor 在当前线程运行
}

void EchoServer::onNewConnection(int fd) {
    // Round-Robin 轮询分配 sub-reactor
    auto& sub = subLoops_[nextSub_];
    nextSub_ = (nextSub_ + 1) % static_cast<int>(subLoops_.size());

    // 在 sub-reactor 线程中创建 TcpConnection
    sub->runInLoop([this, fd, sub = sub.get()]() {
        auto conn = std::make_unique<TcpConnection>();
        conn->fd = fd;
        conn->loop = sub;
        conn->channel = new Channel(sub, fd);

        auto* rawConn = conn.get();
        conn->channel->setReadCallback([this, rawConn]() {
            handleRead(rawConn);
        });
        conn->channel->setCloseCallback([this, rawConn]() {
            removeConnection(rawConn);
        });

        // 注册到 epoll，监听 EPOLLIN | EPOLLET（边缘触发）
        conn->channel->enableReading();

        {
            std::lock_guard lock(connMtx_);
            connections_[fd] = std::move(conn);
        }
    });
}

void EchoServer::handleRead(TcpConnection* conn) {
    // ET 模式：循环读取直到 EAGAIN
    while (true) {
        ssize_t n = ::recv(conn->fd, conn->buf, sizeof(conn->buf), 0);
        if (n > 0) {
            ::send(conn->fd, conn->buf, static_cast<size_t>(n), 0);
        } else if (n == 0) {
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            if (errno == EINTR) continue;
            break;
        }
    }
}

void EchoServer::removeConnection(TcpConnection* conn) {
    // 在所属 sub-reactor 线程中安全移除
    conn->loop->runInLoop([this, fd = conn->fd]() {
        {
            std::lock_guard lock(connMtx_);
            auto it = connections_.find(fd);
            if (it != connections_.end()) {
                it->second->loop->removeChannel(it->second->channel);
                delete it->second->channel;
                ::close(it->second->fd);
                connections_.erase(it);
            }
        }
    });
}

void EchoServer::stop() {
    mainLoop_->quit();
}
