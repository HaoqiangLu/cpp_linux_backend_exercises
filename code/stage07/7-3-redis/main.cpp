#include "RedisClient.h"
#include <cstddef>
#include <exception>
#include <iostream>

int main() {
    try {
        RedisClient cli;

        std::cout << "=== 测试用户会话 ===" << std::endl;
        cli.setSession("session_001", "user_123", 1800);
        std::string userId = cli.getSession("session_001");
        std::cout << "Session session_001 -> userId: " << userId << std::endl;

        std::cout << "\n=== 测试接口限流 ===" << std::endl;
        for (int i = 0; i < 5; ++i) {
            bool limited = cli.isRateLimited("rate:user_123", 3, 60);
            std::cout << "Request " << (i + 1) << ": "
                      << (limited ? "LIMITED" : "ALLOWED") << std::endl;
        }

        std::cout << "\n=== 测试排行榜 ===" << std::endl;
        cli.updateScore("leaderboard", "alice", 95.5);
        cli.updateScore("leaderboard", "bob", 88.0);
        cli.updateScore("leaderboard", "charlie", 92.3);
        cli.updateScore("leaderboard", "david", 97.8);

        auto top3 = cli.topN("leaderboard", 3);
        std::cout << "Top 3 players:" << std::endl;
        for (size_t i = 0; i < top3.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << top3[i].first
                      << " - " << top3[i].second << "分" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}