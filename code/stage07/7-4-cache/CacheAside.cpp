#include "CacheAside.h"
#include <memory>
#include <ostream>
#include <sw/redis++/redis++.h>
#include <mutex>
#include <sw/redis++/redis.h>
#include <thread>
#include <chrono>
#include <iostream>

namespace {
constexpr const char* NULL_SENTINEL = "NULL";
constexpr int NULL_TTL_SEC = 5;
}

// 练习1：Cache Aside 模式
CacheAside::CacheAside(DbQuery q)
    : redis_(std::make_shared<sw::redis::Redis>("tcp://127.0.0.1:6379")),
      dbQuery_(std::move(q)) {
}

void CacheAside::setDbUpdater(DbUpdater updater) {
    dbUpdater_ = std::move(updater);
}

std::string CacheAside::get(const std::string& key) {
    auto val = redis_->get(key);
    if (val) {
        std::cout << "[CacheAside] HIT: " << key << std::endl;
        return *val;
    }

    std::cout << "[CacheAside] MISS: " << key << " -> query DB" << std::endl;
    std::string value = dbQuery_(key);
    if (!value.empty()) {
        redis_->set(key, value, std::chrono::seconds(60));
    }
    return value;
}

void CacheAside::set(const std::string& key, const std::string& value, int ttlSec) {
    redis_->del(key);
    std::cout << "[CacheAside] set: deleted cache for " << key << std::endl;
    if (dbUpdater_) {
        dbUpdater_(key, value);
    }
    redis_->set(key, value, std::chrono::seconds(ttlSec));
    std::cout << "[CacheAside] set: updated cache for " << key << std::endl;
}

// 练习2：缓存穿透防护
AntiPenetrationCache::AntiPenetrationCache(DbQuery q)
    : redis_(std::make_shared<sw::redis::Redis>("tcp://127.0.0.1:6379")),
      dbQuery_(std::move(q)) {
}

std::string AntiPenetrationCache::get(const std::string& key) {
    auto val = redis_->get(key);
    if (val) {
        if (*val == NULL_SENTINEL) {
            std::cout << "[AntiPenetration] HIT sentinel: " << key << " -> return empty" << std::endl;
            return "";
        }
        std::cout << "[AntiPenetration] HIT: " << key << std::endl;
        return *val;
    }

    std::cout << "[AntiPenetration] MISS: " << key << " -> query DB" << std::endl;
    std::string value = dbQuery_(key);
    if (!value.empty()) {
        redis_->set(key, value, std::chrono::seconds(60));
    } else {
        redis_->set(key, NULL_SENTINEL, std::chrono::seconds(NULL_TTL_SEC));
        std::cout << "[AntiPenetration] cached NULL sentinel for " << key << " (TTL=" << NULL_TTL_SEC << "s)" << std::endl;
    }
    return value;
}

// 练习3：缓存击穿防护
AntiBreakdownCache::AntiBreakdownCache(DbQuery q)
    : redis_(std::make_shared<sw::redis::Redis>("tcp://127.0.0.1:6379")),
      dbQuery_(std::move(q)) {
}

std::string AntiBreakdownCache::get(const std::string& key) {
    auto val = redis_->get(key);
    if (val) {
        std::cout << "[AntiBreakdown] HIT: " << key << std::endl;
        return *val;
    }

    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);

    val = redis_->get(key);
    if (val) {
        std::cout << "[AntiBreakdown] HIT after lock: " << key << std::endl;
        return *val;
    }

    std::cout << "[AntiBreakdown] query DB: " << key << std::endl;
    std::string value = dbQuery_(key);
    if (!value.empty()) {
        redis_->set(key, value, std::chrono::seconds(60));
    }

    return value;
}

// 练习4：延迟双删
DelayDoubleDelete::DelayDoubleDelete(DbQuery q)
    : redis_(std::make_shared<sw::redis::Redis>("tcp://127.0.0.1:6379")),
      dbQuery_(std::move(q)) {
}

void DelayDoubleDelete::setDbUpdater(DbUpdater updater) {
    dbUpdater_ = std::move(updater);
}

void DelayDoubleDelete::update(const std::string& key, const std::string& newValue) {
    redis_->del(key);
    std::cout << "[DelayDoubleDelete] step1: delete cache" << std::endl;

    if (dbUpdater_) {
        dbUpdater_(key, newValue);
    }
    std::cout << "[DelayDoubleDelete] step2: update DB with '" << newValue << "'" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    redis_->del(key);
    std::cout << "[DelayDoubleDelete] step3: delete cache again (delayed)" << std::endl;
}