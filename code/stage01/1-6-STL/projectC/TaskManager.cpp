#include "TaskManager.h"
#include <algorithm>
#include <iostream>

void TaskManager::add(const std::string& title, int priority) {
    tasks_.emplace_back(nextId_++, title, priority);
}

void TaskManager::complete(int id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
        [&id] (const Task& task) { return task.id == id; }
    );
    if (it != tasks_.end()) {
        it->done = true;
    }
}

void TaskManager::remove(int id) {
    tasks_.erase(
        std::remove_if(tasks_.begin(), tasks_.end(),
            [&id] (const Task& task) { return task.id == id; }
        ),
        tasks_.end()
    );
}

void TaskManager::showPending() const {
    std::cout << "Pending tasks:" << std::endl;
    std::cout << "-----------------" << std::endl;
    for (const auto& task : tasks_) {
        if (!task.done) {
            std::cout << task.id << " - " << task.title << " (priority: " << task.priority << ")" << std::endl;
        }
    }
    std::cout << "-----------------" << std::endl;
}

void TaskManager::sortByPriority() {
    std::sort(tasks_.begin(), tasks_.end(),
        [] (const Task& a, const Task& b) {
            if (a.priority != b.priority) 
                return a.priority > b.priority; // 先按优先级降序
            return a.id < b.id;                                           // 再按 id 升序
        }
    );
}