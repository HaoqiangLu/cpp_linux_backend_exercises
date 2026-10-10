#pragma once
#include <memory>
#include <string>

class TcpConnection;

class StaticFileSender {
public:
    static void sendWithReadWrite(const std::shared_ptr<TcpConnection>& conn, const std::string& path);
    static void sendWithSendfile(const std::shared_ptr<TcpConnection>& conn, const std::string& path);
    static void sendWithMmap(const std::shared_ptr<TcpConnection>& conn, const std::string& path);
};