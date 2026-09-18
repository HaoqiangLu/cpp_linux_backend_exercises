#include "SmartPointerExercises.h"
#include "ListNode.h"
#include "Watcher.h"

#include <cstdio>
#include <iostream>
#include <memory>

void uniqueArrayDemo() {
    // auto arr = std::make_unique<int[]>(5);
    // 填充并打印数组
    auto arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; i++) {
        arr[i] = i * 10;
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void sharedPtrDemo() {
    // auto sp = std::make_shared<int>(42);
    // 创建多个 shared_ptr 副本，打印 use_count()
    auto sp = std::make_shared<int>(42);
    std::cout << sp.use_count() << std::endl;
    {
        auto sp1 = sp;
        std::cout << sp.use_count() << std::endl;

        auto sp2 = sp1;
        std::cout << sp.use_count() << std::endl;
    }
    std::cout << sp.use_count() << std::endl;

    sp.reset();
    std::cout << "sp reset 后: " << sp.use_count() << std::endl;
}

void linkedListDemo() {
    // 用 std::unique_ptr<ListNode> 构建链表并遍历打印
    auto head = std::make_unique<ListNode>(1);
    head->next = std::make_unique<ListNode>(2);
    head->next->next = std::make_unique<ListNode>(3);
    head->next->next->next = std::make_unique<ListNode>(4);
    head->next->next->next->next = std::make_unique<ListNode>(5);

    // 遍历只是"观察"链表，不转移所有权：使用裸指针作为非拥有性游标，
    // 让 head 始终持有整条链表，函数结束时自动逐节点析构。
    for (auto current = head.get(); current != nullptr; current = current->next.get()) {
        std::cout << current->val << " ";
    }
    std::cout << std::endl;
}

void filePtrDemo() {
    struct FcloseDeleter {
        void operator()(FILE* fp) const noexcept {
            if (fp) std::fclose(fp);
        }
    };
    // std::unique_ptr<FILE, decltype(&fclose)> fp(std::fopen("text.txt", "w"), &std::fclose);
    std::unique_ptr<FILE, FcloseDeleter> fp{std::fopen("text.txt", "w")};
    fprintf(fp.get(), "Hello, World! unique_ptr");
}

void watchDemo() {
    // 用 std::unique_ptr / std::shared_ptr 管理 Watcher，观察析构时机
    std::cout << "=== unique_ptr 管理 Watcher ===" << std::endl;
    {
        std::unique_ptr<Watcher> up = std::make_unique<Watcher>("unique-watcher");
        std::cout << "即将离开作用域..." << std::endl;
    }
    std::cout << "已离开作用域\n" << std::endl;

    std::cout << "=== unique_ptr reset 提前释放 ===" << std::endl;
    {
        std::unique_ptr<Watcher> up = std::make_unique<Watcher>("unique-reset");
        std::cout << "调用 reset() ..." << std::endl;
        up.reset();
        std::cout << "reset() 完成" << std::endl;
    }
    std::cout << "已离开作用域" << std::endl;

    std::cout << "=== shared_ptr 管理 Watcher ===" << std::endl;
    {
        std::shared_ptr<Watcher> sp1 = std::make_shared<Watcher>("shared-watcher");
        {
            std::shared_ptr<Watcher> sp2 = sp1;
            std::cout << "use_count: " << sp1.use_count() << std::endl;
            std::cout << "sp2 即将离开作用域..." << std::endl;
        }
        std::cout << "sp2 已离开作用域, use_count: " << sp1.use_count() << std::endl;
        std::cout << "sp1 即将离开作用域..." << std::endl;
    }
    std::cout << "sp1 已离开作用域" << std::endl;
}