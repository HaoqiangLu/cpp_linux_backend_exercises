#include "SockOptExercises.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <chrono>
#include <cstdlib>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sched.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <print>
#include <array>



void reuseAddrServer(int port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = ::bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    if (ret == -1) {
        std::perror("bind");
        ::close(fd);
        return;
    }

    ret = ::listen(fd, SOMAXCONN);
    if (ret == -1) {
        std::perror("listen");
        ::close(fd);
        return;
    }

    ::accept(fd, nullptr, nullptr);
}

// =====================================================

// 收到 SIGINT/SIGTERM 时置位，父子进程共用（fork 会复制这份变量）
volatile std::sig_atomic_t g_stop = 0;

void stopHandler(int) { g_stop = 1; }

// 子进程主体：独立 socket + SO_REUSEPORT + bind + listen + accept 循环
void workerAcceptLoop(int port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::perror("socket");
        return;
    }

    int opt = 1;
    if (::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        std::perror("setsockopt");
        ::close(fd);
        return;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1) {
        std::perror("bind");
        ::close(fd);
        return;
    }

    if (::listen(fd, SOMAXCONN) == -1) {
        std::perror("listen");
        ::close(fd);
        return;
    }

    std::println("[worker {}] listening on port {}", ::getpid(), port);

    int count = 0;
    while (!g_stop) {
        int conn = ::accept(fd, nullptr, nullptr);
        if (conn == -1) {
            if (errno == EINTR) continue;  // 被信号打断，回去检查 g_stop
            std::perror("accept");
            break;
        }
        ++count;
        std::println("[worker {}] connection #{}", ::getpid(), count);
        ::close(conn);  // 只演示内核分流，不做业务
    }

    ::close(fd);
}

void reusePortMultiProcess(int port) {
    // 不设 SA_RESTART：信号到来时 accept 返回 EINTR、pause 被唤醒，循环才能检查 g_stop
    struct sigaction sa;
    sa.sa_handler = stopHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    ::sigaction(SIGINT, &sa, nullptr);
    ::sigaction(SIGTERM, &sa, nullptr);

    constexpr int kWorkers = 4;
    for (int i = 0; i < kWorkers; ++i) {
        pid_t pid = ::fork();
        if (pid == -1) {
            std::perror("fork");
            break;
        }
        if (pid == 0) {
            workerAcceptLoop(port);
            std::fflush(stdout);  // _exit 不冲刷缓冲区，先手动 flush 保留输出
            _exit(0);  // 子进程绝不回到 for 循环；_exit 避免重复冲刷继承的缓冲区
        }
    }

    std::println("[parent {}] {} workers forked, press Ctrl+C to quit",
                 ::getpid(), kWorkers);

    ::pause();  // 父进程只等信号

    // 通知整个进程组退出，再【阻塞】回收每个子进程，避免僵尸
    ::kill(0, SIGTERM);
    for (int i = 0; i < kWorkers; ++i) {
        ::waitpid(-1, nullptr, 0);
    }
    std::println("[parent] all workers exited");
}

// =====================================================

void runEchoServer(int port) {
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::perror("socket");
        std::_Exit(1);
    }

    int opt = 1;
    ::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int ret = ::bind(server_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    if (ret == -1) {
        std::perror("bind");
        ::close(server_fd);
        std::_Exit(1);
    }

    ret = ::listen(server_fd, SOMAXCONN);
    if (ret == -1) {
        std::perror("listen");
        ::close(server_fd);
        std::_Exit(1);
    }

    int client_fd = ::accept(server_fd, nullptr, nullptr);
    if (client_fd == -1) {
        std::perror("accept");
        ::close(server_fd);
        std::_Exit(1);
    }

    // 回显循环：读到1字节就写回1字节，知道对端关闭
    char buf;
    while (::read(client_fd, &buf, 1) == 1) {
        ::write(client_fd, &buf, 1);
    }

    ::close(client_fd);
    ::close(server_fd);
}

void tcpNoDelayTest(bool enable) {
    constexpr int kPort = 19899;
    constexpr int kIteration = 200;

    pid_t pid = ::fork();
    if (pid == -1) {
        std::perror("fork");
        return;
    }

    if (pid == 0) {
        // 子进程
        runEchoServer(kPort);
        std::_Exit(0);
    }

    ::usleep(100000);

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::perror("client socket");
        ::kill(pid, SIGTERM);
        return;
    }

    int flag = enable ? 1 : 0;
    int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    if (ret == -1) {
        std::perror("setsockopt TCP_NODELAY");
    }

    struct sockaddr_in srvAddr{};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(kPort);
    srvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    ret = ::connect(fd, reinterpret_cast<struct sockaddr*>(&srvAddr), sizeof(srvAddr));
    if (ret == -1) {
        std::perror("connect");
        ::close(fd);
        ::kill(pid, SIGTERM);
        ::waitpid(pid, nullptr, 0);
        return;
    }

    char buf = 'x';
    ::write(fd, &buf, 1);
    ::read(fd, &buf, 1);

    auto t0 = std::chrono::steady_clock::now();

    for (int i = 0; i < kIteration; ++i) {
        ::write(fd, &buf, 1);
        ::read(fd, &buf, 1);
    }

    auto t1 = std::chrono::steady_clock::now();
    double totalMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
    double avgUs = (totalMs * 1000.0) / kIteration;

    std::println("[TCP_NODELAY={}] {} iterations | total {:.3f} ms | avg {:.2f} us/pkt",
                enable ? 1 : 0, kIteration, totalMs, avgUs);

    ::close(fd);
    ::kill(pid, SIGTERM);
    ::waitpid(pid, nullptr, 0);
}

