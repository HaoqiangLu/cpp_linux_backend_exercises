#include "IpcExercises.h"
#include <cstdio>
#include <fcntl.h>
#include <print>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <array>
#include <string>
#include <sys/msg.h>
#include <sys/ipc.h>

void pipeParentChild() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    // 父子进程间的单向数据传递
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        close(pipefd[0]);
        std::string message = "Hello Message form Child";
        write(pipefd[1], message.c_str(), message.size() + 1);
        close(pipefd[1]);
        _exit(0);
    } else {
        close(pipefd[1]);
        std::array<char, 128> buffer{};
        read(pipefd[0], buffer.data(), buffer.size());
        std::println("Parent: {}", buffer.data());
        close(pipefd[0]);
        wait(nullptr);
    }
}

void fifoChannel() {
    const char* path = "/tmp/my_fifo";

    if (mkfifo(path, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        // 子进程：写端
        int wfd = open(path, O_WRONLY);
        std::array<std::string, 3> msgs = {"Hello from child\n", "FIFO is nice\n", "bye\n"};
        for (const auto& msg : msgs) {
            write(wfd, msg.data(), msg.size() + 1);
        }
        close(wfd);  // 关闭写端，父进程 read 将返回 0（EOF）
        _exit(0);
    } else {
        // 父进程：读端
        int rfd = open(path, O_RDONLY);
        std::array<char, 128> buf;
        int n;
        while ((n = read(rfd, buf.data(), buf.size())) > 0) {
            std::println("Parent:\n{}", std::string_view(buf.data(), n));
        }
        close(rfd);
        wait(nullptr);
    }
    unlink(path);  // 清理 FIFO 文件
}

void socketpairDuplex() {
    int sv[2];

    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
    if (ret < 0) {
        perror("socketpair");
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        // 子进程：通过 sv[1] 通信
        std::string message = "Hello from child";
        write(sv[1], message.c_str(), message.size() + 1);
        shutdown(sv[1], SHUT_WR);  // 通知父进程：我写完了

        std::array<char, 128> buffer{};
        int n;
        while ((n = read(sv[1], buffer.data(), buffer.size())) > 0) {
            std::println("Child received: {}", std::string_view(buffer.data(), n));
        }
        close(sv[0]);
        close(sv[1]);
        _exit(0);
    } else {
        // 父进程：通过 sv[0] 通信
        std::array<char, 128> buffer{};
        int n;
        while ((n = read(sv[0], buffer.data(), buffer.size())) > 0) {
            std::println("Parent received: {}", std::string_view(buffer.data(), n));
        }

        std::string reply = "Hello from parent";
        write(sv[0], reply.c_str(), reply.size() + 1);
        shutdown(sv[0], SHUT_WR);  // 通知子进程：我写完了

        close(sv[0]);
        close(sv[1]);
        wait(nullptr);
    }
}

struct SharedData {
    pthread_mutex_t mtx;
    int buffer[8];
    int head, tail, count;
};

void producerConsumer() {
    int fd = shm_open("/my_shm", O_CREAT|O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        return;
    }

    int ret = ftruncate(fd, sizeof(SharedData));
    if (ret < 0) {
        perror("ftruncate");
        return;
    }

    SharedData* data = static_cast<SharedData*>(mmap(nullptr, sizeof(SharedData), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
    if (data == MAP_FAILED) {
        perror("mmap");
        return;
    }

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&data->mtx, &attr);
    pthread_mutexattr_destroy(&attr);

    // fork 出生产者与消费者进程
    // 生产者加锁 → 写 buffer → 解锁
    // 消费者加锁 → 读 buffer → 解锁
    // 初始化环形缓冲区
    data->head = 0;
    data->tail = 0;
    data->count = 0;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        // 子进程：生产者 —— 生产 5 个数据
        for (int i = 1; i <= 5; ++i) {
            pthread_mutex_lock(&data->mtx);
            while (data->count == 8) {
                pthread_mutex_unlock(&data->mtx);
                usleep(1000);
                pthread_mutex_lock(&data->mtx);
            }
            data->buffer[data->tail] = i;
            data->tail = (data->tail + 1) % 8;
            data->count++;
            std::println("[Producer] produced: {}", i);
            pthread_mutex_unlock(&data->mtx);
        }
        _exit(0);
    } else {
        // 父进程：消费者 —— 消费 5 个数据
        for (int i = 0; i < 5; ++i) {
            pthread_mutex_lock(&data->mtx);
            while (data->count == 0) {
                pthread_mutex_unlock(&data->mtx);
                usleep(1000);
                pthread_mutex_lock(&data->mtx);
            }
            int val = data->buffer[data->head];
            data->head = (data->head + 1) % 8;
            data->count--;
            std::println("[Consumer] consumed: {}", val);
            pthread_mutex_unlock(&data->mtx);
        }
        wait(nullptr);
        pthread_mutex_destroy(&data->mtx);
        munmap(data, sizeof(SharedData));
        close(fd);
        shm_unlink("/my_shm");
    }
}

// ==================== IPC Benchmark Helpers ====================

static constexpr size_t DATA_SIZE = 1024 * 1024;  // 1MB
static constexpr size_t CHUNK     = 4096;

static double nowMs() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}

