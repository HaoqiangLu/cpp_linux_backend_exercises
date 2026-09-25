#include "ChatServer.h"
#include "CmdServer.h"
#include "EchoClient.h"
#include "EchoServer.h"
#include "HttpServer.h"
#include "MultiClientServer.h"

#include <iostream>
#include <string>

int main() {
    std::cout << "=== Socket 练习测试 ===" << std::endl;
    std::cout << "1. EchoServer (TCP 回声服务器)" << std::endl;
    std::cout << "2. EchoClient (TCP 回声客户端)" << std::endl;
    std::cout << "3. ChatServer (TCP 聊天服务器)" << std::endl;
    std::cout << "4. ChatClient (TCP 聊天客户端)" << std::endl;
    std::cout << "5. MultiClientServer (多客户端服务器)" << std::endl;
    std::cout << "6. CmdServer (命令执行服务器)" << std::endl;
    std::cout << "7. HttpServer (HTTP 服务器)" << std::endl;
    std::cout << "请选择: ";

    std::string choice;
    std::getline(std::cin, choice);

    switch (std::stoi(choice)) {
        case 1: echoServer(); break;
        case 2: echoClient(); break;
        case 3: chatServer(); break;
        case 4: chatClient(); break;
        case 5: multiClientServer(); break;
        case 6: cmdServer(); break;
        case 7: httpServer(); break;
        default: std::cerr << "无效选项" << std::endl; break;
    }

    return 0;
}