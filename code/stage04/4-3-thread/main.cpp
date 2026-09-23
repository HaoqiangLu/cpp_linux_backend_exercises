# include "ThreadExercises.h"
# include <iostream>

int main() {
    // 练习2：一线程输入、一线程处理
    // 生产者线程填充数据，消费者线程处理数据

    // 练习4：生产者消费者模型（框架，完整实现见 4.4）
    // 用 std::mutex + std::condition_variable 实现

    // 调用以上函数进行测试
    std::cout << "===== 多线程打印数字 =====" << std::endl;
    multiPrintDemo();

    std::cout << "===== 数据竞争示例 =====" << std::endl;
    raceDemo();

    std::cout << "===== 下载模拟器 =====" << std::endl;
    downloadSimulator("example.txt", 10, 5);
}