#include "UserRepo.h"
#include <cstdio>

int main() {
    UserRepo repo("127.0.0.1", "root", "password", "testdb");

    std::printf("=== 注册用户 ===\n");
    repo.registerUser("alice", "123456");
    repo.registerUser("bob", "abcdef");

    std::printf("\n=== 登陆测试 ===\n");
    repo.login("alice", "123456");
    repo.login("alice", "wrongpwd");
    repo.login("nonexistent", "xxx");

    std::printf("\n");
    repo.demoInjection();
}