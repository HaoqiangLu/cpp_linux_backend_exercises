#include "SockOptExercises.h"

#include <iostream>
#include <string>

constexpr int kDefaultPort = 8888;

int main() {
    std::cout << "=== Socket 选项 (sockopt) 练习测试 ===" << std::endl;
    std::cout << "1. SO_REUSEADDR  (避免重启报 Address already in use)" << std::endl;
    std::cout << "2. SO_REUSEPORT  (多进程负载均衡)" << std::endl;
    std::cout << "3. TCP_NODELAY   (对比小消息场景下的延迟)" << std::endl;
    std::cout << "4. SO_KEEPALIVE  (检测对端异常断开)" << std::endl;
    std::cout << "5. SO_LINGER     (控制 close 行为)" << std::endl;
    std::cout << "请选择: ";

    std::string choice;
    std::getline(std::cin, choice);

    switch (std::stoi(choice)) {
        case 1: {
            std::cout << "请输入端口 (默认 " << kDefaultPort << "): ";
            std::string in;
            std::getline(std::cin, in);
            int port = in.empty() ? kDefaultPort : std::stoi(in);
            reuseAddrServer(port);
            break;
        }
        case 2: {
            std::cout << "请输入端口 (默认 " << kDefaultPort << "): ";
            std::string in;
            std::getline(std::cin, in);
            int port = in.empty() ? kDefaultPort : std::stoi(in);
            reusePortMultiProcess(port);
            break;
        }
        case 3: {
            std::cout << "=== TCP_NODELAY 延迟对比测试 (各 200 次 ping-pong) ===" << std::endl;
            tcpNoDelayTest(false);   // Nagle ON（默认）
            tcpNoDelayTest(true);    // TCP_NODELAY ON
            break;
        }
        case 4: {
            std::cout << "请输入端口 (默认 " << kDefaultPort << "): ";
            std::string in;
            std::getline(std::cin, in);
            int port = in.empty() ? kDefaultPort : std::stoi(in);
            keepAliveServer(port);
            break;
        }
        case 5: {
            std::cout << "是否启用 SO_LINGER? (1=是, 0=否): ";
            std::string en;
            std::getline(std::cin, en);
            bool enableLinger = !en.empty() && en[0] == '1';

            std::cout << "请输入 linger 秒数 (默认 0): ";
            std::string sec;
            std::getline(std::cin, sec);
            int lingerSec = sec.empty() ? 0 : std::stoi(sec);

            lingerTest(enableLinger, lingerSec);
            break;
        }
        default:
            std::cerr << "无效选项" << std::endl;
            break;
    }

    return 0;
}
