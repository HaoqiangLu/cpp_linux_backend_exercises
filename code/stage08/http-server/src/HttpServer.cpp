#include "HttpServer.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "HttpHandler.h"
#include "Logger.h"
#include "TcpConnection.h"
#include "ThreadPool.h"
#include "TimerQueue.h"
#include <chrono>
#include <cctype>
#include <csignal>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <unistd.h>

namespace {
HttpServer* g_server = nullptr; // 仅支持单实例（信号处理需要全局入口）

int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void signalHandler(int) {
    // 信号路径只做 async-signal-safe 操作：stop() 内部仅置原子标志 + write(eventfd)
    if (g_server) {
        g_server->stop();
    }
}

// 应用层分帧：判断 buf 中是否已收到一个完整的 HTTP 请求。
// 返回完整请求的字节长度（含 body）；返回 0 表示尚不完整，需继续等待后续数据。
// 规则：① 必须出现头部结束的空行 \r\n\r\n；② 若带 Content-Length，body 必须收全。
size_t completeRequestLen(const std::string& buf) {
    static const std::string kHeaderEnd = "\r\n\r\n";
    size_t pos = buf.find(kHeaderEnd);
    if (pos == std::string::npos) {
        return 0;   // 头部尚未收全（半包）
    }
    size_t headerEnd = pos + kHeaderEnd.size();

    // 逐行扫描头部，大小写不敏感地取 Content-Length
    size_t contentLength = 0;
    const std::string head = buf.substr(0, pos);
    size_t ls = 0;
    while (ls < head.size()) {
        size_t le = head.find("\r\n", ls);
        if (le == std::string::npos) le = head.size();
        std::string line = head.substr(ls, le - ls);
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            for (char& c : key) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            if (key == "content-length") {
                try { contentLength = std::stoul(line.substr(colon + 1)); } catch (...) { contentLength = 0; }
            }
        }
        ls = le + 2;
    }

    if (buf.size() - headerEnd < contentLength) {
        return 0;   // body 尚未收全
    }
    return headerEnd + contentLength;
}
}   // namespace


HttpServer::HttpServer(int port, int subReactorCount, int threadPoolSize) : port_(port) {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
    g_server = this;

    mainLoop_ = std::make_unique<EventLoop>();
    acceptor_ = std::make_unique<Acceptor>(mainLoop_.get(), port);
    acceptor_->setNewConnCallback([this](int fd) { onNewConnection(fd); });
    threadPool_ = std::make_unique<ThreadPool>(threadPoolSize);

    // 创建 subReactorCount 个 sub EventLoop，各自在独立线程中 [构造 + loop()]
    subLoops_.reserve(subReactorCount);
    subThreads_.reserve(subReactorCount);
    for (int i = 0; i < subReactorCount; ++i) {
        std::promise<std::unique_ptr<EventLoop>> prom;
        std::future<std::unique_ptr<EventLoop>> fut = prom.get_future();
        subThreads_.emplace_back([p = std::move(prom)]() mutable {
            // 必须在子线程构造 EventLoop，threadId_ 才会记录为本线程（runInLoop 判定依赖它）
            auto loop = std::make_unique<EventLoop>();
            EventLoop* lp = loop.get();
            p.set_value(std::move(loop));   // 所有权移交主线程存入 subLoops_
            lp->loop(); // 阻塞运行，quit() 后返回
        });
        subLoops_.push_back(fut.get()); // 主线程等待 subLoop 构造完成后再接管
    }
}

HttpServer::~HttpServer() {
    stop(); // 幂等：确保所有 loop 退出
    for (std::thread& t : subThreads_) {
        if (t.joinable()) {
            t.join();
        }
    }
    if (g_server == this) {
        g_server = nullptr;
    }
}

void HttpServer::start() {
    LOG_INFO("HttpServer started on port %d", port_);
    acceptor_->listen();
    mainLoop_->loop();  // 阻塞，直到 signaleHandler -> stop() -> quit()
}

void HttpServer::stop() {
    if (mainLoop_) {
        mainLoop_->quit();
    }
    for (auto& loop : subLoops_) {
        if (loop) {
            loop->quit();
        }
    }
}