// ---- 1. Pipe ----
static double benchPipe() {
    int pipefd[2];
    pipe(pipefd);

    double t0 = nowMs();
    pid_t pid = fork();
    if (pid == 0) {
        close(pipefd[0]);
        std::array<char, CHUNK> buf{};
        for (size_t i = 0; i < DATA_SIZE; i += CHUNK)
            write(pipefd[1], buf.data(), CHUNK);
        close(pipefd[1]);
        _exit(0);
    }
    close(pipefd[1]);
    std::array<char, CHUNK> buf{};
    while (read(pipefd[0], buf.data(), CHUNK) > 0) {}
    close(pipefd[0]);
    wait(nullptr);
    return nowMs() - t0;
}

// ---- 2. FIFO ----
static double benchFifo() {
    const char* path = "/tmp/bench_fifo";
    unlink(path);
    mkfifo(path, 0666);

    double t0 = nowMs();
    pid_t pid = fork();
    if (pid == 0) {
        int wfd = open(path, O_WRONLY);
        std::array<char, CHUNK> buf{};
        for (size_t i = 0; i < DATA_SIZE; i += CHUNK)
            write(wfd, buf.data(), CHUNK);
        close(wfd);
        _exit(0);
    }
    int rfd = open(path, O_RDONLY);
    std::array<char, CHUNK> buf{};
    while (read(rfd, buf.data(), CHUNK) > 0) {}
    close(rfd);
    wait(nullptr);
    unlink(path);
    return nowMs() - t0;
}

// ---- 3. Socketpair ----
static double benchSocketpair() {
    int sv[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

    double t0 = nowMs();
    pid_t pid = fork();
    if (pid == 0) {
        close(sv[0]);
        std::array<char, CHUNK> buf{};
        for (size_t i = 0; i < DATA_SIZE; i += CHUNK)
            write(sv[1], buf.data(), CHUNK);
        close(sv[1]);
        _exit(0);
    }
    close(sv[1]);
    std::array<char, CHUNK> buf{};
    while (read(sv[0], buf.data(), CHUNK) > 0) {}
    close(sv[0]);
    wait(nullptr);
    return nowMs() - t0;
}

// ---- 4. Shared Memory ----
struct BenchShm {
    pthread_mutex_t mtx;
    char data[1024 * 1024];
};

static double benchShm() {
    shm_unlink("/bench_shm");
    int fd = shm_open("/bench_shm", O_CREAT|O_RDWR, 0666);
    ftruncate(fd, sizeof(BenchShm));
    auto* shm = static_cast<BenchShm*>(mmap(nullptr, sizeof(BenchShm),
        PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm->mtx, &attr);
    pthread_mutexattr_destroy(&attr);

    double t0 = nowMs();
    pid_t pid = fork();
    if (pid == 0) {
        pthread_mutex_lock(&shm->mtx);
        memset(shm->data, 0xAB, DATA_SIZE);
        pthread_mutex_unlock(&shm->mtx);
        close(fd);
        _exit(0);
    }
    wait(nullptr);
    pthread_mutex_lock(&shm->mtx);
    volatile char sum = 0;
    for (size_t i = 0; i < DATA_SIZE; ++i) sum += shm->data[i];
    (void)sum;
    pthread_mutex_unlock(&shm->mtx);

    double elapsed = nowMs() - t0;
    pthread_mutex_destroy(&shm->mtx);
    munmap(shm, sizeof(BenchShm));
    close(fd);
    shm_unlink("/bench_shm");
    return elapsed;
}

// ---- 5. Message Queue ----
struct MsgBuf {
    long mtype;
    char mtext[CHUNK];
};

static double benchMsgqueue() {
    key_t key = ftok("/tmp", 'A');
    int msqid = msgget(key, IPC_CREAT|0666);

    double t0 = nowMs();
    pid_t pid = fork();
    if (pid == 0) {
        MsgBuf msg{};
        msg.mtype = 1;
        for (size_t i = 0; i < DATA_SIZE; i += CHUNK)
            msgsnd(msqid, &msg, CHUNK, 0);
        _exit(0);
    }
    MsgBuf msg{};
    for (size_t i = 0; i < DATA_SIZE; i += CHUNK)
        msgrcv(msqid, &msg, CHUNK, 0, 0);
    wait(nullptr);

    double elapsed = nowMs() - t0;
    msgctl(msqid, IPC_RMID, nullptr);
    return elapsed;
}

// ==================== Main Benchmark ====================

void ipcBenchmark() {
    std::println("\n========== IPC Benchmark (1MB data) ==========");

    double t1 = benchPipe();
    double t2 = benchFifo();
    double t3 = benchSocketpair();
    double t4 = benchShm();
    double t5 = benchMsgqueue();

    std::println("{:<15} {:>12} {:>14}", "IPC Method", "Time (ms)", "Throughput");
    std::println("{:-<43}", "");

    auto show = [](const char* name, double ms) {
        double mb = DATA_SIZE / (1024.0 * 1024.0);
        double throughput = mb / (ms / 1000.0);
        std::println("{:<15} {:>10.2f} {:>10.2f} MB/s", name, ms, throughput);
    };
    show("Pipe",        t1);
    show("FIFO",        t2);
    show("Socketpair",  t3);
    show("Shared Mem",  t4);
    show("Msg Queue",   t5);

    std::println(
        "\n---------- Summary ----------"
        "\n- Pipe:        最简单，适合父子进程单向数据流"
        "\n- FIFO:        可用于无亲缘关系的进程，有文件系统路径"
        "\n- Socketpair:  全双工，适合双向交互通信"
        "\n- Shared Mem:  最快，零拷贝，但需自行同步（互斥锁/信号量）"
        "\n- Msg Queue:   按消息边界传输，自带排队，但有内核拷贝开销"
    );
}