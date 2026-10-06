#include "ConnectionPool.h"
#include <chrono>
#include <cstdio>
#include <mutex>
#include <mysql.h>

ConnectionPool::ConnectionPool(const std::string& h, const std::string& u,
                               const std::string& p, const std::string& d,
                               size_t max)
    : host_(h), user_(u), pwd_(p), db_(d), maxConn_(max) {
    for (size_t i = 0; i < maxConn_; ++i) {
        void* conn = createConnection();
        if (conn) {
            pool_.push(conn);
            ++totalCreated_;
        }
    }
}

ConnectionPool::~ConnectionPool() {
    std::lock_guard<std::mutex> lk(mtx_);
    while (!pool_.empty()) {
        destroyConnection(pool_.front());
        pool_.pop();
    }
    totalCreated_ = 0;
}

void* ConnectionPool::createConnection() {
    MYSQL* mysql = mysql_init(nullptr);
    if (mysql == nullptr) {
        return nullptr;
    }

    if (!mysql_real_connect(mysql, host_.c_str(), user_.c_str(), pwd_.c_str(), db_.c_str(), 0, nullptr, 0)) {
        std::fprintf(stderr, "连接失败: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return nullptr;
    }

    return mysql;
}

void ConnectionPool::destroyConnection(void* conn) {
    mysql_close(static_cast<MYSQL*>(conn));
}

ConnectionPool::ConnPtr ConnectionPool::acquire(int timeoutMs) {
    std::unique_lock<std::mutex> lk(mtx_);
    if (!cv_.wait_for(lk, std::chrono::milliseconds(timeoutMs),
        [this]() { return !pool_.empty() || totalCreated_ < maxConn_; })) {
        return nullptr;
    }

    void* conn = nullptr;
    if (!pool_.empty()) {
        conn = pool_.front();
        pool_.pop();
    } else if (totalCreated_ < maxConn_) {
        conn = createConnection();
        if (conn) {
            ++totalCreated_;
        }
    }

    if (!conn) {
        return nullptr;
    }

    return ConnPtr(conn, [this](void* c) {
        std::lock_guard<std::mutex> lk(mtx_);
        pool_.push(c);
        cv_.notify_one();
    });
}

bool ConnectionPool::transferMoney(int fromId, int toId, double amount) {
    auto conn = acquire();
    if (!conn) {
        return false;
    }

    MYSQL* sql = static_cast<MYSQL*>(conn.get());
    if (mysql_query(sql, "START TRANSACTION")) {
        return false;
    }

    char query[256];
    std::snprintf(query, sizeof(query),
        "UPDATE accounts SET balance = balance - %.2f WHERE id = %d",
        amount, fromId);
    if (mysql_query(sql, query)) {
        mysql_query(sql, "ROLLBACK");
        return false;
    }

    std::snprintf(query, sizeof(query),
        "UPDATE accounts SET balance = balance + %.2f WHERE id = %d",
        amount, toId);
    if (mysql_query(sql, query)) {
        mysql_query(sql, "ROLLBACK");
        return false;
    }

    return mysql_query(sql, "COMMIT") == 0;
}
