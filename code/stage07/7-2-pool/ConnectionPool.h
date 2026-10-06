#pragma once
#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

// 练习1：连接池
class ConnectionPool {
public:
    ConnectionPool(const std::string& host, const std::string& user,
                   const std::string& pwd, const std::string& db,
                   size_t maxConn = 10);
    ~ConnectionPool();

    // RAII 包装：返回 shared_ptr，自定义 deleter 自动归还连接
    using ConnPtr = std::shared_ptr<void>;
    ConnPtr acquire(int timeoutMs = 3000);

    // 练习3：事务封装
    bool transferMoney(int fromId, int toId, double amount);

private:
    void* createConnection();
    void destroyConnection(void* conn);

    std::string host_, user_, pwd_, db_;
    size_t maxConn_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<void*> pool_;
    size_t totalCreated_{0};
};