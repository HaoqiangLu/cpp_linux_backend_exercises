#pragma once

#include <string>
#include <vector>
#include <utility>
#include <memory>

// 前置声明
namespace sw { namespace redis { class Redis; } }

// 练习2~4：RAII 封装的 Redis 客户端
class RedisClient {
public:
    RedisClient(const std::string& host = "tcp://127.0.0.1:6379");
    ~RedisClient();

    // 练习2：用户会话
    bool setSession(const std::string& sessionId, const std::string& userId, int ttlSec = 1800);
    std::string getSession(const std::string& sessionId);

    // 练习3：接口限流（INCR + EXPIRE 滑动窗口）
    bool isRateLimited(const std::string& key, int maxCount, int windowSec);

    // 练习4：排行榜(zset)
    bool updateScore(const std::string& key, const std::string& user, double score);
    std::vector<std::pair<std::string, double>> topN(const std::string& key, int n);

private:
    std::unique_ptr<sw::redis::Redis> redis_;
};