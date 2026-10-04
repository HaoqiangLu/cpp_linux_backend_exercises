#pragma once

#include <cstdint>
#include <functional>
#include <string>

// 拆包回调：每拆出一条完整消息就回调一次
using MessageCallback = std::function<void(const std::string& msg)>;

// 练习1：定长包协议（每条消息固定 64 字节）
class FixedLengthCodec {
public:
    explicit FixedLengthCodec(size_t fixedLen = 64);
    // 输入接收缓冲区新数据，内部拆包并回调
    void onData(const char* data, size_t len, const MessageCallback& cb);

private:
    size_t fixedLen_;
    std::string buffer_;
};

// 练习2：分隔符包协议（以 '\n' 为分隔）
class DelimiterCodec {
public:
    explicit DelimiterCodec(char delim = '\n');
    void onData(const char* data, size_t len, const MessageCallback& cb);

private:
    char delim_;
    std::string buffer_;
};

// 练习3：Length-Field 协议（前 4 字节大端长度 + 消息体）
class LengthFieldCodec {
public:
    void onData(const char* data, size_t len, const MessageCallback& cb);

private:
    std::string buffer_;
};