#pragma once
#include "Task.h"
#include <string>
#include <vector>

class TaskManager {
private:
    std::vector<Task> tasks_;
    int nextId_ = 1;
public:
    void add(const std::string& title, int priority);
    void complete(int id);
    void remove(int id);
    void showPending() const;
    void sortByPriority();
};