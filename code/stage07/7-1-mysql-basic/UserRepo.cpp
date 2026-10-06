#include "UserRepo.h"
#include <cstdint>
#include <mysql/mysql.h>
#include <mariadb_com.h>
#include <crypt.h>
#include <cstdio>
#include <cstring>

UserRepo::UserRepo(const char* host, const char* user, const char* pwd, const char* db, int port) {
    conn_ = mysql_init(nullptr);
    if(mysql_real_connect((MYSQL*)conn_, host, user, pwd, db, port, nullptr, 0) == nullptr) {
        std::fprintf(stderr, "连接失败: %s\n", mysql_error((MYSQL*)conn_));
    }
}

UserRepo::~UserRepo() {
    mysql_close((MYSQL*)conn_);
}

bool UserRepo::registerUser(const std::string& username, const std::string& password) {
    char* hash = crypt(password.c_str(), "$2b$10$abcdefghijklmnopqrstuv");
    if (hash == nullptr) {
        std::perror("crypt");
        return false;
    }

    MYSQL_STMT* stmt = mysql_stmt_init((MYSQL*)conn_);
    const char* sql = "INSERT INTO users(username, password_hash) VALUES (?, ?)";
    if (mysql_stmt_prepare(stmt, sql, std::strlen(sql))) {
        std::fprintf(stderr, "与处理失败: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    MYSQL_BIND bind[2]{};
    uint64_t nameLen = username.size();
    uint64_t hashLen = std::strlen(hash);

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)username.c_str();
    bind[0].buffer_length = nameLen;
    bind[0].length = &nameLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)hash;
    bind[1].buffer_length = hashLen;
    bind[1].length = &hashLen;

    if (mysql_stmt_bind_param(stmt, bind)) {
        std::fprintf(stderr, "绑定失败: %s", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    bool ok = (mysql_stmt_execute(stmt) == 0);
    if (!ok) {
        std::fprintf(stderr, "插入失败: %s\n", mysql_stmt_error(stmt));
    }
    mysql_stmt_close(stmt);
    return ok;
}

bool UserRepo::login(const std::string& username, const std::string& password) {
    MYSQL_STMT* stmt = mysql_stmt_init((MYSQL*)conn_);
    const char* sql = "SELECT password_hash FROM users WHERE username = ?";
    if (mysql_stmt_prepare(stmt, sql, std::strlen(sql))) {
        std::fprintf(stderr, "预处理失败: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    MYSQL_BIND inBind[1]{};
    uint64_t nameLen = username.size();
    inBind[0].buffer_type = MYSQL_TYPE_STRING;
    inBind[0].buffer = (void*)username.c_str();
    inBind[0].buffer_length = nameLen;
    inBind[0].length = &nameLen;

    if (mysql_stmt_bind_param(stmt, inBind)) {
        std::fprintf(stderr, "绑定失败: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    if (mysql_stmt_execute(stmt)) {
        std::fprintf(stderr, "执行失败: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    char hashBuf[256]{};
    uint64_t hashLen = 0;
    MYSQL_BIND outBind[1]{};
    outBind[0].buffer_type = MYSQL_TYPE_STRING;
    outBind[0].buffer = hashBuf;
    outBind[0].buffer_length = sizeof(hashBuf);
    outBind[0].length = &hashLen;

    mysql_stmt_bind_result(stmt, outBind);
    bool found = (mysql_stmt_fetch(stmt) == 0);
    mysql_stmt_close(stmt);

    if (!found) {
        std::printf("用户 '%s' 不存在\n", username.c_str());
        return false;
    }

    char* computed = crypt(password.c_str(), hashBuf);
    if (!computed) {
        std::perror("crypt");
        return false;
    }

    bool match = (std::strcmp(computed, hashBuf) == 0);
    std::printf("登录 %s: %s\n", username.c_str(), match ? "成功" : "密码错误");
    return match;
}

void UserRepo::demoInjection() {
    std::string maliciousInput = "' OR '1'='1";
    std::string unsafeQuery = "SELECT * FROM users WHERE username = '" + maliciousInput + "'";
    std::printf("=== SQL 注入演示 ===\n");
    std::printf("拼接的危险查询: %s\n", unsafeQuery.c_str());
    std::printf("该查询会返回 users 表所有行, 绕过身份验证!\n\n");

    std::printf("使用预处理语句后，参数作为纯数据传递，不会被解析为 SQL 语法\n");

    MYSQL_STMT* stmt = mysql_stmt_init((MYSQL*)conn_);
    const char* sql = "SELECT id, username FROM users WHERE username = ?";
    mysql_stmt_prepare(stmt, sql, std::strlen(sql));

    MYSQL_BIND bind[1]{};
    uint64_t len = maliciousInput.size();
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)maliciousInput.c_str();
    bind[0].buffer_length = len;
    bind[0].length = &len;

    mysql_stmt_bind_param(stmt, bind);
    mysql_stmt_execute(stmt);

    int rows = 0;
    while (mysql_stmt_fetch(stmt) == 0) {
        ++rows;
    }
    std::printf("预处理语句查询结果: %d 行（安全！恶意输入被当作普通字符串）\n", rows);
    mysql_stmt_close(stmt);
}