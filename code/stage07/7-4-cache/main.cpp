#include "CacheAside.h"
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <unordered_map>

int main() {
    // 练习1：Cache Aside
    std::cout << "=== 练习1: Cache Aside ===" << std::endl;
    auto db = std::make_shared<std::unordered_map<std::string, std::string>>();
    (*db)["user:1"] = "Alice";
    (*db)["user:2"] = "Bob";

    DbQuery query = [db](const std::string& key) -> std::string {
        std::cout << "  [DB] query: " << key << std::endl;
        auto it = db->find(key);
        return it != db->end() ? it->second : "";
    };

    CacheAside cache(query);
    cache.setDbUpdater([db](const std::string& key, const std::string& value) {
        (*db)[key] = value;
    });

    std::cout << "1st get(user:1): " << cache.get("user:1") << std::endl;
    std::cout << "2nd get(user:1): " << cache.get("user:1") << " (should hit cache)" << std::endl;

    cache.set("user:1", "Alice_Updated", 60);
    std::cout << "after set, get(user:1): " << cache.get("user:1") << std::endl;

    std::cout << "\nget(user:999): '" << cache.get("user:999") << "' (not in DB)" << std::endl;

    // 练习2：缓存穿透防护
    std::cout << "\n=== 练习2: 缓存穿透防护 ===" << std::endl;
    AntiPenetrationCache antiPen(query);

    for (int i = 0; i < 3; ++i) {
        std::string val = antiPen.get("nonexistent_key");
        std::cout << "attempt " << (i + 1) << ": '" << val << "'" << std::endl;
    }

    // 练习3：缓存击穿防护
    std::cout << "\n=== 练习3: 缓存击穿防护 ===" << std::endl;
    std::atomic<int> dbCount{0};
    DbQuery slowQuery = [&dbCount](const std::string& key) -> std::string {
        ++dbCount;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return "db_value_of_" + key;
    };

    AntiBreakdownCache antiBd(slowQuery);

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&antiBd, i]() {
            std::string val = antiBd.get("hot_key");
            std::cout << "  thread" << i << ": " << val << std::endl;
        });
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "DB query count: " << dbCount.load() << " (mutex should reduce this)" << std::endl;

    // 练习4：延迟双删
    std::cout << "\n=== 练习4: 延迟双删 ===" << std::endl;
    auto db2 = std::make_shared<std::unordered_map<std::string, std::string>>();
    (*db2)["item:1"] = "old_value";

    DbQuery query2 = [db2](const std::string& key) -> std::string {
        auto it = db2->find(key);
        return it != db2->end() ? it->second : "";
    };

    DelayDoubleDelete ddd(query2);
    ddd.setDbUpdater([db2](const std::string& key, const std::string& value) {
        (*db2)[key] = value;
    });
    ddd.update("item:1", "new_value");
    std::cout << "DB after update: item:1 = " << (*db2)["item:1"] << std::endl;
}