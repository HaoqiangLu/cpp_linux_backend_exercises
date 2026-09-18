#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Resource.h"

// 练习C：小型资源管理器
class ResourceManager {
private:
    std::vector<std::unique_ptr<Resource>> resources_;

public:
    void add(std::string name, std::string type, int priority);
    void remove(const std::string& name);
    void printByPriority() const;
    void filterByType(const std::string& type) const;
};