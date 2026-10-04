#include "TcpExercises.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== 6.1 TCP 协议细节 练习测试 ===" << std::endl;
    std::cout << "1. startEchoServer    (回声服务端, 制造 CLOSE_WAIT)" << std::endl;
    std::cout << "2. startBrokenClient  (坏客户端, 不发 FIN)" << std::endl;
    std::cout << "3. shortConnectionStress (短连接压测, 制造 TIME_WAIT)" << std::endl;
    std::cout << "4. printTcpStats      (ss -s 系统级 TCP 统计)" << std::endl;
    std::cout << "5. startStressEchoServer (练习3专用, echo后立即close)" << std::endl;
    std::cout << "请选择: ";

    std::string choice;
    std::getline(std::cin, choice);

    switch (std::stoi(choice)) {
        case 1: startEchoServer(); break;
        case 2: startBrokenClient(); break;
        case 3: {
            std::cout << "压测次数: ";
            std::string n;
            std::getline(std::cin, n);
            shortConnectionStress(std::stoi(n));   // 需要额外读一个参数
            break;
        }
        case 4: printTcpStats(); break;
        case 5: startStressEchoServer(); break;
        default: std::cerr << "无效选项" << std::endl; break;
    }

    return 0;
}