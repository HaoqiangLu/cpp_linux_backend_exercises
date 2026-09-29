#pragma once

// 练习1：用 mmap 映射文件，实现内存式读取
void mmapReadFile(const char* path);

// 练习2：用匿名 mmap 在父子进程间共享计数器
void anonymousMmapCounter();

// 练习3：用 POSIX 共享内存实现两个独立进程间的字符串传递
void posixSharedMemory();

// 练习4：用 mmap 实现简单的共享内存日志（多进程追加写，另一进程读）
void sharedMemoryLog();
