#include "ConnectionPool.h"
#include <mysql.h>
#include <chrono>
#include <iostream>
#include <print>
#include <string>
#include <thread>
#include <vector>

static void benchmarkNewConnection(const std::string& host, const std::string& user,
                                   const std::string& pwd, const std::string& db,
                                   int threads, int queriesPerThread) {
    auto start = std::chrono::steady_clock::now();
    std::vector<std::thread> workers;
    workers.reserve(threads);
    for (int i = 0; i < threads; ++i) {
        workers.emplace_back([&, i]() {
            for (int j = 0; j < queriesPerThread; ++j) {
                MYSQL* conn = mysql_init(nullptr);
                mysql_real_connect(conn, host.c_str(), user.c_str(), pwd.c_str(), db.c_str(), 0, nullptr, 0);
                mysql_query(conn, "SELECT 1");
                mysql_close(conn);
            }
        });
    }
    for (auto& t : workers) {
        t.join();
    }
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
    std::println("每次新建连接:\t{} 线程 * {} 次 = {} ms\n", threads, queriesPerThread, ms);
}

static void benchmarkPool(ConnectionPool& pool, int threads, int queriesPerThread) {
    auto start = std::chrono::steady_clock::now();
    std::vector<std::thread> workers;
    workers.reserve(threads);
    for (int i = 0; i < threads; ++i) {
        workers.emplace_back([&]() {
            for (int j = 0; j < queriesPerThread; ++j) {
                auto conn = pool.acquire();
                if (conn) {
                    mysql_query(static_cast<MYSQL*>(conn.get()), "SELECT 1");
                }
            }
        });
    }
    for (auto& t : workers) {
        t.join();
    }
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
    std::println("使用连接池:\t{} 线程 * {} 次 = {} ms\n", threads, queriesPerThread, ms);
}

static void testTransferMoney(ConnectionPool& pool) {
    std::println("\n=== 测试 transferMoney ===");

    bool ok = pool.transferMoney(1, 2, 300.0);
    std::println("转账 1->2 (300): {}", ok ? "成功" : "失败");

    auto conn = pool.acquire();
    if (!conn) {
        std::println("获取连接失败");
        return;
    }
    MYSQL* sql = static_cast<MYSQL*>(conn.get());
    mysql_query(sql, "SELECT id, balance FROM accounts ORDER BY id");
    MYSQL_RES* res = mysql_store_result(sql);
    if (res) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            std::println("  id={} balance={}", row[0], row[1]);
        }
        mysql_free_result(res);
    }
}

int main() {
    std::cout << "1. 新建连接&连接池 benchmark\n";
    std::cout << "2. 转账测试\n";
    std::cout << "选择要执行的测试:" << std::flush;

    const std::string host = "127.0.0.1";
    const std::string user = "root";
    const std::string pwd = "password";
    const std::string db = "testdb";
    constexpr int Threads = 100;
    constexpr int QueriesPerThr = 10;

    ConnectionPool pool(host, user, pwd, db, 10);

    std::string choice;
    std::getline(std::cin, choice);
    std::cout << std::endl;
    switch (std::stoi(choice)) {
        case 1: {
            benchmarkNewConnection(host, user, pwd, db, Threads, QueriesPerThr);
            benchmarkPool(pool, Threads, QueriesPerThr);
            break;
        }
        case 2:
            testTransferMoney(pool);
            break;
        default:
            std::println("无效的选择");
    }
}