void HttpServer::onNewConnection(int fd) {
    if (subLoops_.empty()) {
        ::close(fd);    // 兜底：无 subLoop 时直接关闭
        return;
    }

    int idx = nextSub_.fetch_add(1) % static_cast<int>(subLoops_.size());   // 轮询(round-robin)
    EventLoop* subLoop = subLoops_[idx].get();

    // 在 subLoop 所属线程中创建并注册连接，保证 epoll 操作与后续 IO 同线程
    subLoop->runInLoop([this, subLoop, fd]() {
        auto conn = std::make_shared<TcpConnection>(subLoop, fd);
        conn->setMessageCallback([this](const std::shared_ptr<TcpConnection>& c, const std::string& msg) {
            onRequest(c, msg);
        });
        conn->setCloseCallback([this](const std::shared_ptr<TcpConnection>& c) {
            onClose(c);
        });

        {
            std::lock_guard<std::mutex> lock(connMtx_);
            connections_[fd] = conn;    // 必须先入 map，handleRead 里 shared_from_this 才有效
        }
        LOG_INFO("new connection: fd=%d", fd);

        // 空闲超时：挂载自校验定时器（捕获 weak_ptr + 身份校验），
        // 陈旧定时器对已析构连接自动失效，不会误删复用同一 fd 的新连接
        scheduleIdleClose(subLoop, fd, conn, connTimeoutMs_);
    });
}

void HttpServer::onRequest(const std::shared_ptr<TcpConnection>& conn, const std::string& msg) {
    // 应用层分帧：msg 是 inputBuffer_ 的引用，可能只是半个请求。
    // 若请求尚未完整到达就分发，会被解析成 400，且半包残余未读会导致
    // close() 触发 RST（ab 无 keep-alive 高并发下大量复现，压测因此中止 -> QPS=N/A）。
    size_t len = completeRequestLen(msg);
    if (len == 0) {
        return;   // 保留 inputBuffer_，等后续 EPOLLIN 补齐（ET 模式下新数据会再次触发）
    }

    std::string request = msg.substr(0, len);   // 只取一个完整请求
    conn->retrieveInput(len);                   // 消费掉这一个请求，剩余（如 pipelining）留待下次

    // IO 与计算分离：把 HTTP 解析/响应投递到业务线程池
    threadPool_->addTask([this, conn, request]() {
        HttpHandler handler(docRoot_, staticStrategy_);
        handler.onRequest(conn, request);
    });
}

void HttpServer::onClose(const std::shared_ptr<TcpConnection>& conn) {
    std::lock_guard<std::mutex> lock(connMtx_);
    auto it = connections_.find(conn->fd());
    // 身份校验：仅当该 fd 仍指向本连接时才移除，防止误删复用同一 fd 的新连接
    if (it != connections_.end() && it->second == conn) {
        LOG_INFO("connection closed: fd=%d", conn->fd());
        connections_.erase(it);
    }
}

void HttpServer::scheduleIdleClose(EventLoop* loop, int fd,
                                   std::weak_ptr<TcpConnection> weak, int64_t delayMs) {
    loop->timerQueue()->addTimer(nowMs() + delayMs, [this, loop, fd, weak]() {
        auto conn = weak.lock();
        if (!conn) {
            // 连接已析构（多为正常关闭）：陈旧定时器直接失效，
            // 不会再误删后来复用同一 fd 的新连接——这正是此前崩溃的根因。
            return;
        }

        // 真·空闲语义：若距上次活动未达超时（期间有请求），按剩余时间顺延，
        // 避免误杀活跃 keep-alive 连接
        int64_t idle = nowMs() - conn->lastActiveMs();
        if (idle < connTimeoutMs_) {
            scheduleIdleClose(loop, fd, weak, connTimeoutMs_ - idle);
            return;
        }

        bool closed = false;
        {
            std::lock_guard<std::mutex> lock(connMtx_);
            auto it = connections_.find(fd);
            // 身份校验：仅当该 fd 仍指向本连接时才移除，防止误删复用同一 fd 的新连接
            if (it != connections_.end() && it->second == conn) {
                connections_.erase(it);
                closed = true;
            }
        }
        if (closed) {
            LOG_INFO("connection idle timeout, closed: fd=%d", fd);
        }
        // conn 在 lambda 结束时于本 IO 线程析构（若为最后引用）：
        // ~TcpConnection 完成 removeChannel + close(fd)，与 epoll 同线程，清理安全。
    });
}