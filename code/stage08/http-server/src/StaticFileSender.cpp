#include "StaticFileSender.h"
#include "TcpConnection.h"
#include <cstddef>
#include <memory>
#include <string>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

// 方式一：普通 read/write —— 文件读进用户态缓冲区，再经 TcpConnection 发送
void StaticFileSender::sendWithReadWrite(const std::shared_ptr<TcpConnection>& conn, const std::string& path) {
    int fd = ::open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        perror("open");
        return;
    }

    struct stat st{};
    if (::fstat(fd, &st) < 0) {
        perror("fstat");
        ::close(fd);
        return;
    }

    std::string buf(static_cast<size_t>(st.st_size), '\0');
    ssize_t n = ::read(fd, buf.data(), buf.size());
    if (n < 0) {
        perror("read");
    } else {
        buf.resize(static_cast<size_t>(n)); // 按实际读到的字节数收敛
        conn->send(buf);    // 走 TcpConnection 缓冲，内部 runInLoop 线程安全
    }
    ::close(fd);
}

// 方式二：sendfile 零拷贝 —— 内核直接把文件数据送到 socket，不经过用户态
void StaticFileSender::sendWithSendfile(const std::shared_ptr<TcpConnection>& conn, const std::string& path) {
    int inFd = ::open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (inFd < 0) {
        perror("open");
        return;
    }

    struct stat st{};
    if (::fstat(inFd, &st) < 0) {
        perror("fstat");
        ::close(inFd);
        return;
    }

    off_t offset = 0;
    ssize_t remaining = st.st_size;
    // sendfile 可能部分发送，循环直到发完
    while (remaining > 0) {
        ssize_t sent = ::sendfile(conn->fd(), inFd, &offset, static_cast<size_t>(remaining));
        if (sent > 0) {
            remaining -= sent;
        } else if (sent < 0 && (errno == EINTR)) {
            continue;   // 被信号中断，重试
        } else if (sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            // 非阻塞 socket 缓冲区满。教学简化：短暂让步后重试；
            // 生产应注册 EPOLLOUT，等写就绪再继续，避免忙等。
            continue;
        } else {
            perror("sendfile");
            break;
        }
    }
    ::close(inFd);
}

// 方式三：mmap 内存映射 —— 文件映射到进程地址空间，再拷进发送缓冲
void StaticFileSender::sendWithMmap(const std::shared_ptr<TcpConnection>& conn, const std::string& path) {
    int fd = ::open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        perror("open");
        return;
    }

    struct stat st{};
    if (::fstat(fd, &st) < 0) {
        perror("fstat");
        ::close(fd);
        return;
    }
    if (st.st_size == 0) {  // mmap 长度不能为 0，否则 EINVAL
        ::close(fd);
        return;
    }

    void* p = ::mmap(nullptr, static_cast<size_t>(st.st_size), PROT_READ, MAP_PRIVATE, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        ::close(fd);
        return;
    }

    conn->send(std::string(static_cast<char*>(p), static_cast<size_t>(st.st_size)));
    ::munmap(p, static_cast<size_t>(st.st_size));
    ::close(fd);
}