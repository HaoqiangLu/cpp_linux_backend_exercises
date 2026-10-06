#include "RedisClient.h"
#include <exception>
#include <iterator>
#include <memory>
#include <sw/redis++/redis++.h>
#include <chrono>
#include <iostream>
#include <sw/redis++/redis.h>

RedisClient::RedisClient(const std::string& host) {
    redis_ = std::make_unique<sw::redis::Redis>(host);
}

RedisClient::~RedisClient() = default;

bool RedisClient::setSession(const std::string& sessionId, const std::string& userId, int ttlSec) {
    try {
        redis_->set(sessionId, userId, std::chrono::seconds(ttlSec));
        return true;
    } catch (const std::exception& e) {
        std::cerr << "setSession failed: " << e.what() << std::endl;
        return false;
    }
}

std::string RedisClient::getSession(const std::string& sessionId) {
    try {
        auto val = redis_->get(sessionId);
        return val ? *val : "";
    } catch (const std::exception& e) {
        std::cerr << "getSession failed: " << e.what() << std::endl;
        return "";
    }
}

bool RedisClient::isRateLimited(const std::string& key, int maxCount, int windowSec) {
    try {
        auto cnt = redis_->incr(key);
        if (cnt == 1) {
            redis_->expire(key, std::chrono::seconds(windowSec));
        }
        return cnt > maxCount;
    } catch (const std::exception& e) {
        std::cerr << "isRateLimited failed: " << e.what() << std::endl;
        return false;
    }
}

bool RedisClient::updateScore(const std::string& key, const std::string& user, double score) {
    try {
        redis_->zadd(key, user, score);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "updateScore failed: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::pair<std::string, double>> RedisClient::topN(const std::string& key, int n) {
    try {
        std::vector<std::pair<std::string, double>> result;
        redis_->zrevrange(key, 0, n-1, std::back_inserter(result));
        return result;
    } catch (const std::exception& e) {
        std::cerr << "topN failed: " << e.what() << std::endl;
        return {};
    }
}