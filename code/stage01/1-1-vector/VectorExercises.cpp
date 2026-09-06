#include "VectorExercises.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

void reversePrint(std::vector<int> v) {
    std::cout << "[rbegin/rend]\n";
    for (auto it = v.rbegin(); it != v.rend(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "[reverse]\n";
    std::reverse(v.begin(), v.end());
    for (auto it = v.begin(); it != v.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void statistics(const std::vector<int>& v) {
    // 统计 vector 中的最大值
    auto max_it = std::max_element(v.begin(), v.end());
    std::cout << "Max value: " << *max_it << std::endl;

    // 统计 vector 中的最小值
    auto min_it = std::min_element(v.begin(), v.end());
    std::cout << "Min value: " << *min_it << std::endl;

    // 统计 vector 中的平均值
    double average = std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    std::cout << "Average value: " << average << std::endl;
}

void removeElement(std::vector<int>& v, int target) {
    std::cout << "Before remove: ";
    for (const auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    /*
     * erase-remove 惯用法
     *
     * std::remove
     *      1. 将所有不等于 target 的元素移动到前面，返回指向新逻辑结尾的迭代器
     *      2. 不会改变容器的大小，逻辑上删除了元素，但物理上仍然存在
     *      3. 返回的迭代器指向新逻辑结尾
     * std::vector::erase
     *      1. 删除指定范围的元素，改变容器的大小
     *      2. 物理上删除元素，释放内存
     */
    std::vector<int> v1{v};
    v1.erase(std::remove(v1.begin(), v1.end(), target), v1.end());
    for (const auto& e : v1) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    /*
     * 手动迭代器遍历删除
     *
     * std::vector::erase
     *     1. 删除单个元素，返回被删除元素后面那个元素的迭代器
     *     2. 删除范围内的元素，返回被删除范围后面那个元素的迭代器
     *     3. 删除后没有元素，返回新的 end()
     */
    std::vector<int> v2{v};
    for (auto it = v2.begin(); it != v2.end();) {
        if (*it == target) {
            it = v2.erase(it);
        } else {
            ++it;
        }
    }
    for (const auto& e : v2) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

void emplaceInsertClear() {
    std::vector<int> v{1, 2, 3};

    /*
     * emplace_back vs push_back
     *     1. push_back: 先构造对象，再拷贝/移动进容器
     *     2. emplace_back: 直接在容器尾部原地构造，省去拷贝/移动开销
     *     3. 对于基础类型两者性能差异可忽略，复杂类型（如自定义类）差异明显
     */
    v.emplace_back(4);
    v.emplace_back(5);
    std::cout << "After emplace_back 4, 5: ";
    for (const auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    /*
     * insert
     *     1. 在指定位置前插入元素，返回指向新插入元素的迭代器
     *     2. 插入点之后的所有元素向后移动，时间复杂度 O(n)
     *     3. 可能导致迭代器失效（所有插入点之后的迭代器）
     */
    v.insert(v.begin(), 0);           // 在头部插入 0
    v.insert(v.begin() + 3, 99);      // 在索引 3 处插入 99
    std::cout << "After insert 0 at begin, 99 at index 3: ";
    for (const auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    /*
     * clear
     *     1. 删除所有元素，size 变为 0
     *     2. 不改变 capacity（内存未释放，可复用）
     *     3. 若需释放内存，可用 shrink_to_fit() 或 swap 惯用法
     */
    std::cout << "Before clear: size=" << v.size()
              << ", capacity=" << v.capacity() << std::endl;
    v.clear();
    std::cout << "After clear: size=" << v.size()
              << ", capacity=" << v.capacity() << std::endl;
}