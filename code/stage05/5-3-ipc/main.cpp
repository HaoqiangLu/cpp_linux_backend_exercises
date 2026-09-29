#include "IpcExercises.h"
#include <iostream>

int main() {
    std::cout << "===== 练习1 =====" << std::endl;
    pipeParentChild();

    std::cout << "\n===== 练习2 =====" << std::endl;
    fifoChannel();

    std::cout << "\n===== 练习3 =====" << std::endl;
    socketpairDuplex();

    std::cout << "\n===== 练习4 =====" << std::endl;
    producerConsumer();

    std::cout << "\n===== 练习5 =====" << std::endl;
    ipcBenchmark();
}