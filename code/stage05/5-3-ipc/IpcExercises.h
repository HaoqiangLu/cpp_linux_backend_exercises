#pragma once

// 练习1：用 pipe 实现父子进程间的单向数据传递
void pipeParentChild();

// 练习2：用 FIFO 实现两个独立进程间的命令通道
void fifoChannel();

// 练习3：用 socketpair 实现全双工的父子进程通信
void socketpairDuplex();

// 练习4：用 POSIX 共享内存 + 互斥锁实现生产者-消费者
void producerConsumer();

// 练习5：对比五种 IPC 方式的性能与适用场景，写一份小结
void ipcBenchmark();