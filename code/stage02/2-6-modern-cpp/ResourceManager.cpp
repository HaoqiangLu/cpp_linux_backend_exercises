#include "ResourceManager.h"
#include "Resource.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>

void ResourceManager::add(std::string name, std::string type, int priority) {
    auto newRes = std::make_unique<Resource>(Resource{std::move(name), std::move(type), priority});
    this->resources_.push_back(std::move(newRes));
}

void ResourceManager::remove(const std::string& name) {
    auto it = std::find_if(this->resources_.begin(), this->resources_.end(),
        [&name] (const std::unique_ptr<Resource>& res) {
            return res->name == name;
        }
    );
    if (it != this->resources_.end()) {
        this->resources_.erase(it);
    }
}

void ResourceManager::printByPriority() const {
    // 拷贝裸指针到临时 vector，std::sort + Lambda 按 priority 排序，打印
    std::vector<const Resource*> tmp;
    tmp.reserve(this->resources_.size());
    for (const auto& res : this->resources_) {
        tmp.push_back(res.get());
    }
    std::sort(tmp.begin(), tmp.end(),
        [] (const Resource* a, const Resource* b) {
            return a->priority < b->priority;
        }
    );

    for (const auto* res : tmp) {
        std::cout << "name:" << res->name
            << ", type:" << res->type
            << ", priority:" << res->priority
            << std::endl;
    }
}

void ResourceManager::filterByType(const std::string& type) const {
    std::cout << "type=" << type << ":" << std::endl;
    for (const auto& res : this->resources_) {
        if (res->type == type) {
            std::cout << "  name:" << res->name
                << ", priority:" << res->priority
                << std::endl;
        }
    }
}