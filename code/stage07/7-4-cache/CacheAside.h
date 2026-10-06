#pragma once
#include <string>
#include <functional>
#include <memory>

namespace sw { namespace redis { class Redis; } }

using DbQuery = std::function<std::string(const std::string& key)>;
using DbUpdater = std::function<void(const std::string& key, const std::string& value)>;

class CacheAside {
public:
    explicit CacheAside(DbQuery dbQuery);
    void setDbUpdater(DbUpdater updater);
    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value, int ttlSec = 60);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    DbQuery dbQuery_;
    DbUpdater dbUpdater_;
};

class AntiPenetrationCache {
public:
    explicit AntiPenetrationCache(DbQuery dbQuery);
    std::string get(const std::string& key);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    DbQuery dbQuery_;
};

class AntiBreakdownCache {
public:
    explicit AntiBreakdownCache(DbQuery dbQuery);
    std::string get(const std::string& key);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    DbQuery dbQuery_;
};

class DelayDoubleDelete {
public:
    explicit DelayDoubleDelete(DbQuery dbQuery);
    void setDbUpdater(DbUpdater updater);
    void update(const std::string& key, const std::string& newValue);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    DbQuery dbQuery_;
    DbUpdater dbUpdater_;
};