#include "HttpServer.h"
#include "Config.h"
#include "Logger.h"

namespace {
LogLevel parseLogLevel(const std::string& s) {
    if (s == "DEBUG") return LogLevel::DEBUG;
    if (s == "WARN") return LogLevel::WARN;
    if (s == "ERROR") return LogLevel::ERROR;
    return LogLevel::INFO;
}
}   // namespace

int main() {
    Config cfg;
    if (!cfg.load("./conf/server.conf")) {
        LOG_WARN("load ./conf/server.conf failed, using defaults");
    }

    int port = cfg.getInt("port", 8080);
    int subReactorCount = cfg.getInt("sub_reactor_count", 3);
    int threadPoolSize = cfg.getInt("thread_pool_size", 4);
    std::string docRoot = cfg.getString("document_root", "./www");
    std::string logPath = cfg.getString("log_path", "");
    std::string logLevel = cfg.getString("log_level", "INFO");
    int connTimeoutMs = cfg.getInt("connection_timeout_ms", 30000);
    std::string staticStrategy = cfg.getString("static_file_strategy", "readwrite");

    Logger::instance().setLevel(parseLogLevel(logLevel));
    if (!logPath.empty()) {
        Logger::instance().setOutput(logPath);
    }

    LOG_INFO("config load: port=%d subReactor=%d pool=%d docRoot=%s strategy=%s",
             port, subReactorCount, threadPoolSize, docRoot.c_str(), staticStrategy.c_str());

    HttpServer server(port, subReactorCount, threadPoolSize);
    server.setDocumentRoot(docRoot);
    server.setConnectionTimeoutMs(connTimeoutMs);
    server.setStaticFileStrategy(staticStrategy);
    server.start();

    LOG_INFO("HttpServer exited cleanly");

    return 0;
}