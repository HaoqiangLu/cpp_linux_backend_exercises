#pragma once
#include <string>

struct User {
    int id;
    std::string username;
    std::string passwordHash;
    std::string createdAt;
};

// 练习4：RAII 封装的 CRUD 类
class UserRepo {
public:
    UserRepo(const char* host, const char* user, const char* pwd, const char* db, int port = 3306);
    ~UserRepo();

    // 练习2：用户注册（密码用 bcrypt 哈希存储）
    bool registerUser(const std::string& username, const std::string& password);
    // 练习3：用预处理语句实现登录
    bool login(const std::string& username, const std::string& password);
    // 练习5：演示 SQL 注入场景
    void demoInjection();

private:
    void* conn_;    // MYSQL* 实际类型
};