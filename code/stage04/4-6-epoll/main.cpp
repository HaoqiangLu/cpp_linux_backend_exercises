#include "EpollChatServer.h"
#include "EpollServer.h"
#include "PollServer.h"
#include "SelectServer.h"

#include <iostream>
#include <string>

int main() {
    std::cout << "=== epoll 练习测试 ===" << std::endl;
    std::cout << "1. SelectServer (select 多客户端服务器)" << std::endl;
    std::cout << "2. PollServer (poll 多客户端服务器)" << std::endl;
    std::cout << "3. EpollServerLT (epoll LT 模式)" << std::endl;
    std::cout << "4. EpollServerET (epoll ET 模式)" << std::endl;
    std::cout << "5. EpollChatServer (epoll 聊天服务器)" << std::endl;
    std::cout << "6. NonBlockingEpollServer (非阻塞 epoll 服务器)" << std::endl;
    std::cout << "请选择: ";

    std::string choice;
    std::getline(std::cin, choice);

    switch (std::stoi(choice)) {
        case 1: selectServer(); break;
        case 2: pollServer(); break;
        case 3: epollServerLT(); break;
        case 4: epollServerET(); break;
        case 5: epollChatServer(); break;
        case 6: nonBlockingEpollServer(); break;
        default: std::cerr << "无效选项" << std::endl; break;
    }

    return 0;
}