// =====================================================

void keepAliveServer(int port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = ::bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    if (ret == -1) {
        std::perror("bind");
        ::close(fd);
        return;
    }

    ret = ::listen(fd, SOMAXCONN);
    if (ret == -1) {
        std::perror("listen");
        ::close(fd);
        return;
    }

    std::println("listening on port {} ...", port);

    struct sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int connfd = ::accept(fd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientLen);
    if (connfd == -1) {
        std::perror("accept");
        ::close(fd);
        return;
    }

    // 在已连接 socket 上设置 keepalive 参数
    int kaEnable = 1, kaIdle = 5, kaIntvl = 2, kaCnt = 3;
    ::setsockopt(connfd, SOL_SOCKET, SO_KEEPALIVE, &kaEnable, sizeof(kaEnable));
    ::setsockopt(connfd, IPPROTO_TCP, TCP_KEEPIDLE, &kaIdle, sizeof(kaIdle));
    ::setsockopt(connfd, IPPROTO_TCP, TCP_KEEPINTVL, &kaIntvl, sizeof(kaIntvl));
    ::setsockopt(connfd, IPPROTO_TCP, TCP_KEEPCNT, &kaCnt, sizeof(kaCnt));

    std::println("client connected, keepalive: idle = {}s intvl={}s probes={}", kaIdle, kaIntvl, kaCnt);

    std::array<char, 256> buf;

    while (true) {
        ssize_t n = ::read(connfd, buf.data(), buf.size());
        if (n == 0) {
            std::println("client disconnected (normal close)");
            break;
        }
        if (n ==-1) {
            std::perror("read (keepalive detected peer death)");
            break;
        }
        std::println("receive {} bytes", n);
    }

    ::close(connfd);
    ::close(fd);
}

// =====================================================

void lingerTest(bool enableLinger, int lingerSec) {
    constexpr int kPort = 19899;

    pid_t pid = ::fork();
    if (pid == -1) {
        std::perror("fork");
        return;
    }

    if (pid == 0) {
        // 子进程：监听并接受一个连接，回显数据
        int serverFd = ::socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        ::setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(kPort);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        int ret = ::bind(serverFd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
        if (ret == -1) {
            std::perror("bind");
            ::close(serverFd);
            std::_Exit(1);
        }

        ret = ::listen(serverFd, SOMAXCONN);
        if (ret == -1) {
            std::perror("listen");
            ::close(serverFd);
            std::_Exit(1);
        }

        int connFd = ::accept(serverFd, nullptr, nullptr);
        if (connFd == -1) {
            std::perror("accept");
            ::close(serverFd);
            std::_Exit(1);
        }

        std::array<char, 256> buf;
        ssize_t n;
        while ((n = ::read(connFd, buf.data(), buf.size())) > 0) {
            ::write(connFd, buf.data(), n);
        }

        ::close(connFd);
        ::close(serverFd);
        std::_Exit(0);
    }

    ::usleep(100000);

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::perror("client socket");
        ::kill(pid, SIGTERM);
        return;
    }

    struct linger lg{};
    lg.l_onoff = enableLinger ? 1 : 0;
    lg.l_linger = lingerSec;
    ::setsockopt(fd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));

    struct sockaddr_in srvAddr{};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(kPort);
    srvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int ret = ::connect(fd, reinterpret_cast<struct sockaddr*>(&srvAddr), sizeof(srvAddr));
    if (ret == -1) {
        std::perror("connect");
        ::close(fd);
        ::kill(pid, SIGTERM);
        ::waitpid(pid, nullptr, 0);
        return;
    }

    std::string msg = "hello";
    ::write(fd, msg.data(), msg.size());

    std::array<char, 256> buf;
    ::read(fd, buf.data(), buf.size());

    std::println("SO_LINGER: l_onoff={}, l_linger={}", lg.l_onoff, lg.l_linger);
    std::println("closing socket... use tcpdump to observe FIN vs RST");
    std::println("  tcpdump -i lo -nn 'tcp port {}'", kPort);

    ::close(fd);

    ::kill(pid, SIGTERM);
    ::waitpid(pid, nullptr, 0);
}