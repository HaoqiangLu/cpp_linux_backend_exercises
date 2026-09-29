#include "MmapExercises.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

void mmapReadFile(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    struct stat st;
    int ret = fstat(fd, &st);
    if (ret < 0) {
        perror("fstat");
        close(fd);
        return;
    }

    char* addr = static_cast<char*>(mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }

    for (ssize_t i = 0; i < st.st_size; ++i) {
        putchar(addr[i]);
        if (addr[i] == '\n') {
            fflush(stdout);
        }
    }

    write(STDOUT_FILENO, addr, st.st_size);

    ret = munmap(addr, st.st_size);
    if (ret < 0) {
        perror("munmap");
    }

    close(fd);
}

void anonymousMmapCounter() {
    int* counter = static_cast<int*>(mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0));
    if (counter == MAP_FAILED) {
        perror("mmap");
        return;
    }

    *counter = 0;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        for (int i = 0; i < 1000000; ++i) {
            ++(*counter);
        }
        _exit(0);
    } else {
        for (int i = 0; i < 1000000; ++i) {
            ++(*counter);
        }

        wait(nullptr);
    }

    // 没有加锁，实际结果可能小于 2000000，因为 ++(*counter) 不是原子操作，存在竞态条件
    printf("Counter: %d\n", *counter);

    int ret = munmap(counter, 4096);
    if (ret < 0) {
        perror("munmap");
    }
}

void posixSharedMemory() {
    int fd = shm_open("/my_shm", O_CREAT|O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        return;
    }

    int ret = ftruncate(fd, 4096);
    if (ret < 0) {
        perror("ftruncate");
        return;
    }

    char* addr = static_cast<char*>(mmap(nullptr, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
    if (addr == MAP_FAILED) {
        perror("mmap");
        return;
    }

    const char* message = "Hello, World!\n";
    memcpy(addr, message, strlen(message) + 1);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        printf("Child: %s", addr);
        fflush(stdout);
        _exit(0);
    } else {
        wait(nullptr);
    }

    ret = munmap(addr, 4096);
    if (ret < 0) {
        perror("munmap");
    }
    close(fd);
    shm_unlink("/my_shm");
}

struct LogHeader {
    size_t write_pos;
    char data[4096 - sizeof(size_t)];
};

void sharedMemoryLog() {
    int fd = shm_open("/my_shmlog", O_CREAT|O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        return;
    }

    int ret = ftruncate(fd, 4096);
    if (ret < 0) {
        perror("ftruncate");
        return;
    }

    LogHeader* header = static_cast<LogHeader*>(mmap(nullptr, sizeof(struct LogHeader), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
    if (header == MAP_FAILED) {
        perror("mmap");
        return;
    }

    // fork 多个子进程，各自用 __sync_fetch_and_add 原子推进 write_pos 并写入日志
    // 父进程 sleep 后读取整块日志打印
    for (int i = 0; i < 10; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            size_t msg_len = strlen("Hello, World!\n") + 1;
            size_t pos = __sync_fetch_and_add(&header->write_pos, msg_len);
            memcpy(header->data + pos, "Hello, World!\n", msg_len);
            _exit(0);
        }
    }

    for (int i = 0; i < 10; ++i) {
        wait(nullptr);
    }

    printf("Log:\n");
    fflush(stdout);
    write(STDOUT_FILENO, header->data, header->write_pos);

    munmap(header, sizeof(struct LogHeader));
    close(fd);
    shm_unlink("/my_shmlog");
}