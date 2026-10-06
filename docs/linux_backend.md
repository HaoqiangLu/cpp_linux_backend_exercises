# 阶段一：STL 容器与算法速成

本阶段核心目标是完成从“C 风格手写数据结构”到“C++ 标准容器 + 算法”的思维转换。重点不在于背诵接口，而在于理解容器的内存模型、复杂度特性以及算法与迭代器的配合。

## 1.1 vector 动态数组

- **练习目标**：
    - 理解 `vector` 的底层内存模型与动态扩容机制（`size` vs `capacity`，1.5 倍增长策略）。
    - 掌握 `push_back`/`emplace_back`/`pop_back`/`insert`/`erase`/`clear` 的用法与适用场景。
    - 掌握 `reserve` 与 `resize` 的区别，理解何时需要预分配内存。
    - 理解迭代器失效的场景，能正确处理删除/插入过程中的迭代器失效问题。
    - 熟练使用下标访问、迭代器遍历与范围 for 循环进行随机访问与遍历。
- **练习任务**：
    1. 输入若干整数存入 `vector<int>`，逆序输出。
    2. 统计 `vector<int>` 中的最大值、最小值、平均值。
    3. 删除 `vector` 中所有等于某个特定值的元素（注意迭代器失效问题）。
    4. 实现"动态成绩表"：支持添加成绩、删除末尾成绩、打印全部、计算平均分。
    5. 演示 `emplace_back`、`insert`、`clear` 的用法，观察 `size` 与 `capacity` 的变化。
- **巩固标准**：
  - [ ] 能不看文档独立完成练习任务 1-5 的全部功能实现。
  - [ ] 能准确说出 `push_back` 与 `emplace_back` 在构造对象时的性能差异，并举出适用场景。
    > **知识讲解**：`push_back` 先在外部构造临时对象，再拷贝/移动到容器尾部；`emplace_back` 直接在容器尾部原地构造，省去一次拷贝/移动。对于 `int` 等基础类型两者几乎无差异，但对于 `std::string`、自定义类等复杂对象，`emplace_back` 可以避免不必要的拷贝开销。适用场景：向容器中添加复杂对象时优先使用 `emplace_back`。
  - [ ] 能在代码中正确使用 `reserve` 预分配内存，并解释其避免的扩容次数与拷贝开销。
    > **知识讲解**：`reserve(n)` 预分配至少 n 个元素的内存空间，只改变 `capacity` 不改变 `size`。当已知元素数量时提前 `reserve`，可以避免 `vector` 多次扩容（每次约 1.5 倍）带来的重复全量拷贝。例如插入 1000 个元素，不 `reserve` 可能扩容约 17 次，`reserve(1000)` 则只需 1 次分配。
  - [ ] 能解释练习 3 中迭代器失效的原因，并至少写出一种正确的删除方式（erase-remove 惯用法或手动迭代器递增控制）。
    > **知识讲解**：`vector` 在 `insert`/`erase` 后，被修改位置之后的所有迭代器都会失效，因为元素在内存中被移动了。删除元素时如果仍用 `it++` 递增迭代器，会跳过元素或访问已失效的迭代器。正确方式：① erase-remove 惯用法——`std::remove` 将保留元素前移并返回新逻辑结尾，再 `erase` 删除尾部多余元素；② 手动迭代器控制——删除时不递增迭代器（`erase` 返回下一个有效迭代器），保留时才递增。
  - [ ] 能说出 `vector` 扩容时的时间复杂度（均摊 O(1)）以及 `reserve` 与 `resize` 对 `size`/`capacity` 的不同影响。
    > **知识讲解**：单次扩容时间复杂度为 O(n)（需拷贝所有旧元素），但由于 1.5 倍增长策略，均摊每次插入仍为 O(1)。`reserve(n)` 只改变 `capacity` 不改变 `size`，用于预分配内存；`resize(n)` 改变 `size`（可能也改变 `capacity`），用于增减实际元素数量——扩容时新元素值初始化，缩小时尾部元素被丢弃。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 1.1 vector 练习框架 ===
// 项目结构:
// 1-1-vector/
// ├── GradeBook.h
// ├── GradeBook.cpp
// ├── VectorExercises.h
// ├── VectorExercises.cpp
// └── main.cpp

// ---------- GradeBook.h ----------
#pragma once
#include <vector>

// 练习4: 动态成绩表
class GradeBook {
public:
    void addGrade(double g);            // TODO: 添加成绩（使用 emplace_back）
    void removeLast();                  // TODO: 删除末尾成绩
    void clear();                       // TODO: 清空所有成绩
    void printAll() const;              // TODO: 打印全部成绩
    double average() const;             // TODO: 计算并返回平均分

private:
    std::vector<double> grades_;
};

// ---------- GradeBook.cpp ----------
#include "GradeBook.h"
#include <iostream>
#include <numeric>

void GradeBook::addGrade(double g) {
    // TODO: grades_.emplace_back(g)
}

void GradeBook::removeLast() {
    // TODO: 判空后 grades_.pop_back()
}

void GradeBook::clear() {
    // TODO: grades_.clear()
}

void GradeBook::printAll() const {
    // TODO: 遍历 grades_ 并输出
}

double GradeBook::average() const {
    // TODO: 使用 std::accumulate 求和后除以 size
    return 0.0;
}

// ---------- VectorExercises.h ----------
#pragma once
#include <vector>

// 练习1: 输入若干整数存入vector，逆序输出
void reversePrint();

// 练习2: 统计vector中的最大值、最小值、平均值
void statistics(const std::vector<int>& v);

// 练习3: 删除vector中所有等于target的元素（注意迭代器失效）
void removeElement(std::vector<int>& v, int target);

// 练习5: 演示 emplace_back、insert、clear 的用法
void emplaceInsertClear();

// ---------- VectorExercises.cpp ----------
#include "VectorExercises.h"
#include <algorithm>
#include <iostream>
#include <numeric>

void reversePrint() {
    std::vector<int> v;
    int x = 0;
    // TODO: 循环读取输入，v.push_back(x)
    // TODO: 逆序遍历并输出（可用 rbegin/rend 或 std::reverse）
}

void statistics(const std::vector<int>& v) {
    // TODO: 使用 std::max_element / std::min_element / std::accumulate
    // TODO: 输出结果
}

void removeElement(std::vector<int>& v, int target) {
    // TODO: 使用 erase-remove 惯用法 或 手动迭代器遍历删除
    // 提示: v.erase(std::remove(v.begin(), v.end(), target), v.end());
}

void emplaceInsertClear() {
    std::vector<int> v{1, 2, 3};
    // TODO: 使用 emplace_back 在尾部添加元素
    // TODO: 使用 insert 在指定位置插入元素
    // TODO: 观察 size 与 capacity 的变化
    // TODO: 使用 clear 清空所有元素，观察 capacity 是否改变
}

// ---------- main.cpp ----------
#include "GradeBook.h"
#include "VectorExercises.h"

int main() {
    // TODO: 调用以上函数与 GradeBook 进行测试
    // 提示: 依次测试 reversePrint、statistics、removeElement、emplaceInsertClear、GradeBook
    return 0;
}
```

</details>

## 1.2 string 字符串处理

- **练习目标**：彻底替代 C 风格字符串；掌握拼接、查找、截取、比较；理解 `std::string` 不是裸指针。
- **练习任务**：
    1. 输入一句话，统计单词数量。
    2. 将句子中所有空格替换为 `%20`。
    3. 判断字符串是否为回文。
    4. 读取多行文本，找出出现频率最高的单词。
    5. 将 `"123,456,789"` 按逗号分割为多个子串。
- **巩固标准**：
  - [ ] 不再习惯性使用 `char[]`、`strcpy`、`strcat`。
    > **知识讲解**：C 风格字符串（`char[]`）需要手动管理内存、计算长度，且 `strcpy`/`strcat` 不检查缓冲区边界，容易导致缓冲区溢出。`std::string` 自动管理内存，提供 `size()`/`length()` 获取长度，`+`/`+=` 拼接，`find()`/`substr()` 查找截取，所有操作都有边界安全保障。养成"默认用 `std::string`，只在对接 C API 时才用 `c_str()`"的习惯。
  - [ ] 自然使用 `find`、`substr`、`s +=` 等操作。
    > **知识讲解**：`find(sub)` 返回子串首次出现的位置（未找到返回 `std::string::npos`），可配合 `substr(pos, len)` 截取任意片段；`s += "xxx"` 是最高效的拼接方式之一（原地追加，避免创建临时对象）；`s.insert(pos, str)` 在指定位置插入；`s.replace(pos, len, str)` 替换指定范围内容。这些操作都基于 `size_t` 位置索引，比手动操作指针安全直观得多。
  - [ ] 理解 SSO（Small String Optimization）对性能的影响。
    > **知识讲解**：SSO 是 `std::string` 的底层优化机制——string 对象内部预留一块固定大小的栈缓冲区（通常 15~22 字节，取决于标准库实现）。当字符串长度 ≤ 缓冲区大小时，数据直接存在栈上，**零堆分配**；超过时才退回到堆上 `new`/`delete`。这意味着短字符串（如 `"hello"`、变量名、短命令等）的构造、拷贝、析构几乎无额外开销。`sizeof(std::string)` 通常为 32 字节（64 位系统），比直觉中的"指针+大小+容量=24 字节"更大，多出的空间正是给 SSO 用的。在高频操作短字符串的场景（解析、哈希表 key、日志拼接）中，SSO 是 `std::string` 性能远超 C 风格字符串手动 `malloc` 的关键原因。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 1.2 string 练习框架 ===
// 项目结构:
// 1-2-string/
// ├── StringExercises.h
// ├── StringExercises.cpp
// └── main.cpp

// ---------- StringExercises.h ----------
#pragma once
#include <string>
#include <vector>

// 练习1: 输入一句话，统计单词数量
int countWords(const std::string& sentence);

// 练习2: 将句子中所有空格替换为%20
std::string replaceSpaces(std::string s);

// 练习3: 判断字符串是否为回文
bool isPalindrome(const std::string& s);

// 练习4: 读取多行文本，找出出现频率最高的单词
std::string mostFrequentWord();

// 练习5: 将"123,456,789"按逗号分割为多个子串
std::vector<std::string> split(const std::string& s, char delim);

// ---------- StringExercises.cpp ----------
#include "StringExercises.h"

#include <sstream>
#include <unordered_map>

int countWords(const std::string& sentence) {
    // TODO: 用 std::istringstream 分割单词，计数并返回
    return 0;
}

std::string replaceSpaces(std::string s) {
    // TODO: 遍历 s，遇到空格替换为 "%20"
    return s;
}

bool isPalindrome(const std::string& s) {
    // TODO: 双指针从两端向中间逼近，逐字符比较
    return false;
}

std::string mostFrequentWord() {
    std::unordered_map<std::string, int> freq;
    std::string word;
    // TODO: 循环读取每个单词，freq[word]++
    // TODO: 遍历 freq 找出出现次数最多的单词
    return "";
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    // TODO: 用 find + substr 或 std::istringstream 按 delim 分割
    return result;
}

// ---------- main.cpp ----------
#include "StringExercises.h"

int main() {
    // TODO: 调用以上函数进行测试
    return 0;
}
```

</details>

## 1.3 map / unordered_map 关联容器

- **练习目标**：理解键值对存储；区分有序 `map`（红黑树）与无序 `unordered_map`（哈希表）；掌握插入、查找、遍历。
- **练习任务**：
    1. 统计文章中每个单词的出现次数。
    2. 输入学生姓名与分数，实现按姓名查询。
    3. 统计数组中每个数字的出现频次。
    4. 实现简易电话本：添加、删除、查询、打印所有联系人。
    5. 解析日志文件，统计每个用户的登录次数。
- **巩固标准**：
  - [ ] 能根据场景正确选择 `map` 或 `unordered_map`。
    > **知识讲解**：`std::map` 底层是红黑树，元素按 key 有序排列，插入/查找/删除时间复杂度均为 O(log n)，适合需要有序遍历、范围查询（`lower_bound`/`upper_bound`）的场景。`std::unordered_map` 底层是哈希表，元素无序存储，平均插入/查找/删除为 O(1)，但最坏情况（哈希冲突严重）退化为 O(n)；且 `rehash` 时代价较高。选择原则：**需要有序或范围操作时用 `map`，其余场景优先用 `unordered_map`**（平均性能更优）。注意 `unordered_map` 的 key 类型需要提供 `std::hash` 特化或自定义哈希函数。
  - [ ] 熟练使用 `m[key]++`、`m.find()`、`m.at()`。
    > **知识讲解**：`m[key]` 会在 key 不存在时**自动插入**一个值初始化的元素（对 `int` 即 0），因此 `m[key]++` 是统计词频的经典写法——首次访问自动初始化为 0 再自增。但这也意味着只读查询时误用 `m[key]` 会意外插入无用键值对，此时应使用 `m.find(key)`（返回迭代器，未找到返回 `m.end()`，不插入）或 `m.at(key)`（找到返回值引用，未找到抛出 `std::out_of_range` 异常）。总结：计数用 `m[key]++`，安全查询用 `m.find()` 或 `m.at()`，三者配合覆盖所有常见场景。
  - [ ] 能用范围 for + 结构化绑定（C++17）遍历键值对。
    > **知识讲解**：C++17 引入的结构化绑定（Structured Bindings）允许 `for (const auto& [key, value] : m)` 直接解构 `map`/`unordered_map` 中的 `std::pair<const Key, Value>`，比 C++11 的 `it->first`/`it->second` 或 `auto& p; p.first` 更直观易读。注意迭代变量应使用 `const auto&` 避免不必要的拷贝（尤其是 value 为 `std::string` 等复杂类型时）；若需修改 value，去掉 `const` 即可（`for (auto& [key, value] : m)`），但 key 始终为 `const` 不可修改。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 1.3 map/unordered_map 练习框架 ===
// 项目结构:
// 1-3-map/
// ├── StudentLookup.h
// ├── StudentLookup.cpp
// ├── PhoneBook.h
// ├── PhoneBook.cpp
// ├── MapExercises.h
// ├── MapExercises.cpp
// └── main.cpp

// ---------- StudentLookup.h ----------
#pragma once
#include <string>
#include <unordered_map>

// 练习2: 学生姓名 → 分数，按姓名查询
class StudentLookup {
public:
    void add(const std::string& name, int score);      // TODO
    int query(const std::string& name) const;          // TODO: 找到返回分数，未找到返回 -1

private:
    std::unordered_map<std::string, int> records_;
};

// ---------- StudentLookup.cpp ----------
#include "StudentLookup.h"

void StudentLookup::add(const std::string& name, int score) {
    // TODO: records_[name] = score
}

int StudentLookup::query(const std::string& name) const {
    // TODO: records_.find(name)，命中返回分数，否则返回 -1
    return -1;
}

// ---------- PhoneBook.h ----------
#pragma once
#include <string>
#include <unordered_map>

// 练习4: 简易电话本
class PhoneBook {
public:
    void add(const std::string& name, const std::string& phone);   // TODO
    void remove(const std::string& name);                          // TODO
    std::string lookup(const std::string& name) const;             // TODO
    void printAll() const;                                         // TODO: 遍历打印所有联系人

private:
    std::unordered_map<std::string, std::string> contacts_;        // name -> phone
};

// ---------- PhoneBook.cpp ----------
#include "PhoneBook.h"
#include <iostream>

void PhoneBook::add(const std::string& name, const std::string& phone) {
    // TODO: contacts_[name] = phone
}

void PhoneBook::remove(const std::string& name) {
    // TODO: contacts_.erase(name)
}

std::string PhoneBook::lookup(const std::string& name) const {
    // TODO: 查找并返回，未找到返回空字符串
    return "";
}

void PhoneBook::printAll() const {
    // TODO: for (const auto& [name, phone] : contacts_) 打印
}

// ---------- MapExercises.h ----------
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

// 练习1: 统计文章中每个单词出现次数
std::unordered_map<std::string, int> wordCount(const std::string& text);

// 练习3: 统计数组中每个数字的出现频次
std::unordered_map<int, int> countFrequency(const std::vector<int>& nums);

// 练习5: 解析日志，统计每个用户登录次数
std::unordered_map<std::string, int> countLogins(const std::vector<std::string>& logLines);

// ---------- MapExercises.cpp ----------
#include "MapExercises.h"
#include <sstream>

std::unordered_map<std::string, int> wordCount(const std::string& text) {
    std::unordered_map<std::string, int> freq;
    // TODO: 逐词读取，freq[word]++
    return freq;
}

std::unordered_map<int, int> countFrequency(const std::vector<int>& nums) {
    std::unordered_map<int, int> freq;
    // TODO: 遍历 nums，统计频次
    return freq;
}

std::unordered_map<std::string, int> countLogins(const std::vector<std::string>& logLines) {
    std::unordered_map<std::string, int> loginCount;
    // TODO: 解析每行提取用户名，loginCount[user]++
    // 日志格式: "[2024-01-15 10:30:00] user:alice login"
    // 提示: 找到 "user:" 前缀，截取到下一个空格之间的部分即为用户名
    return loginCount;
}

// ---------- main.cpp ----------
#include "MapExercises.h"
#include "PhoneBook.h"
#include "StudentLookup.h"

int main() {
    // TODO: 调用以上函数与类进行测试
    return 0;
}
```

</details>

## 1.4 set / unordered_set 集合容器

- **练习目标**：理解自动去重与有序/无序特性；掌握插入、查找、删除操作。
- **练习任务**：
    1. 输入一串数字，去重后升序输出。
    2. 判断两个数组是否存在交集。
    3. 统计文章中不同单词的数量。
    4. 实现黑名单系统：加入、移除、判断 ID 是否在列。
    5. 从一组数字中找出前 K 个不重复的数。
- **巩固标准**：
  - [ ] 能准确区分 `vector`、`map`、`set` 的适用边界。
    > **知识讲解**：`vector` 适合有序索引访问和顺序存储场景，支持随机访问 O(1)，但不保证唯一性；`map` 适合键值对映射场景（如统计词频、按名字查分数），key 自动有序且唯一；`set` 适合纯集合操作（去重、交集、并集、成员判定），元素自动有序且唯一。选择原则：**需要“存在性判定+自动去重”用 `set`，需要“键→值”映射用 `map`，需要顺序存储或按位置访问用 `vector`**。若不需要有序性，优先选对应的 `unordered_` 版本以获得均摊 O(1) 性能。
  - [ ] 不再用数组+手动去重解决集合类问题。
    > **知识讲解**：传统 C 风格做法是用数组存所有元素，再遍历一遍跳过重复值（通常需要先排序）。这种方式代码冗长、容易出错，且时间复杂度为 O(n log n)（排序）+ O(n)（去重）。使用 `std::set` 或 `std::unordered_set` 可以在插入时自动去重——`set` 插入时检查是否已存在，重复元素直接忽略，代码只需一行 `s.insert(x)`。`set` 版本复杂度为 O(n log n)，`unordered_set` 为均摊 O(n)，且代码更简洁、语义更清晰。
  - [ ] 理解 `count` 与 `find` 在语义和性能上的区别。
    > **知识讲解**：`s.count(x)` 返回元素 x 在集合中的出现次数——对 `set`/`unordered_set` 结果只能是 0 或 1（元素唯一），对 `multiset`/`multimap` 则可能大于 1。`s.find(x)` 返回指向 x 的迭代器（未找到返回 `s.end()`），可以直接用于访问元素或作为删除操作的参数。性能上两者相同（都是 O(log n) 或均摊 O(1)），但语义不同：**只需判断“是否存在”用 `count`（返回布尔语义的 0/1），需要获取元素或做后续操作时用 `find`（返回迭代器）**。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 1.4 set/unordered_set 练习框架 ===
// 项目结构:
// 1-4-set/
// ├── Blacklist.h
// ├── Blacklist.cpp
// ├── SetExercises.h
// ├── SetExercises.cpp
// └── main.cpp

// ---------- Blacklist.h ----------
#pragma once
#include <unordered_set>

// 练习4: 黑名单系统
class Blacklist {
public:
    void add(int id);                  // TODO
    void remove(int id);               // TODO
    bool contains(int id) const;       // TODO

private:
    std::unordered_set<int> ids_;
};

// ---------- Blacklist.cpp ----------
#include "Blacklist.h"

void Blacklist::add(int id) {
    // TODO: ids_.insert(id)
}

void Blacklist::remove(int id) {
    // TODO: ids_.erase(id)
}

bool Blacklist::contains(int id) const {
    // TODO: return ids_.count(id) > 0  或  ids_.find(id) != ids_.end()
    return false;
}

// ---------- SetExercises.h ----------
#pragma once
#include <set>
#include <string>
#include <vector>

// 练习1: 输入一串数字，去重后升序输出
std::set<int> deduplicateAndSort(const std::vector<int>& nums);

// 练习2: 判断两个数组是否存在交集
bool hasIntersection(const std::vector<int>& a, const std::vector<int>& b);

// 练习3: 统计文章中不同单词的数量
int uniqueWordCount(const std::string& text);

// 练习5: 从一组数字中找出前K个不重复的数
std::vector<int> topKUnique(const std::vector<int>& nums, int k);

// ---------- SetExercises.cpp ----------
#include "SetExercises.h"
#include <sstream>
#include <unordered_set>

std::set<int> deduplicateAndSort(const std::vector<int>& nums) {
    std::set<int> result;
    // TODO: 将 nums 元素 insert 到 set，自动去重 + 升序
    return result;
}

bool hasIntersection(const std::vector<int>& a, const std::vector<int>& b) {
    // TODO: 将 a 放入 std::unordered_set，遍历 b 检查是否存在于 set 中
    return false;
}

int uniqueWordCount(const std::string& text) {
    std::unordered_set<std::string> words;
    // TODO: 逐词提取并 insert 到 set
    return static_cast<int>(words.size());
}

std::vector<int> topKUnique(const std::vector<int>& nums, int k) {
    // TODO: 用 std::unordered_set 去重，取前 k 个
    return {};
}

// ---------- main.cpp ----------
#include "Blacklist.h"
#include "SetExercises.h"

int main() {
    // TODO: 调用以上函数与 Blacklist 进行测试
    return 0;
}
```

</details>

## 1.5 标准算法与 Lambda

- **练习目标**：会用 `sort/find/for_each` 等标准算法；能配合 Lambda 表达式自定义行为；理解迭代器区间概念。
- **练习任务**：
    1. 对 `vector<int>` 进行升序、降序排序。
    2. 对学生结构体按分数排序。
    3. 在 `vector` 中查找某元素是否存在。
    4. 用 `for_each` + Lambda 打印容器所有元素。
    5. 用 `sort` + Lambda 实现：按字符串长度排序、按绝对值排序、自定义复合规则排序。
- **巩固标准**：
  - [ ] 能用标准算法替代手写循环完成常见任务。
    > **知识讲解**：手写循环需要自己维护下标、边界和终止条件，容易写错（如 `<=` 越界、忘记 `break`），而标准算法把“遍历”与“做什么”解耦：算法负责遍历策略，你只提供一个谓词/操作（Lambda）。常用对应关系——排序用 `std::sort(begin, end)`，查找用 `std::find(begin, end, value)` / `std::find_if(begin, end, pred)`，遍历处理用 `std::for_each`，计数用 `std::count_if`，最大值用 `std::max_element`，条件拷贝用 `std::copy_if`。所有算法都基于迭代器区间 `[begin, end)`（左闭右开，`end` 不参与运算），因此同一份算法代码可作用于 `vector`、`deque`、原生数组等任意随机访问容器。标准算法内部往往有向量化、循环展开等优化，通常比手写循环更快，且代码意图一目了然。
  - [ ] 能正确书写 `[=]`、`[&]`、`[x]`、`[&x]` 等捕获列表并理解其生命周期风险。
    > **知识讲解**：捕获列表决定 Lambda 如何“看见”外部变量。`[x]` 按值捕获，创建一份副本，外部变量后续变化不影响 Lambda；`[&x]` 按引用捕获，不复制、可修改外部变量，但引用必须始终有效；`[=]` 隐式按值捕获所有用到的外部变量；`[&]` 隐式按引用捕获。核心风险有两类：**其一，悬垂引用**——把按引用捕获的 Lambda 存起来延迟使用（如放进 `std::function` 成员、传给异步线程、作为回调注册），一旦原变量离开作用域，调用即未定义行为；`[=]` 捕获局部对象（如 `std::string`）在异步场景相对安全，但仍需注意其内部指针指向的资源。**其二，`[=]` 捕获 `this` 的陷阱**——在成员函数中 `[=]` 实际捕获的是 `this` 指针（C++17 起有弃用警告），对象析构后调用 Lambda 同样悬垂，应显式写 `[self = *this]` 复制对象本身。经验法则：**同步、就地执行的算法（`sort`/`for_each`/`find_if`）用 `[&]` 最省心；Lambda 会存活超过当前作用域时，一律按值捕获所需数据，绝不捕获引用或裸 `this`**。
  - [ ] 理解算法的复杂度保证。
    > **知识讲解**：标准库对算法复杂度有明确约定，这是选择算法的重要依据。`std::sort` 平均 O(n log n)（内省排序：快排 + 堆排 + 插入排序混合），且**要求随机访问迭代器**，所以 `std::list` 不能用 `std::sort`，得用其成员函数 `list::sort`。`std::stable_sort` 保持相等元素相对顺序，有足够内存时 O(n log n)，否则 O(n log² n)。`std::find`/`std::find_if`/`std::for_each`/`std::count_if` 是线性 O(n)，对**有序**区间应改用二分查找 `std::binary_search` / `std::lower_bound` / `std::upper_bound`，复杂度降为 O(log n)。`std::distance` 对随机访问迭代器是 O(1)，对 `list`/`map` 的前向迭代器是 O(n)。注意 `map`/`set` 是关联容器，其自身 `find` 是 O(log n)（红黑树），**不要**对它们调用需要随机访问迭代器的算法。另外“比较次数”与“交换/拷贝次数”是两个维度：元素体积大时 `std::sort` 的移动开销可能超过比较开销，此时可考虑排序索引（`vector<size_t>`）再间接访问，或用 `stable_sort` 减少移动。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 1.5 标准算法与Lambda 练习框架 ===
// 项目结构:
// 1-5-algorithm/
// ├── Student.h
// ├── AlgorithmExercises.h
// ├── AlgorithmExercises.cpp
// └── main.cpp

// ---------- Student.h ----------
#pragma once
#include <string>

struct Student {
    std::string name;
    int score = 0;
};

// ---------- AlgorithmExercises.h ----------
#pragma once
#include <vector>

#include "Student.h"

// 练习1: vector<int> 升序、降序排序
void sortDemo(std::vector<int>& v);

// 练习2: 学生结构体按分数排序
void sortStudents(std::vector<Student>& students);

// 练习3: 在vector中查找某元素
bool contains(const std::vector<int>& v, int target);

// 练习4: for_each + Lambda 打印容器所有元素
void printAll(const std::vector<int>& v);

// 练习5: 多种Lambda排序
void customSorts(std::vector<int>& v);

// ---------- AlgorithmExercises.cpp ----------
#include "AlgorithmExercises.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>

void sortDemo(std::vector<int>& v) {
    // TODO: std::sort(v.begin(), v.end()) 升序
    // TODO: std::sort(v.begin(), v.end(), std::greater<int>{}) 降序
}

void sortStudents(std::vector<Student>& students) {
    // TODO: std::sort + Lambda，按 score 降序
}

bool contains(const std::vector<int>& v, int target) {
    // TODO: 用 std::find 或 std::find_if 查找
    return false;
}

void printAll(const std::vector<int>& v) {
    // TODO: std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << ' '; });
}

void customSorts(std::vector<int>& v) {
    // TODO: 按绝对值排序（std::abs）
    // TODO: 按字符串长度排序（需 std::vector<std::string>）
}

// ---------- main.cpp ----------
#include "AlgorithmExercises.h"

int main() {
    // TODO: 调用以上函数进行测试
    return 0;
}
```

</details>

## 1.6 STL 综合项目（三选一）

- **项目 A：学生成绩管理系统**
  - 功能：增删改查、按成绩排序、统计均分/最高/最低。
  - 数据结构：`vector<Student>` + `algorithm`。
- **项目 B：单词频率统计器**
  - 功能：读取文本、词频统计、Top10 输出、忽略大小写。
  - 数据结构：`unordered_map<string, int>` + `vector<pair>` 排序。
- **项目 C：简易任务管理器**
  - 功能：添加/完成/删除任务、显示未完成列表、按优先级排序。
  - 数据结构：`vector<Task>` + Lambda 排序。
- **验收标准**：独立完成全部功能，代码无内存泄漏，逻辑清晰可读。

<details>
<summary>📦 项目框架代码（三选一）</summary>

```cpp
// === 1.6 STL 综合项目（三选一） ===
// 以下三个项目任选其一完成，项目结构已按现代 C++ 风格拆分。

// ================================================================
// 项目 A：1-6-a-grade-manager/
//   ├── Student.h
//   ├── GradeManager.h
//   ├── GradeManager.cpp
//   └── main.cpp
// ================================================================

// ---------- Student.h ----------
#pragma once
#include <string>

struct Student {
    std::string name;
    double score = 0.0;
};

// ---------- GradeManager.h ----------
#pragma once
#include <string>
#include <vector>

#include "Student.h"

class GradeManager {
public:
    void add(const std::string& name, double score);       // TODO
    void remove(const std::string& name);                   // TODO
    Student* find(const std::string& name);                 // TODO
    void updateScore(const std::string& name, double s);    // TODO
    void sortByScore();                                     // TODO: std::sort + Lambda
    double average() const;                                 // TODO
    double highest() const;                                 // TODO
    double lowest() const;                                  // TODO
    void printAll() const;                                  // TODO

private:
    std::vector<Student> students_;
};

// ---------- GradeManager.cpp ----------
#include "GradeManager.h"

#include <algorithm>
#include <iostream>
#include <numeric>

// TODO: 实现以上成员函数

// ---------- main.cpp ----------
#include "GradeManager.h"

int main() {
    // TODO: 实例化 GradeManager 并测试全部功能
    return 0;
}

// ================================================================
// 项目 B：1-6-b-word-frequency/
//   ├── WordFrequency.h
//   ├── WordFrequency.cpp
//   └── main.cpp
// ================================================================

// ---------- WordFrequency.h ----------
#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class WordFrequency {
public:
    void loadFile(const std::string& filename);                          // TODO: 逐词读取，转小写，freq_[word]++
    std::vector<std::pair<std::string, int>> top10() const;              // TODO: 拷贝到 vector，排序取前 10
    void print() const;                                                  // TODO

private:
    std::unordered_map<std::string, int> freq_;
};

// ---------- WordFrequency.cpp ----------
#include "WordFrequency.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

// TODO: 实现以上成员函数

// ---------- main.cpp ----------
#include "WordFrequency.h"

int main() {
    // TODO: 实例化 WordFrequency 并测试全部功能
    return 0;
}

// ================================================================
// 项目 C：1-6-c-task-manager/
//   ├── Task.h
//   ├── TaskManager.h
//   ├── TaskManager.cpp
//   └── main.cpp
// ================================================================

// ---------- Task.h ----------
#pragma once
#include <string>

struct Task {
    int id = 0;
    std::string title;
    int priority = 1;     // 1=低, 2=中, 3=高
    bool done = false;
};

// ---------- TaskManager.h ----------
#pragma once
#include <string>
#include <vector>

#include "Task.h"

class TaskManager {
public:
    void add(const std::string& title, int priority);   // TODO
    void complete(int id);                              // TODO: 标记 done=true
    void remove(int id);                                // TODO
    void showPending() const;                           // TODO: 打印未完成列表
    void sortByPriority();                              // TODO: std::sort + Lambda

private:
    std::vector<Task> tasks_;
    int nextId_ = 1;
};

// ---------- TaskManager.cpp ----------
#include "TaskManager.h"

#include <algorithm>
#include <iostream>

// TODO: 实现以上成员函数

// ---------- main.cpp ----------
#include "TaskManager.h"

int main() {
    // TODO: 实例化 TaskManager 并测试全部功能
    return 0;
}
```

</details>

---

# 阶段二：现代 C++ 特性

本阶段核心目标是从 C 风格资源管理过渡到现代 C++ 的自动管理与简洁表达，重点掌握 RAII、移动语义与智能指针。

## 2.1 auto 类型推导

- **练习目标**：减少冗余类型书写；理解 `auto` 是编译期推导而非运行时万能类型。
- **练习任务**：
    1. 将显式迭代器声明改为 `auto`。
    2. 遍历 `map` 时使用 `auto` 接收键值对。
    3. 函数返回 `vector<int>`，调用方用 `auto` 接收。
- **巩固标准**：
  - [ ] 能用 `auto` 简化复杂类型，但不滥用导致可读性下降。
    > **知识讲解**：`auto` 的最大价值是消除冗长类型名——如 `std::map<std::string, std::vector<int>>::iterator` 可简写为 `auto`，既减少敲键又避免类型写错，容器类型改变时也无需同步修改声明。但滥用会损害可读性：`auto x = compute();` 让读者无法一眼看出 `x` 的类型，需要跳转定义。经验法则——**迭代器、Lambda、模板返回、以及从右侧初始化表达式即可直观推断类型时用 `auto`；当右侧是返回类型不直观的函数调用、或需要显式表达数值类型（`double d = 3.0;` 比 `auto d = 3.0;` 更明确）时，写明类型更清晰**。此外 `auto` 会自动 decay 掉引用与顶层 const，`auto x = ref;` 得到的是副本而非引用。
  - [ ] 理解 `auto`、`auto&`、`const auto&`、`auto&&` 的推导差异。
    > **知识讲解**：四者的核心区别在于是否拷贝、是否可修改。`auto x`——按值推导，创建副本，忽略引用与顶层 const，修改 x 不影响原对象；`auto& x`——左值引用，绑定原对象、可修改，但**不能绑定右值/临时对象**；`const auto& x`——只读引用，既避免拷贝又禁止修改，是遍历大对象（`std::string`、容器）的首选；`auto&& x`——万能引用（转发引用），既能绑定左值也能绑定右值，常用于泛型编程与完美转发 `std::forward`。陷阱：`auto` 推导时会丢失引用，想保留需显式写 `auto&`；对代理类型（如 `vector<bool>::reference`）用 `auto` 可能得到非预期类型，此时应改用 `decltype(auto)`。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 2.1 auto 练习框架 ===
// 项目结构:
// 2-1-auto/
// ├── AutoExercises.h
// ├── AutoExercises.cpp
// └── main.cpp

// ---------- AutoExercises.h ----------
#pragma once
#include <map>
#include <string>
#include <vector>

// 练习1: 将显式迭代器改为 auto
void autoIteratorDemo(const std::vector<int>& v);

// 练习2: 遍历 map 时使用 auto 接收键值对
void autoMapDemo(const std::map<std::string, int>& m);

// 练习3: 函数返回 vector<int>，调用方用 auto 接收
std::vector<int> generateData(int n);

// ---------- AutoExercises.cpp ----------
#include "AutoExercises.h"
#include <iostream>

void autoIteratorDemo(const std::vector<int>& v) {
    // TODO: 用 auto it = v.begin() 遍历打印
    // 对比: std::vector<int>::iterator it = v.begin();
}

void autoMapDemo(const std::map<std::string, int>& m) {
    // TODO: for (const auto& [key, val] : m) 打印键值对
}

std::vector<int> generateData(int n) {
    std::vector<int> result;
    // TODO: 填充 1~n 并返回
    return result;
}

// ---------- main.cpp ----------
#include "AutoExercises.h"
#include <iostream>

int main() {
    // TODO: auto data = generateData(10); 并打印
    return 0;
}
```

</details>

## 2.2 范围 for 循环

- **练习目标**：用 `for (auto x : container)` 安全遍历；理解值拷贝与引用修改的区别。
- **练习任务**：
    1. 遍历 `vector<int>` 并打印。
    2. 用范围 for 将所有元素乘以 2（需引用）。
    3. 遍历 `map` 打印键值对。
    4. 遍历 `vector<string>` 统计总字符数（需 const 引用避免拷贝）。
- **巩固标准**：
  - [ ] 能根据只读/修改/大对象场景正确选择 `int x`、`int& x`、`const auto& x`。
    > **知识讲解**：范围 for 的循环变量形式直接决定性能与语义。`for (int x : v)`——按值拷贝，适合基础类型（int/double/指针，拷贝成本可忽略）且只读或仅需修改副本的场景；`for (int& x : v)`——引用，用于**需要修改容器内元素**（如 `x *= 2`）；`for (const auto& x : v)`——const 引用，用于**只读遍历大对象**（`std::string`、自定义类、嵌套容器），既避免每次拷贝构造的开销，又防止意外修改。选择原则：**基础类型用值，需要修改用 `auto&`，只读大对象用 `const auto&`**。误用 `for (auto x : bigObjVec)` 会对每个元素执行拷贝构造，在元素体积大或数量多时造成明显性能损失。
  - [ ] 知道范围 for 不能用于需要修改容器结构（如删除元素）的场景。
    > **知识讲解**：范围 for 本质是 `for (auto it = begin; it != end; ++it)` 的语法糖，它在循环开始时缓存了 `end()` 迭代器。若在循环体内 `push_back`/`insert`/`erase` 改变容器结构，会导致：① `vector` 扩容后所有迭代器失效，缓存的 `end` 与 `it` 变成悬垂，行为未定义；② 即使不扩容，删除元素也会使被删位置之后的迭代器失效。因此**需要增删元素时不能用范围 for**，应改用显式迭代器循环并利用 `erase` 的返回值更新迭代器（`it = v.erase(it)`），或使用 erase-remove 惯用法。范围 for 只适合“遍历并读取/修改元素值”，不适合“修改容器大小”。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 2.2 范围for 练习框架 ===
// 项目结构:
// 2-2-range-for/
// ├── RangeForExercises.h
// ├── RangeForExercises.cpp
// └── main.cpp

// ---------- RangeForExercises.h ----------
#pragma once
#include <map>
#include <string>
#include <vector>

// 练习1: 遍历 vector<int> 并打印
void printVector(const std::vector<int>& v);

// 练习2: 范围 for 将所有元素乘以 2（需引用）
void doubleElements(std::vector<int>& v);

// 练习3: 遍历 map 打印键值对
void printMap(const std::map<std::string, int>& m);

// 练习4: 遍历 vector<string> 统计总字符数（const 引用避免拷贝）
int totalChars(const std::vector<std::string>& v);

// ---------- RangeForExercises.cpp ----------
#include "RangeForExercises.h"
#include <iostream>

void printVector(const std::vector<int>& v) {
    // TODO: for (int x : v) 打印每个元素
}

void doubleElements(std::vector<int>& v) {
    // TODO: for (int& x : v) x *= 2;
}

void printMap(const std::map<std::string, int>& m) {
    // TODO: for (const auto& [key, val] : m) 打印
}

int totalChars(const std::vector<std::string>& v) {
    int sum = 0;
    // TODO: for (const auto& s : v) sum += static_cast<int>(s.size());
    return sum;
}

// ---------- main.cpp ----------
#include "RangeForExercises.h"

int main() {
    // TODO: 调用以上函数进行测试
    return 0;
}
```

</details>

## 2.3 Lambda 表达式

- **练习目标**：理解匿名函数与捕获列表；能配合算法使用；理解闭包的生命周期。
- **练习任务**：
    1. 写 Lambda 计算两数之和。
    2. 用 Lambda 给 `vector<int>` 降序排序。
    3. 用 Lambda 筛选 `vector` 中所有偶数。
    4. 捕获外部阈值变量，统计大于该值的元素个数。
    5. 用 `sort` + Lambda 对学生按分数排序。
- **巩固标准**：
  - [ ] 能正确区分值捕获与引用捕获的使用场景。
    > **知识讲解**：值捕获 `[x]`/`[=]` 在 Lambda 创建时拷贝一份外部变量，之后与外部解耦——外部再修改也不影响 Lambda 内的副本；引用捕获 `[&x]`/`[&]` 不拷贝，Lambda 内直接操作外部变量本身，可修改且零开销。**选择依据是 Lambda 的生命周期**：若 Lambda 就地同步执行（如传给 `std::sort`/`for_each`/`find_if`，调用返回前即销毁），引用捕获 `[&]` 最简洁高效；若 Lambda 会被存储、异步执行或作为回调延迟调用（放入 `std::function`、传给线程、注册事件），**必须值捕获**，否则原变量离开作用域后引用悬垂，调用即未定义行为。此外 `[=]` 在成员函数中捕获的是 `this` 指针而非成员副本，需警惕对象生命周期。
  - [ ] 理解 Lambda 本质是仿函数对象，可作为参数传递或存储。
    > **知识讲解**：编译器会为每个 Lambda 生成一个匿名类（闭包类型 closure type），捕获的变量成为该类的成员，`operator()` 就是 Lambda 体——因此 Lambda 本质是重载了 `operator()` 的仿函数（functor）对象。这解释了它的行为：① 每个 Lambda 有独一无二的类型，无法直接写出类型名，只能用 `auto` 接收；② 可作为参数传递给接受模板/仿函数的算法；③ 需要存储或跨类型传递时用 `std::function<返回类型(参数...)>` 做类型擦除包装（但有堆分配与调用开销）；④ 默认 `operator()` 是 `const` 的，值捕获的成员不可修改，需加 `mutable` 才能改；⑤ 无捕获的 Lambda 可隐式转换为普通函数指针，能传给 C API。
  - [ ] 避免在 Lambda 中悬挂引用。
    > **知识讲解**：悬挂引用是 Lambda 最隐蔽的 bug，典型场景有三类：① **异步/线程**——把 `[&]` 捕获的 Lambda 交给 `std::thread` 并 `detach`，或提交到线程池，主线程作用域结束后局部变量已销毁，线程再访问即悬垂；② **循环变量引用**——在循环中创建 `[&]` Lambda 并存入容器，所有 Lambda 引用同一个循环变量，循环结束后变量失效且值也不对；③ **返回 Lambda**——函数返回一个按引用捕获局部变量的 Lambda，调用方拿到时局部变量已析构。规避原则：**只要 Lambda 的存活时间可能超过捕获变量的作用域，就一律改用值捕获**（拷贝所需数据），对 `this` 则用 `[self = *this]` 或 C++14 初始化捕获复制必要成员。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 2.3 Lambda 练习框架 ===
// 项目结构:
// 2-3-lambda/
// ├── Student.h
// ├── LambdaExercises.h
// ├── LambdaExercises.cpp
// └── main.cpp

// ---------- Student.h ----------
#pragma once
#include <string>

struct Student {
    std::string name;
    int score = 0;
};

// ---------- LambdaExercises.h ----------
#pragma once
#include <vector>

#include "Student.h"

// 练习2: Lambda 降序排序
void sortDescending(std::vector<int>& v);

// 练习3: Lambda 筛选偶数
std::vector<int> filterEven(const std::vector<int>& v);

// 练习4: 捕获外部阈值，统计大于该值的元素个数
int countAbove(const std::vector<int>& v, int threshold);

// 练习5: sort + Lambda 对学生按分数排序
void sortStudents(std::vector<Student>& students);

// ---------- LambdaExercises.cpp ----------
#include "LambdaExercises.h"

#include <algorithm>

void sortDescending(std::vector<int>& v) {
    // TODO: std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
}

std::vector<int> filterEven(const std::vector<int>& v) {
    std::vector<int> result;
    // TODO: 使用 std::copy_if + Lambda 筛选偶数，或遍历 v 手动 push_back
    return result;
}

int countAbove(const std::vector<int>& v, int threshold) {
    // TODO: 使用 std::count_if + Lambda，值捕获 threshold
    return 0;
}

void sortStudents(std::vector<Student>& students) {
    // TODO: std::sort + Lambda 按 score 降序
}

// ---------- main.cpp ----------
#include "LambdaExercises.h"
#include <iostream>

int main() {
    // 练习1: Lambda 计算两数之和
    // TODO: auto add = [](int a, int b) { return a + b; };

    // TODO: 调用以上函数进行测试
    return 0;
}
```

</details>

## 2.4 智能指针

- **练习目标**：理解 RAII 原则；减少手动 `new/delete`；掌握 `unique_ptr` 与 `shared_ptr`。
- **练习任务**：
    1. 用 `unique_ptr` 管理动态数组。
    2. 用 `shared_ptr` 管理共享对象，观察引用计数变化。
    3. 用 `unique_ptr` 实现链表节点，管理 next 指针。
    4. 将 `FILE*` 等 C 资源包装为智能指针（自定义删除器）。
    5. 写类在析构中打印信息，验证智能指针释放时机。
- **巩固标准**：
  - [ ] 默认优先使用 `unique_ptr`，仅在真正共享所有权时用 `shared_ptr`。
    > **知识讲解**：`unique_ptr` 表达“独占所有权”——同一时刻只有一个指针拥有对象，不可拷贝只能移动，析构时自动释放，**零额外开销**（大小与裸指针相同，无引用计数）。`shared_ptr` 表达“共享所有权”——内部维护控制块（引用计数 + weak 计数），每次拷贝都原子递增计数，析构递减，归零才释放，因此有**内存开销（控制块）与运行时开销（原子操作）**。原则：**默认用 `unique_ptr`，只有当确实需要多个所有者共享同一对象、且无法确定谁最后释放时才用 `shared_ptr`**。滥用 `shared_ptr` 会掩盖所有权设计问题，还带来循环引用风险。若只需“观察”对象而不拥有，用 `weak_ptr` 或裸指针/引用。
  - [ ] 知道 `make_unique/make_shared` 优于直接 `new`。
    > **知识讲解**：`std::make_unique<T>(args)`（C++14）和 `std::make_shared<T>(args)`（C++11）相比 `new` 有三大优势：① **异常安全**——`foo(std::shared_ptr<A>(new A), mayThrow())` 中若 `new A` 与 `shared_ptr` 构造之间调用 `mayThrow()` 抛异常，会泄漏；`make_shared` 一步完成，无此风险；② **性能**——`make_shared` 将对象与其控制块**一次性合并分配**在同一块内存，减少一次 `malloc`，缓存局部性更好；③ **简洁**——无需重复书写类型名。`make_shared` 的唯一代价是：只要有 `weak_ptr` 存活，控制块（含对象内存）就不会释放，对超大对象可能延迟内存回收。
  - [ ] 理解循环引用问题及 `weak_ptr` 解决方案。
    > **知识讲解**：`shared_ptr` 循环引用是经典内存泄漏——若 A 持有指向 B 的 `shared_ptr`，B 又持有指向 A 的 `shared_ptr`，两者引用计数永远 ≥1，即使外部指针全部销毁，计数也无法归零，对象永不释放。解决方案是**将其中一方（通常是“反向/从属”关系，如子指向父、双向链表的 prev 指针）改为 `weak_ptr`**。`weak_ptr` 引用对象但**不增加引用计数**，不拥有所有权；使用前必须 `lock()` 提升为 `shared_ptr`（若对象已释放则返回空），以此安全访问。设计准则：所有权关系应是有向无环的，用 `shared_ptr` 表达“拥有”，用 `weak_ptr` 表达“引用但不拥有”。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 2.4 智能指针练习框架 ===
// 项目结构:
// 2-4-smart-pointer/
// ├── ListNode.h
// ├── Watcher.h
// ├── Watcher.cpp
// ├── SmartPointerExercises.h
// ├── SmartPointerExercises.cpp
// └── main.cpp

// ---------- ListNode.h ----------
#pragma once
#include <memory>

// 练习3: unique_ptr 实现链表节点
struct ListNode {
    int val = 0;
    std::unique_ptr<ListNode> next;

    explicit ListNode(int v) : val(v), next(nullptr) {}
};

// ---------- Watcher.h ----------
#pragma once
#include <string>

// 练习5: 析构打印验证释放时机
struct Watcher {
    explicit Watcher(std::string n);
    ~Watcher();

    Watcher(const Watcher&) = delete;
    Watcher& operator=(const Watcher&) = delete;

    std::string name;
};

// ---------- Watcher.cpp ----------
#include "Watcher.h"
#include <iostream>
#include <utility>

Watcher::Watcher(std::string n) : name(std::move(n)) {
    std::cout << name << " created\n";
}

Watcher::~Watcher() {
    std::cout << name << " destroyed\n";
}

// ---------- SmartPointerExercises.h ----------
#pragma once

// 练习1: unique_ptr 管理动态数组
void uniqueArrayDemo();

// 练习2: shared_ptr 管理共享对象，观察引用计数
void sharedPtrDemo();

// 练习3: unique_ptr 实现链表
void linkedListDemo();

// 练习4: FILE* 包装为智能指针（自定义删除器）
void filePtrDemo();

// 练习5: 析构打印验证释放时机
void watchDemo();

// ---------- SmartPointerExercises.cpp ----------
#include "SmartPointerExercises.h"

#include <cstdio>
#include <iostream>
#include <memory>

#include "ListNode.h"
#include "Watcher.h"

void uniqueArrayDemo() {
    // TODO: auto arr = std::make_unique<int[]>(5);
    // TODO: 填充并打印数组
}

void sharedPtrDemo() {
    // TODO: auto sp = std::make_shared<int>(42);
    // TODO: 创建多个 shared_ptr 副本，打印 use_count()
}

void linkedListDemo() {
    // TODO: 用 std::unique_ptr<ListNode> 构建链表并遍历打印
}

void filePtrDemo() {
    // TODO: std::unique_ptr<FILE, decltype(&fclose)> fp(std::fopen("test.txt", "w"), &std::fclose);
}

void watchDemo() {
    // TODO: 用 std::unique_ptr / std::shared_ptr 管理 Watcher，观察析构时机
}

// ---------- main.cpp ----------
#include "SmartPointerExercises.h"

int main() {
    // TODO: 调用以上函数进行测试
    return 0;
}
```

</details>

## 2.5 右值引用与移动语义

- **练习目标**：理解左值/右值区分；掌握移动构造/赋值；理解性能优化原理。
- **练习任务**：
    1. 实现类的拷贝构造、拷贝赋值、移动构造、移动赋值四件套。
    2. 在构造/析构/移动函数中打印日志，观察对象生命周期。
    3. 用 `std::move` 转移 `vector/string` 内容。
    4. 对比拷贝与移动 `vector` 的性能差异。
    5. 实现简易 `MyString` 类，内部持有 `char*`，完整支持移动语义。
- **巩固标准**：
  - [ ] 理解 `std::move` 仅是类型转换，真正移动由移动构造函数执行。
    > **知识讲解**：`std::move` 本身**不移动任何东西**，它只是一个 `static_cast<T&&>`——把表达式无条件转换为右值引用，从而让编译器在重载决议时选中“移动构造/移动赋值”而非“拷贝”版本。真正的资源转移发生在移动构造函数的函数体里（通常是把源对象的指针“窃取”过来、再把源指针置空）。因此若一个类**没有定义移动构造函数**，`std::move(x)` 后仍会退化为拷贝（因为找不到匹配的移动重载，右值也能绑定到 `const T&` 拷贝构造），达不到优化效果。理解这点才能避免“以为 move 了就一定快”的误区。
  - [ ] 知道移动后源对象处于有效但未指定状态。
    > **知识讲解**：“有效但未指定”（valid but unspecified）意味着——移动后的源对象仍是合法对象，可以安全析构、可以重新赋值，但**其内部值不可预测，不应再读取或依赖其内容**。规范的移动构造函数应把源对象置于一个确定状态（如指针置 `nullptr`、size 置 0），保证析构时不会 double-free。实践中：移动后你可以对源对象执行“赋值新值”或“析构”，但不要 `std::cout << moved_from_str` 期望有意义的内容，也不要对其做依赖原值的逻辑判断。标准库容器（如 `std::string`、`std::vector`）移动后保证为空，但这是库的实现承诺，自定义类型需自己保证。
  - [ ] 能在性能敏感场景正确应用移动语义。
    > **知识讲解**：移动语义的核心价值是**避免深拷贝**——把 O(n) 的堆内存分配+逐字节复制降为 O(1) 的指针交换。典型受益场景：① **函数返回大对象**——返回局部 `vector`/`string` 时依赖移动（或 RVO/NRVO 直接省略拷贝），无需手写 `std::move`（反而可能抑制 RVO）；② **容器扩容与元素转移**——`vector` 扩容时若元素有 `noexcept` 移动构造，会用移动而非拷贝，因此**移动构造应标记 `noexcept`**，否则 `vector` 为保证异常安全退回拷贝；③ **将左值显式转移所有权**——如把不再使用的 `string` 存入容器用 `v.push_back(std::move(s))`。注意：对基础类型、小对象或即将析构的临时对象，移动无收益甚至多余。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 2.5 右值引用与移动语义练习框架 ===
// 项目结构:
// 2-5-move-semantics/
// ├── Resource.h
// ├── Resource.cpp
// ├── MyString.h
// ├── MyString.cpp
// ├── MoveExercises.h
// ├── MoveExercises.cpp
// └── main.cpp

// ---------- Resource.h ----------
#pragma once
#include <cstddef>

// 练习1&2: 四件套 + 日志打印
class Resource {
public:
    explicit Resource(std::size_t n);
    ~Resource();

    Resource(const Resource& other);                    // 拷贝构造（深拷贝）
    Resource& operator=(const Resource& other);         // 拷贝赋值
    Resource(Resource&& other) noexcept;                // 移动构造
    Resource& operator=(Resource&& other) noexcept;     // 移动赋值

    std::size_t size() const noexcept { return size_; }

private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
};

// ---------- Resource.cpp ----------
#include "Resource.h"
#include <iostream>
#include <utility>

Resource::Resource(std::size_t n) : data_(new int[n]{}), size_(n) {
    std::cout << "构造\n";
}

Resource::~Resource() {
    delete[] data_;
    std::cout << "析构\n";
}

Resource::Resource(const Resource& other) {
    // TODO: 深拷贝 other.data_ 到 data_
}

Resource& Resource::operator=(const Resource& other) {
    // TODO: 自赋值保护 + 释放旧资源 + 深拷贝
    return *this;
}

Resource::Resource(Resource&& other) noexcept {
    // TODO: 窃取 other.data_，并将 other 重置为安全空状态
}

Resource& Resource::operator=(Resource&& other) noexcept {
    // TODO: 自赋值保护 + 释放旧资源 + 窃取
    return *this;
}

// ---------- MyString.h ----------
#pragma once

// 练习5: MyString 类
class MyString {
public:
    MyString(const char* s);
    ~MyString();

    MyString(const MyString& other);                     // 拷贝构造
    MyString& operator=(const MyString& other);          // 拷贝赋值
    MyString(MyString&& other) noexcept;                 // 移动构造
    MyString& operator=(MyString&& other) noexcept;      // 移动赋值

    const char* c_str() const noexcept { return data_; }

private:
    char* data_ = nullptr;
};

// ---------- MyString.cpp ----------
#include "MyString.h"
#include <cstring>
#include <utility>

// TODO: 实现以上所有成员函数（注意 std::strlen + new char[n+1]）

// ---------- MoveExercises.h ----------
#pragma once

// 练习3: std::move 转移 vector/string
void moveDemo();

// 练习4: 对比拷贝与移动性能
void benchmark();

// ---------- MoveExercises.cpp ----------
#include "MoveExercises.h"

#include <chrono>
#include <iostream>
#include <utility>
#include <vector>

void moveDemo() {
    std::vector<int> v = {1, 2, 3};
    // TODO: auto v2 = std::move(v);
    // TODO: 打印 v 和 v2，观察 v 的状态
}

void benchmark() {
    std::vector<int> src(1000000, 1);
    // TODO: 使用 std::chrono 计时拷贝 vs 移动
}

// ---------- main.cpp ----------
#include "MoveExercises.h"
#include "MyString.h"
#include "Resource.h"

int main() {
    // TODO: 调用以上函数与类进行测试
    return 0;
}
```

</details>

## 2.6 现代 C++ 综合练习

- **练习 A：智能指针重写旧代码**
  - 将 `new/delete[]` 改为 `make_unique<int[]>`。
- **练习 B：Lambda + 算法重写循环**
  - 将传统索引循环改为 `for_each` + Lambda。
- **练习 C：小型资源管理器**
  - 模拟管理文件/网络/缓冲区资源，要求：使用智能指针、`auto`、范围 for、Lambda 排序过滤。
- **验收标准**：代码符合现代 C++ 风格，无裸 `new/delete`，异常安全。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 2.6 现代C++综合练习框架 ===
// 项目结构:
// 2-6-modern-cpp/
// ├── Resource.h
// ├── ResourceManager.h
// ├── ResourceManager.cpp
// ├── ModernExercises.h
// ├── ModernExercises.cpp
// └── main.cpp

// ---------- Resource.h ----------
#pragma once
#include <string>

struct Resource {
    std::string name;
    std::string type;     // "file" / "network" / "buffer"
    int priority = 0;
};

// ---------- ResourceManager.h ----------
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Resource.h"

// 练习C: 小型资源管理器
class ResourceManager {
public:
    void add(std::string name, std::string type, int priority);
    void remove(const std::string& name);
    void printByPriority() const;
    void filterByType(const std::string& type) const;

private:
    std::vector<std::unique_ptr<Resource>> resources_;
};

// ---------- ResourceManager.cpp ----------
#include "ResourceManager.h"

#include <algorithm>
#include <iostream>
#include <utility>

void ResourceManager::add(std::string name, std::string type, int priority) {
    // TODO: std::make_unique<Resource>(Resource{std::move(name), std::move(type), priority})
    //       并 push_back 到 resources_
}

void ResourceManager::remove(const std::string& name) {
    // TODO: 使用 std::find_if + Lambda 查找，命中后 erase
}

void ResourceManager::printByPriority() const {
    // TODO: 拷贝指针到临时 vector，std::sort + Lambda 按 priority 排序，打印
}

void ResourceManager::filterByType(const std::string& type) const {
    // TODO: std::for_each + Lambda 过滤并打印指定 type 的资源
}

// ---------- ModernExercises.h ----------
#pragma once

// 练习A: 智能指针重写旧代码
void smartPtrRewrite();

// 练习B: Lambda + 算法重写循环
void lambdaRewrite();

// ---------- ModernExercises.cpp ----------
#include "ModernExercises.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

void smartPtrRewrite() {
    // 旧代码:
    // int* arr = new int[10]; ... delete[] arr;
    // TODO: 改为 auto arr = std::make_unique<int[]>(10);
}

void lambdaRewrite() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    // 旧代码: for (std::size_t i = 0; i < v.size(); ++i) std::cout << v[i] << ' ';
    // TODO: 改为 std::for_each + Lambda
}

// ---------- main.cpp ----------
#include "ModernExercises.h"
#include "ResourceManager.h"

int main() {
    // TODO: 调用以上函数与 ResourceManager 进行测试
    return 0;
}
```

</details>

---

# 阶段三：面向对象设计

本阶段核心目标不是学会写 class，而是理解封装、继承、多态的设计意图，以及虚函数机制与析构安全。

## 3.1 类与封装

- **练习目标**：理解 `class/struct` 区别；掌握访问控制；理解构造/析构的基本职责。
- **练习任务**：
    1. 实现 `Student` 类：姓名、年龄、成绩、打印信息。
    2. 实现 `BankAccount` 类：余额私有，提供存款/取款/查询接口。
    3. 实现 `Rectangle` 类：长宽私有，提供面积/周长计算。
    4. 为 `BankAccount` 增加取款校验：余额不足拒绝操作。
- **巩固标准**：
  - [ ] 能合理划分公有接口与私有数据。
    > **知识讲解**：封装的核心是**数据成员一律设为 `private`，只通过公有的成员函数（接口）暴露必要操作**。这样做的价值：① 隐藏实现细节，外部只依赖稳定的接口，内部可自由重构；② 在接口函数中集中做参数校验（如 `setAge` 拒绝负数），保护对象状态合法；③ 控制读写权限——可提供只读的 getter 而不给 setter。划分原则：**对外暴露“能做什么”（行为），隐藏“用什么数据做”（状态）**。C++ 中 `class` 默认 `private`、`struct` 默认 `public`，因此表达“有不变量需保护的抽象”用 `class`，纯粹的数据聚合（POD-like）才用 `struct`。切忌把数据成员直接设为 public，那等于放弃封装。
  - [ ] 理解封装不仅是隐藏数据，更是保护不变量。
    > **知识讲解**：不变量（invariant）是“对象在整个生命周期中必须始终成立的约束”，如 `BankAccount` 的余额不能为负、`Rectangle` 的长宽必须 > 0、日期对象的月必须在 1~12。封装的真正意义在于**让不变量无法被外部破坏**：把所有能修改状态的入口收敛为公有成员函数，并在其中做校验，就能保证对象一旦构造成功、之后任何时刻都处于合法状态。若数据是 public 的，任何代码都能随意赋非法值，不变量形同虚设，bug 会在远离出错点的地方爆发。因此设计类时应先想清楚“这个类的不变量是什么”，再据此决定哪些操作开放、每个操作需校验什么。构造函数负责建立初始不变量，成员函数负责维护它。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 3.1 类与封装练习框架 ===
// 项目结构:
// 3-1-encapsulation/
// ├── Student.h
// ├── Student.cpp
// ├── BankAccount.h
// ├── BankAccount.cpp
// ├── Rectangle.h
// ├── Rectangle.cpp
// └── main.cpp

// ---------- Student.h ----------
#pragma once
#include <string>

// 练习1: Student 类
class Student {
public:
    Student(std::string n, int a, double s);
    void printInfo() const;      // TODO: 打印姓名、年龄、成绩

private:
    std::string name_;
    int age_ = 0;
    double score_ = 0.0;
};

// ---------- Student.cpp ----------
#include "Student.h"
#include <iostream>
#include <utility>

Student::Student(std::string n, int a, double s)
    : name_(std::move(n)), age_(a), score_(s) {}

void Student::printInfo() const {
    // TODO: 打印 name_ / age_ / score_
}

// ---------- BankAccount.h ----------
#pragma once

// 练习2&4: BankAccount 类
class BankAccount {
public:
    explicit BankAccount(double init = 0.0);

    bool deposit(double amount);       // TODO: 存款，返回是否成功
    bool withdraw(double amount);      // TODO: 取款，余额不足返回 false
    double getBalance() const;         // TODO: 查询余额

private:
    double balance_ = 0.0;             // 私有：余额
};

// ---------- BankAccount.cpp ----------
#include "BankAccount.h"

BankAccount::BankAccount(double init) : balance_(init) {}

bool BankAccount::deposit(double amount) {
    // TODO: amount > 0 时累加到 balance_，返回操作是否成功
    return false;
}

bool BankAccount::withdraw(double amount) {
    // TODO: 校验 amount 合法且 balance_ 足够，否则返回 false
    return false;
}

double BankAccount::getBalance() const {
    // TODO: return balance_;
    return 0.0;
}

// ---------- Rectangle.h ----------
#pragma once

// 练习3: Rectangle 类
class Rectangle {
public:
    Rectangle(double w, double h);
    double area() const;        // TODO: 返回面积
    double perimeter() const;   // TODO: 返回周长

private:
    double width_ = 0.0;
    double height_ = 0.0;
};

// ---------- Rectangle.cpp ----------
#include "Rectangle.h"

Rectangle::Rectangle(double w, double h) : width_(w), height_(h) {}

double Rectangle::area() const {
    // TODO: return width_ * height_;
    return 0.0;
}

double Rectangle::perimeter() const {
    // TODO: return 2.0 * (width_ + height_);
    return 0.0;
}

// ---------- main.cpp ----------
#include "BankAccount.h"
#include "Rectangle.h"
#include "Student.h"

int main() {
    // TODO: 实例化以上类并测试各接口
    return 0;
}
```

</details>

## 3.2 构造与析构函数

- **练习目标**：理解对象生命周期；掌握多种构造函数；理解析构函数的资源释放职责。
- **练习任务**：
    1. 在构造/析构中打印日志，观察调用时机。
    2. 创建对象数组，观察构造/析构顺序。
    3. 实现管理动态数组的类，析构时自动释放。
    4. 实现拷贝构造函数，对比深拷贝与浅拷贝。
    5. 实现 `MyString` 类：构造分配、析构释放、拷贝深拷贝。
- **巩固标准**：
  - [ ] 牢记“谁申请资源，谁负责释放”。
    > **知识讲解**：这是资源管理的基本责任原则——**在构造函数中获取资源（分配内存、打开文件、加锁），就必须在析构函数中对应释放**，且释放顺序与获取顺序相反。之所以强调这条，是因为 C++ 保证：对象一旦构造完成，其析构函数**一定会被调用**（无论离开作用域、被 `delete`、还是栈展开于异常），因此把“释放”绑定到析构函数上就不会遗漏。反面教材是“C 风格”的谁用谁手动 `free`——一旦有多条退出路径（提前 return、抛异常），极易漏掉释放导致泄漏。现代 C++ 更进一步：优先用智能指针/RAII 容器让“释放”自动化，尽量不手写 `new`/`delete`。
  - [ ] 理解 RAII 是 C++ 资源管理的基石。
    > **知识讲解**：RAII（Resource Acquisition Is Initialization，资源获取即初始化）指**把资源的生命周期绑定到对象的生命周期**：构造时获取资源，析构时释放资源。由于栈对象的析构由编译器自动保证（作用域结束、异常栈展开都会触发），RAII 让资源释放不再依赖程序员手写释放代码，从根本上杜绝了“忘记释放”和“异常路径泄漏”。标准库到处是 RAII：`std::unique_ptr`/`shared_ptr` 管内存，`std::lock_guard`/`unique_lock` 管互斥锁，`std::fstream` 管文件，`std::vector`/`string` 管动态数组。掌握 RAII 后，“资源泄漏”问题几乎消失——这也是 C++ 相比 Java/C# 无需 GC 却仍安全的关键。
  - [ ] 知道 Rule of Three/Five 的适用场景。
    > **知识讲解**：**Rule of Three**——若一个类需要自定义析构函数、拷贝构造函数、拷贝赋值运算符中的**任意一个**，那它几乎必然需要全部三个（因为它们通常都涉及资源管理，只定义其一会留下浅拷贝隐患）。C++11 扩展为 **Rule of Five**，加上移动构造函数和移动赋值运算符。触发场景：类**直接持有裸资源**（如 `char* data` 指向 `new` 的内存、文件描述符）时，编译器默认生成的拷贝是浅拷贝（只复制指针，导致 double-free），必须手写这五个函数实现深拷贝与移动。反之 **Rule of Zero**——若用智能指针/标准容器管理资源（不持有裸资源），则五个函数一个都不用写，让编译器默认行为即可，这是现代 C++ 的首选。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 3.2 构造与析构练习框架 ===
// 项目结构:
// 3-2-ctor-dtor/
// ├── Lifecycle.h
// ├── Lifecycle.cpp
// ├── DynArray.h
// ├── DynArray.cpp
// ├── MyString.h
// ├── MyString.cpp
// └── main.cpp

// ---------- Lifecycle.h ----------
#pragma once
#include <string>

// 练习1: 构造/析构打印日志
class Lifecycle {
public:
    explicit Lifecycle(std::string t);
    ~Lifecycle();

private:
    std::string tag_;
};

// ---------- Lifecycle.cpp ----------
#include "Lifecycle.h"
#include <iostream>
#include <utility>

Lifecycle::Lifecycle(std::string t) : tag_(std::move(t)) {
    std::cout << "[构造] " << tag_ << '\n';
}

Lifecycle::~Lifecycle() {
    std::cout << "[析构] " << tag_ << '\n';
}

// ---------- DynArray.h ----------
#pragma once
#include <cstddef>

// 练习3: 管理动态数组的类
class DynArray {
public:
    explicit DynArray(std::size_t n);          // TODO: new int[n]，初始化
    ~DynArray();                                // TODO: delete[] data_
    DynArray(const DynArray& other);            // TODO: 深拷贝
    DynArray& operator=(const DynArray& other); // TODO: 拷贝赋值

    std::size_t getSize() const noexcept { return size_; }

private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
};

// ---------- DynArray.cpp ----------
#include "DynArray.h"

DynArray::DynArray(std::size_t n) {
    // TODO: data_ = new int[n]{}; size_ = n;
}

DynArray::~DynArray() {
    // TODO: delete[] data_;
}

DynArray::DynArray(const DynArray& other) {
    // TODO: 深拷贝 other.data_
}

DynArray& DynArray::operator=(const DynArray& other) {
    // TODO: 自赋值保护 + 释放旧资源 + 深拷贝
    return *this;
}

// ---------- MyString.h ----------
#pragma once

// 练习5: MyString 类
class MyString {
public:
    explicit MyString(const char* s);          // TODO: 分配内存并复制字符串
    ~MyString();                                // TODO: delete[] data_
    MyString(const MyString& other);            // TODO: 深拷贝
    MyString& operator=(const MyString& other); // TODO: 拷贝赋值

    const char* c_str() const noexcept { return data_; }

private:
    char* data_ = nullptr;
};

// ---------- MyString.cpp ----------
#include "MyString.h"
#include <cstring>

MyString::MyString(const char* s) {
    // TODO: std::strlen(s) + new char[n+1] + std::strcpy
}

MyString::~MyString() {
    // TODO: delete[] data_;
}

MyString::MyString(const MyString& other) {
    // TODO: 深拷贝
}

MyString& MyString::operator=(const MyString& other) {
    // TODO: 自赋值保护 + 释放旧资源 + 深拷贝
    return *this;
}

// ---------- main.cpp ----------
#include "DynArray.h"
#include "Lifecycle.h"
#include "MyString.h"

int main() {
    // TODO: 创建对象数组，观察构造/析构顺序
    // TODO: 测试深拷贝与浅拷贝的区别
    return 0;
}
```

</details>

## 3.3 继承

- **练习目标**：理解基类/派生类关系；掌握成员继承规则；理解构造/析构调用顺序。
- **练习任务**：
    1. 实现 `Animal` 基类：名字、年龄、打印信息。
    2. 派生 `Dog/Cat`：增加特有行为，重写打印函数。
    3. 实现 `Shape` 基类及 `Circle/Rectangle/Triangle` 派生类。
    4. 通过日志验证基类构造→派生类构造→派生类析构→基类析构的顺序。
- **巩固标准**：
  - [ ] 理解继承表达的是“is-a”关系，而非代码复用手段。
    > **知识讲解**：公有继承（`class Derived : public Base`）语义上表达**“派生类是一种基类”**（is-a）——`Dog` 是 `Animal`、`Circle` 是 `Shape`，任何用到 `Base` 的地方都能安全替换为 `Derived`（里氏替换原则 LSP）。继承**不是**为了“复用基类代码”而存在——若只是想复用功能而无 is-a 关系（如“汽车有一个引擎”是 has-a，而非“汽车是一种引擎”），应改用**组合/聚合**（把成员对象作为数据成员持有）。误用继承复用代码会导致：接口被强行暴露、基类改动波及所有派生类、继承层次过深难以维护。经验法则：**优先组合，只有在确属 is-a 且需要多态时才用继承**。
  - [ ] 知道 protected 成员的可见性规则。
    > **知识讲解**：`protected` 是介于 `public` 和 `private` 之间的访问级别——**派生类的成员函数可以访问基类的 `protected` 成员，但类外部代码不能**。三种访问级别对比：`public` 谁都能访问；`protected` 仅本类和派生类内部可访问；`private` 仅本类内部可访问（派生类也不行）。注意继承方式还会改变成员在派生类中的可见性：`public` 继承下基类 `public`/`protected` 成员在派生类中保持 `public`/`protected`；`protected` 继承下基类 `public` 成员降为 `protected`；`private` 继承下全部降为 `private`。实践中 `protected` 用于“允许派生类访问、但对外隐藏”的内部数据；不过过度暴露 `protected` 数据会破坏封装，更好的做法是提供 `protected` 的辅助函数。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 3.3 继承练习框架 ===
// 项目结构:
// 3-3-inheritance/
// ├── Animal.h
// ├── Animal.cpp
// ├── Dog.h
// ├── Dog.cpp
// ├── Cat.h
// ├── Cat.cpp
// ├── Shape.h
// ├── Shape.cpp
// ├── Circle.h
// ├── Circle.cpp
// ├── Rectangle.h
// ├── Rectangle.cpp
// ├── Triangle.h
// ├── Triangle.cpp
// └── main.cpp

// ---------- Animal.h ----------
#pragma once
#include <string>

// 练习1: Animal 基类
class Animal {
public:
    Animal(std::string n, int a);
    virtual ~Animal() = default;

    virtual void printInfo() const;      // TODO: 打印名字、年龄

protected:
    std::string name_;
    int age_ = 0;
};

// ---------- Animal.cpp ----------
#include "Animal.h"
#include <iostream>
#include <utility>

Animal::Animal(std::string n, int a) : name_(std::move(n)), age_(a) {}

void Animal::printInfo() const {
    // TODO: 输出 name_ 与 age_
}

// ---------- Dog.h ----------
#pragma once
#include <string>

#include "Animal.h"

// 练习2: Dog 派生
class Dog : public Animal {
public:
    Dog(std::string n, int a, std::string b);
    void printInfo() const override;     // TODO: 打印信息 + 品种
    void bark() const;                    // TODO: 打印 "汪汪"

private:
    std::string breed_;
};

// ---------- Dog.cpp ----------
#include "Dog.h"
#include <iostream>
#include <utility>

Dog::Dog(std::string n, int a, std::string b)
    : Animal(std::move(n), a), breed_(std::move(b)) {}

void Dog::printInfo() const {
    // TODO: 先调用 Animal::printInfo()，再打印 breed_
}

void Dog::bark() const {
    // TODO: std::cout << "汪汪\n";
}

// ---------- Cat.h ----------
#pragma once
#include <string>

#include "Animal.h"

class Cat : public Animal {
public:
    Cat(std::string n, int a, bool indoor);
    void printInfo() const override;     // TODO: 打印信息 + 室内/室外
    void meow() const;                    // TODO: 打印 "喵喵"

private:
    bool isIndoor_ = false;
};

// ---------- Cat.cpp ----------
#include "Cat.h"
#include <iostream>
#include <utility>

Cat::Cat(std::string n, int a, bool indoor)
    : Animal(std::move(n), a), isIndoor_(indoor) {}

void Cat::printInfo() const {
    // TODO: 先调用 Animal::printInfo()，再打印 isIndoor_
}

void Cat::meow() const {
    // TODO: std::cout << "喵喵\n";
}

// ---------- Shape.h ----------
#pragma once

// 练习3: Shape 体系抽象基类
class Shape {
public:
    virtual ~Shape() = default;

    virtual double area() const = 0;
    virtual void printInfo() const;
};

// ---------- Shape.cpp ----------
#include "Shape.h"
#include <iostream>

void Shape::printInfo() const {
    std::cout << "Shape\n";
}

// ---------- Circle.h ----------
#pragma once
#include "Shape.h"

class Circle : public Shape {
public:
    explicit Circle(double r);
    double area() const override;        // TODO: π*r*r

private:
    double radius_ = 0.0;
};

// ---------- Circle.cpp ----------
#include "Circle.h"
#include <cmath>

Circle::Circle(double r) : radius_(r) {}

double Circle::area() const {
    // TODO: return M_PI * radius_ * radius_;
    return 0.0;
}

// ---------- Rectangle.h ----------
#pragma once
#include "Shape.h"

class Rectangle : public Shape {
public:
    Rectangle(double w, double h);
    double area() const override;        // TODO: w*h

private:
    double width_ = 0.0;
    double height_ = 0.0;
};

// ---------- Rectangle.cpp ----------
#include "Rectangle.h"

Rectangle::Rectangle(double w, double h) : width_(w), height_(h) {}

double Rectangle::area() const {
    // TODO: return width_ * height_;
    return 0.0;
}

// ---------- Triangle.h ----------
#pragma once
#include "Shape.h"

class Triangle : public Shape {
public:
    Triangle(double b, double h);
    double area() const override;        // TODO: 0.5*b*h

private:
    double base_ = 0.0;
    double height_ = 0.0;
};

// ---------- Triangle.cpp ----------
#include "Triangle.h"

Triangle::Triangle(double b, double h) : base_(b), height_(h) {}

double Triangle::area() const {
    // TODO: return 0.5 * base_ * height_;
    return 0.0;
}

// ---------- main.cpp ----------
#include "Cat.h"
#include "Circle.h"
#include "Dog.h"
#include "Rectangle.h"
#include "Triangle.h"

int main() {
    // TODO: 测试各派生类的 printInfo 和 area
    // TODO: 验证 基类构造 → 派生类构造 → 派生类析构 → 基类析构 的顺序
    return 0;
}
```

</details>

## 3.4 多态与虚函数

- **练习目标**：理解运行时多态机制；掌握基类指针/引用调用派生类行为；正确使用 `override`。
- **练习任务**：
    1. 用基类指针数组管理多个派生类对象并调用虚函数。
    2. 实现图形面积计算多态体系。
    3. 实现员工工资系统：Employee/Manager/Developer 各自计算逻辑。
    4. 实现日志系统：Logger 基类 + FileLogger/ConsoleLogger 派生。
- **巩固标准**：
  - [ ] 理解没有 `virtual` 就没有运行时多态。
    > **知识讲解**：运行时多态（动态绑定）依赖 `virtual` 关键字。当通过**基类指针或引用**调用一个函数时：若该函数在基类中声明为 `virtual`，则在运行时根据指针实际指向的对象类型（动态类型）调用对应版本——这才能实现“基类指针调用派生类行为”；若**没有 `virtual`**，则是静态绑定，编译期就按指针的声明类型（基类）决定调用哪个版本，派生类的同名函数只是“隐藏”而非“重写”，多态失效。因此凡是设计为“通过基类接口统一调用、期望表现出各自派生行为”的函数（如 `Shape::area()`、`Logger::log()`）**必须声明为 `virtual`**。此外基类析构函数通常也需 `virtual`（见 3.5）。
  - [ ] 始终对重写函数使用 `override` 关键字。
    > **知识讲解**：C++11 的 `override` 关键字显式标注“此函数意在重写基类虚函数”，让编译器帮你校验。它的价值在于**捕获隐蔽错误**：若函数签名与基类不完全一致（参数类型/const 限定/漏写 `virtual` 等），没有 `override` 时编译器会认为你在定义一个**全新的函数**而非重写，多态悄无声息地失效，极难排查；加了 `override`，编译器会直接报错“没有找到可重写的基类函数”。例如基类 `virtual void f(int)`，派生类误写 `void f(double)`——无 `override` 时合法但没重写成功，有 `override` 时立即报错。准则：**所有重写虚函数都加 `override`**（可省略 `virtual`，因为 `override` 已足够表达意图）。
  - [ ] 理解虚函数表（vtable）的实现原理。
    > **知识讲解**：编译器为**每个含虚函数的类**生成一张虚函数表（vtable）——一个存放该类所有虚函数地址的数组；同时为**每个对象**在内存布局最前面插入一个隐藏指针 vptr，指向其所属类的 vtable。调用虚函数时（通过基类指针/引用），实际执行“经 vptr 找到 vtable，再按固定槽位取出函数地址并调用”，即间接跳转，这就是动态绑定的实现，也是虚函数比直接调用略慢（多两次内存访问 + 无法内联）的原因。派生类重写某虚函数时，编译器把派生类 vtable 对应槽位的地址替换为派生类版本，其余槽位继承基类地址。构造派生类对象时，vptr 会随构造过程逐步更新（先指向基类 vtable，再指向派生类 vtable），这也是**构造函数中调用虚函数不会触发多态**的原因——那时 vptr 还指向基类 vtable。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 3.4 多态与虚函数练习框架 ===
// 项目结构:
// 3-4-polymorphism/
// ├── shape.h                       （复用 3.3 的 Shape/Circle/Rectangle/Triangle）
// ├── Shape.cpp
// ├── PolyDemo.h
// ├── PolyDemo.cpp
// ├── Employee.h
// ├── Employee.cpp
// ├── Manager.h
// ├── Manager.cpp
// ├── Developer.h
// ├── Developer.cpp
// ├── Logger.h
// ├── FileLogger.h
// ├── FileLogger.cpp
// ├── ConsoleLogger.h
// ├── ConsoleLogger.cpp
// └── main.cpp

// ---------- PolyDemo.h ----------
#pragma once

// 练习1&2: 图形面积多态体系（复用 3.3 的 Shape/Circle/Rectangle/Triangle）
void polyDemo();

// ---------- PolyDemo.cpp ----------
#include "PolyDemo.h"

#include <iostream>
#include <memory>
#include <vector>

#include "Circle.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Triangle.h"

void polyDemo() {
    std::vector<std::unique_ptr<Shape>> shapes;
    // TODO: shapes.push_back(std::make_unique<Circle>(1.0));  ……
    // TODO: 遍历调用 area()，观察多态行为
}

// ---------- Employee.h ----------
#pragma once
#include <string>

// 练习3: 员工工资系统
class Employee {
public:
    explicit Employee(std::string n);
    virtual ~Employee() = default;

    virtual double calcSalary() const = 0;   // TODO: 纯虚函数
    virtual void printInfo() const;

protected:
    std::string name_;
};

// ---------- Employee.cpp ----------
#include "Employee.h"
#include <iostream>
#include <utility>

Employee::Employee(std::string n) : name_(std::move(n)) {}

void Employee::printInfo() const {
    // TODO: 输出 name_ 与 calcSalary()
}

// ---------- Manager.h ----------
#pragma once
#include <string>

#include "Employee.h"

class Manager : public Employee {
public:
    Manager(std::string n, double base);
    double calcSalary() const override;      // TODO: baseSalary + 提成

private:
    double baseSalary_ = 0.0;
};

// ---------- Manager.cpp ----------
#include "Manager.h"
#include <utility>

Manager::Manager(std::string n, double base)
    : Employee(std::move(n)), baseSalary_(base) {}

double Manager::calcSalary() const {
    // TODO: return baseSalary_ * 1.2;  （示例：20% 提成）
    return 0.0;
}

// ---------- Developer.h ----------
#pragma once
#include <string>

#include "Employee.h"

class Developer : public Employee {
public:
    Developer(std::string n, int h, double rate);
    double calcSalary() const override;      // TODO: hours * hourlyRate

private:
    int hours_ = 0;
    double hourlyRate_ = 0.0;
};

// ---------- Developer.cpp ----------
#include "Developer.h"
#include <utility>

Developer::Developer(std::string n, int h, double rate)
    : Employee(std::move(n)), hours_(h), hourlyRate_(rate) {}

double Developer::calcSalary() const {
    // TODO: return hours_ * hourlyRate_;
    return 0.0;
}

// ---------- Logger.h ----------
#pragma once
#include <string>

// 练习4: 日志系统
class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const std::string& msg) const = 0;
};

// ---------- FileLogger.h ----------
#pragma once
#include <string>

#include "Logger.h"

class FileLogger : public Logger {
public:
    explicit FileLogger(std::string f);
    void log(const std::string& msg) const override;   // TODO: 写入文件

private:
    std::string filename_;
};

// ---------- FileLogger.cpp ----------
#include "FileLogger.h"
#include <fstream>
#include <utility>

FileLogger::FileLogger(std::string f) : filename_(std::move(f)) {}

void FileLogger::log(const std::string& msg) const {
    // TODO: std::ofstream ofs(filename_, std::ios::app); ofs << msg << '\n';
}

// ---------- ConsoleLogger.h ----------
#pragma once
#include "Logger.h"

class ConsoleLogger : public Logger {
public:
    void log(const std::string& msg) const override;   // TODO: 输出到控制台
};

// ---------- ConsoleLogger.cpp ----------
#include "ConsoleLogger.h"
#include <iostream>

void ConsoleLogger::log(const std::string& msg) const {
    // TODO: std::cout << msg << '\n';
}

// ---------- main.cpp ----------
#include "ConsoleLogger.h"
#include "Developer.h"
#include "FileLogger.h"
#include "Manager.h"
#include "PolyDemo.h"

#include <memory>
#include <vector>

int main() {
    // TODO: 测试以上多态体系
    return 0;
}
```

</details>

## 3.5 虚析构函数

- **练习目标**：理解基类指针删除派生类对象的资源泄漏风险；掌握虚析构函数的必要性。
- **练习任务**：
    1. 基类/派生类均动态分配资源。
    2. 用基类指针 `new` 派生类对象后 `delete`。
    3. 观察无虚析构时的析构不完整现象。
    4. 添加 `virtual ~Base()` 后验证正确释放。
- **巩固标准**：
  - [ ] 牢记规则：**只要类可能被继承且通过基类指针删除，就必须有虚析构函数**。
    > **知识讲解**：当用**基类指针 `delete` 一个派生类对象**时，若基类析构函数**不是 `virtual`**，则只会调用基类析构函数，派生类析构函数被跳过——派生类中申请的资源（内存、文件句柄）不会释放，造成资源泄漏和未定义行为。若基类析构声明为 `virtual`，则会通过 vtable 动态绑定，正确调用派生类析构→再自动调用基类析构，完整释放。规则：**只要一个类打算作为多态基类被继承，且可能通过基类指针/引用删除派生对象，其析构函数就必须是 `virtual`**。反之，若一个类确定不会被继承（可标记 `final`）或不通过基类指针删除，则不必加虚析构——因为虚析构会引入 vtable/vptr，增加对象体积（每个对象多一个指针）。
  - [ ] 理解纯虚析构函数的特殊写法。
    > **知识讲解**：纯虚函数用于把类声明为抽象类（不可实例化）。通常写法是 `virtual void f() = 0;` 且不提供实现。**纯虚析构函数特殊之处在于：即使声明为纯虚 `virtual ~Base() = 0;`，也必须在类外提供函数体定义**——因为派生类析构时会自动调用基类析构函数，若基类析构没有定义，链接阶段会报“undefined reference”。正确写法：类内声明 `virtual ~Base() = 0;`，类外补 `inline Base::~Base() {}`（或普通定义）。这样既让 `Base` 成为抽象类（因存在纯虚函数），又保证派生类能正确链接。这是纯虚析构与普通纯虚函数唯一的、也是最容易踩坑的差异。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 3.5 虚析构函数练习框架 ===
// 项目结构:
// 3-5-virtual-dtor/
// ├── Base.h
// ├── Base.cpp
// ├── Derived.h
// ├── Derived.cpp
// └── main.cpp

// ---------- Base.h ----------
#pragma once

// 练习1: 基类/派生类均动态分配资源
class Base {
public:
    explicit Base(int val);

    // TODO: 先不加 virtual，观察问题
    // TODO: 然后改为 virtual ~Base();
    ~Base();

protected:
    int* data_ = nullptr;
};

// ---------- Base.cpp ----------
#include "Base.h"
#include <iostream>

Base::Base(int val) : data_(new int(val)) {
    std::cout << "[Base 构造]\n";
}

Base::~Base() {
    delete data_;
    std::cout << "[Base 析构]\n";
}

// ---------- Derived.h ----------
#pragma once
#include "Base.h"

class Derived : public Base {
public:
    Derived(int val, int extraVal);
    ~Derived();      // TODO: 释放 extra_

private:
    int* extra_ = nullptr;
};

// ---------- Derived.cpp ----------
#include "Derived.h"
#include <iostream>

Derived::Derived(int val, int extraVal) : Base(val), extra_(new int(extraVal)) {
    std::cout << "[Derived 构造]\n";
}

Derived::~Derived() {
    delete extra_;
    std::cout << "[Derived 析构]\n";
}

// ---------- main.cpp ----------
#include "Base.h"
#include "Derived.h"

int main() {
    // 练习2: 基类指针 new 派生类后 delete
    Base* p = new Derived(1, 2);
    delete p;   // 练习3: 观察无虚析构时 Derived::~Derived 是否被调用

    // 练习4: 将 Base::~Base 改为 virtual 后重新验证
    return 0;
}
```

</details>

## 3.6 面向对象综合项目（三选一）

- **项目 A：图形系统**
  - 用 `vector<unique_ptr<Shape>>` 统一管理 Circle/Rectangle/Triangle，计算面积/周长/打印信息。
- **项目 B：员工管理系统**
  - 实现 Employee/Manager/Programmer 继承体系，计算工资、打印信息、按工资排序。
- **项目 C：日志系统**
  - 实现 Logger 抽象基类，FileLogger/ConsoleLogger 多态输出，支持 INFO/WARN/ERROR 级别切换。
- **验收标准**：多态行为正确，无内存泄漏，扩展新类型无需修改已有代码（开闭原则）。

<details>
<summary>📦 项目框架代码（三选一）</summary>

```cpp
// === 3.6 面向对象综合项目（三选一） ===
// 以下三个项目任选其一完成，项目结构已按现代 C++ 风格拆分。

// ================================================================
// 项目 A：3-6-a-shape-system/
//   ├── Shape.h
//   ├── Circle.h / Circle.cpp
//   ├── Rectangle.h / Rectangle.cpp
//   ├── Triangle.h / Triangle.cpp
//   ├── ShapeManager.h / ShapeManager.cpp
//   └── main.cpp
// ================================================================

// ---------- Shape.h ----------
#pragma once

class Shape {
public:
    virtual ~Shape() = default;

    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void printInfo() const = 0;
};

// ---------- Circle.h ----------
#pragma once
#include "Shape.h"

class Circle : public Shape {
public:
    explicit Circle(double r);
    double area() const override;         // TODO: π*r²
    double perimeter() const override;    // TODO: 2πr
    void printInfo() const override;

private:
    double radius_ = 0.0;
};

// ---------- Rectangle.h ----------
#pragma once
#include "Shape.h"

class Rectangle : public Shape {
public:
    Rectangle(double w, double h);
    double area() const override;         // TODO: w*h
    double perimeter() const override;    // TODO: 2(w+h)
    void printInfo() const override;

private:
    double w_ = 0.0;
    double h_ = 0.0;
};

// ---------- Triangle.h ----------
#pragma once
#include "Shape.h"

class Triangle : public Shape {
public:
    Triangle(double a, double b, double c);   // 三边
    double area() const override;              // TODO: 海伦公式
    double perimeter() const override;         // TODO: a+b+c
    void printInfo() const override;

private:
    double a_ = 0.0;
    double b_ = 0.0;
    double c_ = 0.0;
};

// ---------- ShapeManager.h ----------
#pragma once
#include <memory>
#include <vector>

#include "Shape.h"

class ShapeManager {
public:
    void add(std::unique_ptr<Shape> s);   // TODO
    void printAll() const;                 // TODO
    double totalArea() const;              // TODO

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};

// ---------- ShapeManager.cpp ----------
#include "ShapeManager.h"
#include <utility>

void ShapeManager::add(std::unique_ptr<Shape> s) {
    // TODO: shapes_.push_back(std::move(s));
}

void ShapeManager::printAll() const {
    // TODO: 遍历 shapes_ 并调用 printInfo()
}

double ShapeManager::totalArea() const {
    // TODO: 累加 area()
    return 0.0;
}

// ---------- main.cpp ----------
#include "Circle.h"
#include "Rectangle.h"
#include "ShapeManager.h"
#include "Triangle.h"

#include <memory>

int main() {
    // TODO: 实例化 ShapeManager，添加多种图形并测试全部功能
    return 0;
}

// ================================================================
// 项目 B：3-6-b-employee-system/
//   ├── Employee.h / Employee.cpp
//   ├── Manager.h / Manager.cpp
//   ├── Programmer.h / Programmer.cpp
//   ├── EmployeeManager.h / EmployeeManager.cpp
//   └── main.cpp
// ================================================================

// ---------- Employee.h ----------
#pragma once
#include <string>

class Employee {
public:
    explicit Employee(std::string n);
    virtual ~Employee() = default;

    virtual double calcSalary() const = 0;
    virtual void printInfo() const;

protected:
    std::string name_;
};

// ---------- Manager.h ----------
#pragma once
#include <string>

#include "Employee.h"

class Manager : public Employee {
public:
    Manager(std::string n, double base);
    double calcSalary() const override;   // TODO

private:
    double baseSalary_ = 0.0;
};

// ---------- Programmer.h ----------
#pragma once
#include <string>

#include "Employee.h"

class Programmer : public Employee {
public:
    Programmer(std::string n, int h, double r);
    double calcSalary() const override;   // TODO

private:
    int hours_ = 0;
    double rate_ = 0.0;
};

// ---------- EmployeeManager.h ----------
#pragma once
#include <memory>
#include <vector>

#include "Employee.h"

class EmployeeManager {
public:
    void add(std::unique_ptr<Employee> e);   // TODO
    void printAll() const;                    // TODO
    void sortBySalary();                      // TODO

private:
    std::vector<std::unique_ptr<Employee>> employees_;
};

// ---------- main.cpp ----------
#include "EmployeeManager.h"
#include "Manager.h"
#include "Programmer.h"

int main() {
    // TODO: 实例化 EmployeeManager，添加多种员工并测试全部功能
    return 0;
}

// ================================================================
// 项目 C：3-6-c-log-system/
//   ├── LogLevel.h
//   ├── Logger.h
//   ├── FileLogger.h / FileLogger.cpp
//   ├── ConsoleLogger.h / ConsoleLogger.cpp
//   ├── LogSystem.h / LogSystem.cpp
//   └── main.cpp
// ================================================================

// ---------- LogLevel.h ----------
#pragma once

enum class LogLevel { INFO, WARN, ERROR };

// ---------- Logger.h ----------
#pragma once
#include <string>

class Logger {
public:
    virtual ~Logger() = default;
    virtual void write(const std::string& msg) const = 0;
};

// ---------- FileLogger.h ----------
#pragma once
#include <string>

#include "Logger.h"

class FileLogger : public Logger {
public:
    explicit FileLogger(std::string f);
    void write(const std::string& msg) const override;   // TODO: 写入文件

private:
    std::string filename_;
};

// ---------- ConsoleLogger.h ----------
#pragma once
#include "Logger.h"

class ConsoleLogger : public Logger {
public:
    void write(const std::string& msg) const override;   // TODO: 输出到 std::cout
};

// ---------- LogSystem.h ----------
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "LogLevel.h"
#include "Logger.h"

class LogSystem {
public:
    void addLogger(std::unique_ptr<Logger> l);           // TODO
    void setLevel(LogLevel lv) noexcept { minLevel_ = lv; }
    void log(LogLevel lv, const std::string& msg);       // TODO: 根据级别过滤，调用所有 logger

private:
    std::vector<std::unique_ptr<Logger>> loggers_;
    LogLevel minLevel_ = LogLevel::INFO;
};

// ---------- main.cpp ----------
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "LogSystem.h"

#include <memory>

int main() {
    // TODO: 实例化 LogSystem，注册 FileLogger / ConsoleLogger，发送不同级别日志
    return 0;
}
```

</details>

---

# 阶段四：Linux 后端核心技能

本阶段进入系统级编程，重点是文件 I/O、进程、线程、网络与高并发模型，脱离语法题转向工程实践。

## 4.1 文件 I/O

- **练习目标**：掌握 C++ 标准文件流操作（`ifstream`/`ofstream`/`fstream`）；理解文件打开模式与流状态；了解 C++ 文件流底层对应的 POSIX 系统调用。
- **练习任务**：
    1. 实现简化版 `cp`：复制文件。
    2. 实现简化版 `cat`：打印文件内容。
    3. 统计文件大小。
    4. 逐行读取文本文件。
    5. 日志写入文件，支持追加模式。
    6. 实现简易配置文件解析器（key=value 格式）。
- **巩固标准**：
  - [ ] 能用 `std::ifstream`/`std::ofstream` 完成基本文件读写操作。
    > **知识讲解**：C++ 用 `<fstream>` 提供三个文件流类：`std::ifstream`（只读输入）、`std::ofstream`（只写输出）、`std::fstream`（可读可写）。基本读法有两种：① 逐行读取用 `while (std::getline(ifs, line))`，最安全，避免 `while(!ifs.eof())` 的经典陷阱——eof 在读完最后一次后才置位，会导致多读一次空行；② 按词/按值读取用 `while (ifs >> word)`，会自动跳过空白。写文件用 `ofs << "text"` 或 `ofs.write(...)`。流对象是 RAII 的——离开作用域自动关闭，无需手动 `close()`（也可显式调用）。读取前应检查 `if (!ifs)` 或 `ifs.is_open()` 判断文件是否成功打开，避免对无效流操作。
  - [ ] 理解文件打开模式（`binary`/`app`/`trunc`/`in`/`out`）的含义与组合。
    > **知识讲解**：打开模式是 `std::ios_base` 的位标志，可用 `|` 组合。`in` 读、`out` 写、`app`（append）追加到文件末尾（即使先 seek 也仍在末尾写）、`trunc` 打开时清空原内容、`binary` 二进制模式（不做换行符转换）、`ate` 打开后定位到末尾（但之后可自由 seek）。默认值：`ifstream` 默认 `in`，`ofstream` 默认 `out | trunc`（即会覆盖！）。常见组合：追加日志用 `std::ios::app`，读写二进制结构体用 `in | out | binary`，清空重写用 `out | trunc`。易错点：想追加却用默认 `ofstream` 会把文件清空，必须显式加 `app`。
  - [ ] 知道 C++ 文件流与底层 POSIX 系统调用（`open/read/write`）的对应关系。
    > **知识讲解**：C++ 文件流是 POSIX 系统调用之上的 C++ 封装，对应关系为：`std::ifstream`/`ofstream` 构造（打开文件）→ `open()`/`openat()`，`>>`/`getline`/`read()` → `read()`，`<<`/`write()` → `write()`，`seekg`/`seekp`（移动读/写指针）→ `lseek()`，析构或 `close()` → `close()`。区别在于：C++ 流带用户态缓冲区、类型安全、支持格式化和 RAII 自动关闭，更适合文本处理；POSIX 调用无缓冲（或需自己管理）、返回字节数/错误码、跨语言通用，适合底层控制（如设置 `O_NONBLOCK`、文件描述符传递给子进程、`mmap`）。网络编程中 socket 只能用 POSIX（无 C++ 标准封装），文件则两者皆可，教学上先用 fstream 建立概念，再理解底层系统调用。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 4.1 文件I/O练习框架 ===
// 项目结构:
// 4-1-file-io/
// ├── FileIoExercises.h
// ├── FileIoExercises.cpp
// ├── ConfigParser.h
// ├── ConfigParser.cpp
// └── main.cpp

// ---------- FileIoExercises.h ----------
#pragma once
#include <string>

// 练习1: 简化版 cp（复制文件）
void copyFile(const std::string& src, const std::string& dst);

// 练习2: 简化版 cat（打印文件内容）
void catFile(const std::string& filename);

// 练习3: 统计文件大小
std::streamsize fileSize(const std::string& filename);

// 练习4: 逐行读取文本文件
void readLines(const std::string& filename);

// 练习5: 日志写入（追加模式）
void writeLog(const std::string& filename, const std::string& msg);

// ---------- FileIoExercises.cpp ----------
#include "FileIoExercises.h"

#include <fstream>
#include <iostream>
#include <string>

void copyFile(const std::string& src, const std::string& dst) {
    // TODO: std::ifstream 以 binary 模式打开源文件
    // TODO: std::ofstream 以 binary|trunc 模式打开目标文件
    // TODO: 用 ofs << ifs.rdbuf() 完成复制
}

void catFile(const std::string& filename) {
    // TODO: std::ifstream 以 binary 模式打开文件
    // TODO: std::cout << ifs.rdbuf() 打印内容
}

std::streamsize fileSize(const std::string& filename) {
    // TODO: std::ifstream 以 binary 模式打开，seekg 到末尾，tellg 获取大小
    return 0;
}

void readLines(const std::string& filename) {
    // TODO: std::ifstream 打开文件，std::getline 逐行读取并打印
}

void writeLog(const std::string& filename, const std::string& msg) {
    // TODO: std::ofstream 以 out|app 模式打开文件，写入 msg
}

// ---------- ConfigParser.h ----------
#pragma once
#include <string>
#include <unordered_map>

// 练习6: 简易配置文件解析器（key=value）
std::unordered_map<std::string, std::string> parseConfig(const std::string& filename);

// ---------- ConfigParser.cpp ----------
#include "ConfigParser.h"
#include <fstream>

std::unordered_map<std::string, std::string> parseConfig(const std::string& filename) {
    std::unordered_map<std::string, std::string> config;
    // TODO: std::ifstream 逐行读取，解析 key=value，存入 config
    return config;
}

// ---------- main.cpp ----------
#include "ConfigParser.h"
#include "FileIoExercises.h"

int main() {
    // TODO: 调用以上函数进行测试
    return 0;
}
```

</details>

## 4.2 进程

- **练习目标**：理解进程创建与地址空间隔离；掌握 `fork/exec/wait`。
- **练习任务**：
    1. `fork` 创建子进程，分别打印 PID。
    2. 父进程 `wait` 等待子进程结束。
    3. 子进程 `exec` 执行 `ls` 命令。
    4. 实现简易 Shell：读取输入→解析命令→fork→exec。
    5. 验证 fork 后父子进程变量独立性。
- **巩固标准**：
  - [ ] 理解 fork 后是独立地址空间，非共享变量。
    > **知识讲解**：`fork()` 创建子进程，子进程是父进程的**副本**——它复制了父进程的代码段、数据段、堆栈，得到一份**独立的地址空间**。因此 fork 后父子进程各自拥有变量的独立副本，一方修改自己的变量，另一方看不到（这与线程共享地址空间截然不同）。关键特性：`fork()` 调用一次却**返回两次**——父进程中返回子进程的 PID（>0），子进程中返回 0，失败返回 -1；据此用 `if (pid == 0)` 区分父子执行不同分支。现代 Linux 用**写时复制（COW）**优化：fork 时不真正复制物理内存，而是父子共享只读页，任一方写入时才复制该页，因此 fork 很快、内存开销小。
  - [ ] 掌握 exec 族函数的区别与用法。
    > **知识讲解**：`exec` 族函数用**新程序的映像替换当前进程的地址空间**（代码、数据、堆栈全部替换），进程 PID 不变，替换成功后原进程后续代码不再执行（不返回）。常见变体区别在参数传递与路径查找方式：后缀 `l` 表示参数逐个列出（`execl`），`v` 表示用 `char* argv[]` 数组（`execv`），`p` 表示在 `PATH` 中查找可执行文件（`execvp`），`e` 表示可传环境变量数组。最常用的是 `execvp(file, argv)`，其中 `argv` 必须以 `NULL` 结尾且 `argv[0]` 惯例为程序名。典型模式是 `fork()` 后在子进程中调用 `exec` 运行外部程序，父进程用 `wait`/`waitpid` 等待其结束——这正是 shell 执行命令的原理。注意 `execvp` 的参数指针生命周期，避免悬垂。
  - [ ] 知道僵尸进程的产生原因与避免方法。
    > **知识讲解**：**僵尸进程（zombie）** 指子进程已退出，但父进程尚未调用 `wait`/`waitpid` 回收其退出状态——此时子进程的进程描述符（保存退出码等）仍占用，在 `ps` 中显示为 `Z`。危害：每个僵尸占用一个 PID，大量积累会耗尽 PID 表导致无法创建新进程。产生原因是父进程“忘记”回收。**避免/处理方法**：① 父进程调用 `wait`（阻塞等待）或 `waitpid`（可非阻塞 `WNOHANG`）回收；② 用 `SIGCHLD` 信号处理函数异步回收（子进程退出时内核向父进程发 `SIGCHLD`，处理函数中循环 `waitpid(-1, NULL, WNOHANG)`）；③ 显式忽略 `signal(SIGCHLD, SIG_IGN)`，让内核自动回收；④ **孤儿进程**（父进程先退出）会被 init/systemd（PID 1）收养并自动回收，因此不是问题。注意：僵尸进程无法被 `kill`（它已经死了），只能靠父进程回收或杀掉父进程让其被 init 收养。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 4.2 进程练习框架 ===
// 项目结构:
// 4-2-process/
// ├── ProcessExercises.h
// ├── ProcessExercises.cpp
// ├── MiniShell.h
// ├── MiniShell.cpp
// └── main.cpp

// ---------- ProcessExercises.h ----------
#pragma once

// 练习1: fork 创建子进程，分别打印 PID
void forkDemo();

// 练习2: 父进程 wait 等待子进程结束
void waitDemo();

// 练习3: 子进程 exec 执行 ls 命令
void execDemo();

// 练习5: 验证 fork 后父子进程变量独立性
void forkIndependence();

// ---------- ProcessExercises.cpp ----------
#include "ProcessExercises.h"

#include <cstdlib>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

void forkDemo() {
    pid_t pid = ::fork();
    if (pid < 0) {
        // TODO: 错误处理
    } else if (pid == 0) {
        // TODO: 子进程打印自己的 PID 和父进程 PID
    } else {
        // TODO: 父进程打印自己的 PID 和子进程 PID
    }
}

void waitDemo() {
    pid_t pid = ::fork();
    if (pid == 0) {
        // 子进程: sleep(2) 后 exit(42)
    } else {
        // TODO: wait(&status)，打印子进程退出码
    }
}

void execDemo() {
    pid_t pid = ::fork();
    if (pid == 0) {
        // TODO: ::execlp("ls", "ls", "-l", nullptr);
    } else {
        ::wait(nullptr);
    }
}

void forkIndependence() {
    int x = 100;
    pid_t pid = ::fork();
    if (pid == 0) {
        // TODO: 子进程修改 x，打印
    } else {
        // TODO: 父进程 sleep 后打印 x，验证未被子进程修改
        ::wait(nullptr);
    }
}

// ---------- MiniShell.h ----------
#pragma once

// 练习4: 简易 Shell
void miniShell();

// ---------- MiniShell.cpp ----------
#include "MiniShell.h"

#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

void miniShell() {
    std::string cmd;
    while (true) {
        std::cout << "myshell> ";
        // TODO: std::getline(std::cin, cmd) 读取输入
        // TODO: std::istringstream 解析命令和参数
        // TODO: fork + exec 执行命令
        // TODO: 输入 "exit" 时退出
    }
}

// ---------- main.cpp ----------
#include "MiniShell.h"
#include "ProcessExercises.h"

int main() {
    // TODO: 调用以上函数进行测试
    return 0;
}
```

</details>

## 4.3 线程

- **练习目标**：掌握 `std::thread`；理解并发执行与线程生命周期。
- **练习任务**：
    1. 多线程打印数字。
    2. 一线程输入、一线程处理。
    3. 多线程累加计数器，观察数据竞争。
    4. 实现生产者消费者模型。
    5. 实现下载模拟器：多线程并行下载文件片段。
- **巩固标准**：
  - [ ] 理解多线程必须考虑同步，而非"写得快就对"。
    > **知识讲解**：多线程并发写入共享变量时，`++counter` 看似一行代码，实际是"读→加→写"三步操作。两个线程可能同时读到相同的旧值，各自加 1 后写回，导致一次更新丢失。这就是**数据竞争（data race）**，属于未定义行为（UB）。解决方案：用 `std::mutex` + `std::lock_guard` 保护临界区，或使用 `std::atomic<int>` 让编译器生成原子指令。`raceDemo` 中 10 个线程各累加 100000 次，期望值 1000000，实际结果远小于此——缺失的数据正是被竞争覆盖的更新。
  - [ ] 知道 `join/detach` 的区别与必要性。
    > **知识讲解**：`join()` 阻塞当前线程，等待目标线程执行完毕后才继续——用于"需要同步等待结果"的场景。`detach()` 将线程与 `std::thread` 对象分离，线程在后台独立运行，`std::thread` 对象变为不可连接状态——用于"不需要等待结果、后台独立运行"的场景。**必须二选一**：如果 `std::thread` 对象在析构时既没有 `join` 也没有 `detach`，程序会调用 `std::terminate()` 直接崩溃。常见错误：① 对已 `join`/`detach` 的线程再次操作（抛 `std::system_error`）；② `detach` 后线程访问了局部变量的引用（局部变量已析构，悬垂引用）。
  - [ ] 能识别常见的数据竞争场景。
    > **知识讲解**：常见数据竞争场景：① 多线程同时 `++/--` 共享计数器（练习 3 `raceDemo`）；② 多线程同时写入 `std::cout`（练习 1 `multiPrintDemo` 中的行内撕裂 `[id 3[id ]: 308`，因为多次 `<<` 不是原子操作）；③ 多线程同时修改容器（`push_back`、`insert` 等）。识别方法：只要两个或以上线程**同时读写同一块内存**，且至少有一个是写操作，就存在数据竞争。解决方案：`std::mutex` 保护临界区、`std::atomic` 保护单变量、或改为线程局部存储避免共享。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 4.3 线程练习框架 ===
// 项目结构:
// 4-3-thread/
// ├── ThreadExercises.h
// ├── ThreadExercises.cpp
// └── main.cpp

// ---------- ThreadExercises.h ----------
#pragma once
#include <string>

// 练习1: 多线程打印数字
void printNumbers(int id, int start, int end);
void multiPrintDemo();

// 练习3: 多线程累加计数器，观察数据竞争
void raceDemo();

// 练习5: 下载模拟器
void downloadSimulator(const std::string& filename, int numChunks, int numThreads);

// ---------- ThreadExercises.cpp ----------
#include "ThreadExercises.h"

#include <iostream>
#include <thread>
#include <vector>

void printNumbers(int id, int start, int end) {
    // TODO: 循环打印 id 和数字
}

void multiPrintDemo() {
    // TODO: 先用 join 演示——创建线程，join 等待其结束，观察主线程在子线程之后才继续
    // TODO: 再用 detach 演示——创建线程，detach 分离，观察主线程不等待、子线程后台独立运行
}

void raceDemo() {
    int counter = 0;
    auto increment = [&counter]() {
        for (int i = 0; i < 100000; ++i) {
            ++counter;   // TODO: 观察无保护时的数据竞争
        }
    };
    // TODO: 创建多个线程调用 increment，观察最终 counter 值
}

void downloadSimulator(const std::string& filename, int numChunks, int numThreads) {
    // TODO: 每个线程负责下载一部分 chunk
    // TODO: 打印每个线程的进度
}

// ---------- main.cpp ----------
#include "ThreadExercises.h"

int main() {
    // 练习2: 一线程输入、一线程处理
    // TODO: 生产者线程填充数据，消费者线程处理数据

    // 练习4: 生产者消费者模型（框架，完整实现见 4.4）
    // TODO: 用 std::mutex + std::condition_variable 实现

    // TODO: 调用以上函数进行测试
    return 0;
}
```

</details>

## 4.4 互斥锁与条件变量

- **练习目标**：掌握 `mutex/lock_guard/condition_variable`；解决数据竞争与线程等待。
- **练习任务**：
    1. 用 mutex 保护多线程累加操作。
    2. 实现线程安全队列：push/pop/empty。
    3. 实现完整生产者消费者：队列满时生产者等待，空时消费者等待。
    4. 用条件变量实现任务队列。
    5. 实现简易线程池雏形：主线程提交任务，工作线程取任务执行。
- **巩固标准**：
  - [ ] 能写出无明显数据竞争的多线程程序。
    > **知识讲解**：数据竞争指两个及以上线程**同时访问同一内存、且至少一个写、又无同步**，属于未定义行为。写出无竞争程序的要点：① 用 `std::mutex` 保护所有对共享数据的读写（读也要加锁，不能只锁写）；② 用 RAII 锁 `std::lock_guard`/`std::unique_lock` 而非手动 `lock()`/`unlock()`，保证异常和提前 return 时也能解锁；③ 简单计数/标志用 `std::atomic` 免锁更高效；④ 尽量缩小临界区、减少共享（用线程局部存储或消息传递）。验证工具：编译时加 `-fsanitize=thread`（ThreadSanitizer）运行可自动检测竞争。注意死锁防范——多把锁要按固定顺序获取，或用 `std::scoped_lock` 一次性锁定多个 mutex。
  - [ ] 理解锁粒度对性能的影响。
    > **知识讲解**：锁粒度指临界区保护的范围大小。**粗粒度**（一把大锁保护整个数据结构/整个循环）实现简单、不易出错，但并发度低——多线程互相阻塞，无法真正并行。**细粒度**（分段锁、每个数据项独立锁）并发度高、吞吐更好，但实现复杂、易死锁、且频繁加解锁本身有开销。权衡原则：**先保证正确性（粗粒度），再根据性能瓶颈逐步细化**；临界区应尽量短，把耗时操作（I/O、计算）移到锁外。反面陷阱：粒度太细导致加解锁次数暴增，反而比不加锁还慢；或为追求并发引入多锁交叉，造成死锁。生产环境常用分段锁（如并发哈希表分 N 个桶各自加锁）平衡二者。
  - [ ] 知道条件变量必须配合 while 循环检查谓词（防虚假唤醒）。
    > **知识讲解**：条件变量 `std::condition_variable` 用于线程间“等待某条件成立”的协作。**必须用 `while` 而非 `if` 检查谓词**，原因有二：① **虚假唤醒（spurious wakeup）**——即使没有线程 `notify`，`wait` 也可能偶然返回，`if` 会在条件未满足时错误地继续执行；② **唤醒后条件可能又被改变**——多个等待线程被 `notify_all` 唤醒后竞争同一资源，先到的线程消费了数据，后到的线程条件再次不成立，`while` 能让它重新等待。推荐写法：`cv.wait(lock, []{ return predicate; })`——这个重载内部就是 `while (!predicate()) wait(lock);`，等价且更简洁。注意 `wait` 期间会自动释放锁、被唤醒后重新获取锁，因此调用前必须持有 `unique_lock`；`notify` 应在修改谓词之后、可在解锁前或解锁后调用（解锁后 notify 可减少被唤醒线程立即阻塞在锁上的概率）。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 4.4 互斥锁与条件变量练习框架 ===
// 项目结构:
// 4-4-mutex-cv/
// ├── SafeQueue.h
// ├── SimpleThreadPool.h
// ├── SimpleThreadPool.cpp
// ├── SyncExercises.h
// ├── SyncExercises.cpp
// └── main.cpp

// ---------- SafeQueue.h ----------
#pragma once
#include <mutex>
#include <queue>

// 练习2: 线程安全队列
template <typename T>
class SafeQueue {
public:
    void push(const T& val) {
        // TODO: std::lock_guard<std::mutex> lock(mtx_); q_.push(val);
    }

    bool pop(T& val) {
        // TODO: std::lock_guard<std::mutex> lock(mtx_);
        //       若 q_.empty() 返回 false；否则 val = q_.front(); q_.pop(); 返回 true
        return false;
    }

    bool empty() const {
        // TODO: std::lock_guard<std::mutex> lock(mtx_); return q_.empty();
        return true;
    }

private:
    std::queue<T> q_;
    mutable std::mutex mtx_;
};

// ---------- SimpleThreadPool.h ----------
#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// 练习4&5: 任务队列 / 简易线程池
class SimpleThreadPool {
public:
    explicit SimpleThreadPool(int numThreads);
    ~SimpleThreadPool();

    SimpleThreadPool(const SimpleThreadPool&) = delete;
    SimpleThreadPool& operator=(const SimpleThreadPool&) = delete;

    void submit(std::function<void()> task);

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_ = false;
};

// ---------- SimpleThreadPool.cpp ----------
#include "SimpleThreadPool.h"
#include <utility>

SimpleThreadPool::SimpleThreadPool(int numThreads) {
    // TODO: 创建 numThreads 个工作线程
    // 工作线程循环: std::unique_lock + cv_.wait + 取任务 + 执行
}

SimpleThreadPool::~SimpleThreadPool() {
    // TODO: stop_ = true; cv_.notify_all(); 并 join 所有线程
}

void SimpleThreadPool::submit(std::function<void()> task) {
    // TODO: std::lock_guard + tasks_.push(std::move(task)) + cv_.notify_one()
}

// ---------- SyncExercises.h ----------
#pragma once

// 练习1: mutex 保护累加操作
void safeAccumulate();

// 练习3: 完整生产者消费者
void producerConsumer();

// ---------- SyncExercises.cpp ----------
#include "SyncExercises.h"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

void safeAccumulate() {
    int sum = 0;
    std::mutex mtx;
    auto add = [&]() {
        for (int i = 0; i < 100000; ++i) {
            // TODO: std::lock_guard<std::mutex> lock(mtx); ++sum;
        }
    };
    // TODO: 创建多个 std::thread 调用 add，join 后打印 sum
}

void producerConsumer() {
    std::queue<int> buffer;
    constexpr std::size_t MAX_SIZE = 10;
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

    auto producer = [&]() {
        for (int i = 0; i < 50; ++i) {
            // TODO: std::unique_lock + cv.wait(buffer 未满) + push + cv.notify_one
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    };

    auto consumer = [&]() {
        while (true) {
            // TODO: std::unique_lock + cv.wait(buffer 非空 || done)
            if (done && buffer.empty()) break;
            // TODO: front + pop + notify_one + unlock + 打印
        }
    };
    // TODO: 创建线程并 join
}

// ---------- main.cpp ----------
#include "SafeQueue.h"
#include "SimpleThreadPool.h"
#include "SyncExercises.h"

int main() {
    // TODO: 调用以上函数与类进行测试
    return 0;
}
```

</details>

## 4.5 Socket 网络编程

- **练习目标**：理解 TCP 通信全流程；掌握 socket/bind/listen/accept/connect/send/recv。
- **练习任务**：
    1. TCP 回声服务器：客户端发消息，服务器原样返回。
    2. TCP 聊天客户端与服务器。
    3. 多客户端服务器：广播消息给所有连接。
    4. 简易 HTTP 服务器：浏览器访问返回固定 HTML。
    5. 命令执行服务器：客户端发命令，服务器执行并返回结果。
- **巩固标准**：
  - [ ] 能独立完成 TCP 客户端/服务器通信。
    > **知识讲解**：TCP 通信基于 socket API，服务器端流程：`socket()` 创建套接字 → `setsockopt(SO_REUSEADDR)` 允许地址重用（避免 TIME_WAIT 端口占用）→ `bind()` 绑定 IP+端口 → `listen()` 转为监听 → `accept()` 阻塞等待并接受客户端连接（返回新的连接套接字）→ 用该套接字 `recv`/`send` 收发数据 → `close()`。客户端流程更简单：`socket()` → `connect()` 连接服务器 → `recv`/`send` → `close()`。地址用 `sockaddr_in` 填充（`sin_family=AF_INET`、`sin_port=htons(port)`、`sin_addr.s_addr=inet_addr(...)`），注意**网络字节序**（大端）转换 `htons`/`htonl`/`ntohs`/`ntohl`。`recv`/`send` 用 `char` 缓冲区，`recv` 返回实际接收字节数（0 表示对端关闭，-1 出错），必须按实际长度处理而非假定缓冲区满。
  - [ ] 理解三次握手/四次挥手在 API 层面的体现。
    > **知识讲解**：**三次握手（建立连接）** 在 API 层由内核完成：客户端 `connect()` 主动发起，发送 SYN；服务器 `listen()` 后内核被动响应 SYN+ACK；客户端收到后回 ACK，`connect()` 返回成功，服务器 `accept()` 返回新连接套接字。因此应用层看不到握手细节，`connect` 返回即表示连接已建立。**四次挥手（断开连接）**：主动方 `close()` 发送 FIN，进入 FIN_WAIT；被动方 `recv()` 返回 0（表示收到对端 FIN），随后 `close()` 发送自己的 FIN；主动方收到后进入 **TIME_WAIT**，等待 2MSL 后才真正关闭。TIME_WAIT 存在的意义：确保最后的 ACK 能重传、让旧连接的滞留报文在网络中消散。这也是为什么服务器频繁重启会遇到“Address already in use”——需 `SO_REUSEADDR` 解决。
  - [ ] 知道粘包问题及简单解决方案。
    > **知识讲解**：TCP 是**面向字节流**的协议，没有“消息边界”概念——发送端多次 `send` 的数据可能被合并成一次 `recv` 收到（粘包），或一次 `send` 的数据被拆成多次 `recv`（拆包）。这不是 bug，而是流式协议的固有特性。**解决方案**（本质都是应用层自己划定消息边界）：① **固定长度**——每条消息定长，不足补空，简单但浪费；② **分隔符**——用特殊字符（如 `\n`）分隔消息，接收端读到分隔符为一条，适合文本协议（如 HTTP 头部、Redis RESP）；③ **长度前缀（最常用）**——消息头先发送 4 字节表示消息体长度，接收端先读定长头部得到 N，再精确读 N 字节体。实现时接收端需维护缓冲区，循环 `recv` 直到凑齐一条完整消息，处理“读到半条”的情况。UDP 是面向数据报的，天然有边界、不会粘包。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 4.5 Socket练习框架 ===
// 项目结构:
// 4-5-socket/
// ├── NetConstants.h
// ├── EchoServer.h / EchoServer.cpp
// ├── EchoClient.h / EchoClient.cpp
// ├── ChatServer.h / ChatServer.cpp
// ├── MultiClientServer.h / MultiClientServer.cpp
// ├── HttpServer.h / HttpServer.cpp
// ├── CmdServer.h / CmdServer.cpp
// └── main.cpp

// ---------- NetConstants.h ----------
#pragma once

inline constexpr int PORT = 8080;
inline constexpr int BUF_SIZE = 1024;

// ---------- EchoServer.h ----------
#pragma once

// 练习1: TCP 回声服务器
void echoServer();

// ---------- EchoServer.cpp ----------
#include "EchoServer.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void echoServer() {
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    // TODO: bind, listen
    int client_fd = ::accept(server_fd, nullptr, nullptr);
    char buf[BUF_SIZE];
    while (true) {
        // TODO: recv -> send 回声，直到客户端断开
    }
    ::close(client_fd);
    ::close(server_fd);
}

// ---------- EchoClient.h ----------
#pragma once

void echoClient();

// ---------- EchoClient.cpp ----------
#include "EchoClient.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void echoClient() {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    // TODO: connect
    // TODO: send -> recv 回声
    ::close(fd);
}

// ---------- ChatServer.h ----------
#pragma once

// 练习2: TCP 聊天（服务器端 + 客户端）
void chatServer();
void chatClient();

// ---------- ChatServer.cpp ----------
#include "ChatServer.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

void chatServer() {
    // TODO: accept 后循环 recv 并打印，同时接收用户输入 send 给客户端
}

void chatClient() {
    // TODO: 同时 recv 显示消息 + 发送用户输入
}

// ---------- MultiClientServer.h ----------
#pragma once

// 练习3: 多客户端服务器
void multiClientServer();

// ---------- MultiClientServer.cpp ----------
#include "MultiClientServer.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

void multiClientServer() {
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    // TODO: bind, listen
    while (true) {
        int client_fd = ::accept(server_fd, nullptr, nullptr);
        // TODO: 为每个客户端创建新 std::thread 处理
    }
}

// ---------- HttpServer.h ----------
#pragma once

// 练习4: 简易 HTTP 服务器
void httpServer();

// ---------- HttpServer.cpp ----------
#include "HttpServer.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void httpServer() {
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    // TODO: bind, listen, accept
    char buf[BUF_SIZE];
    // TODO: recv 请求
    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<h1>Hello from C++ Server!</h1>";
    // TODO: send(response), close
}

// ---------- CmdServer.h ----------
#pragma once

// 练习5: 命令执行服务器
void cmdServer();

// ---------- CmdServer.cpp ----------
#include "CmdServer.h"
#include "NetConstants.h"

#include <cstdio>
#include <iostream>

void cmdServer() {
    // TODO: recv 命令字符串，popen 执行，读取结果 send 回客户端
}

// ---------- main.cpp ----------
#include "ChatServer.h"
#include "CmdServer.h"
#include "EchoClient.h"
#include "EchoServer.h"
#include "HttpServer.h"
#include "MultiClientServer.h"

int main() {
    // TODO: 选择服务器或客户端模式测试
    return 0;
}
```

</details>

<details>
<summary>📖 使用说明</summary>

编译后运行程序，根据菜单选择练习：

```
=== Socket 练习测试 ===
1. EchoServer (TCP 回声服务器)
2. EchoClient (TCP 回声客户端)
3. ChatServer (TCP 聊天服务器)
4. ChatClient (TCP 聊天客户端)
5. MultiClientServer (多客户端服务器)
6. CmdServer (命令执行服务器)
7. HttpServer (HTTP 服务器)
请选择: 
```

> 端口固定为 **8080**，需要多个终端窗口配合测试。

#### 练习 1：回声服务器 + 客户端

需要 **2 个终端**：

| 步骤 | 终端 A（服务器） | 终端 B（客户端） |
|------|------------------|------------------|
| 1 | 运行程序，输入 `1` 启动 EchoServer | |
| 2 | 等待连接... | 运行程序，输入 `2` 启动 EchoClient |
| 3 | 看到 `Received: Hello, server!` | 看到 `Received: Hello, server!` |

客户端发送固定消息 `Hello, server!`，服务器收到后打印，程序自动结束。

#### 练习 2：聊天服务器 + 客户端

需要 **2 个终端**：

| 步骤 | 终端 A（服务器） | 终端 B（客户端） |
|------|------------------|------------------|
| 1 | 运行程序，输入 `3` 启动 ChatServer | |
| 2 | 等待连接... | 运行程序，输入 `4` 启动 ChatClient |
| 3 | 输入 `你好` 回车 → 客户端收到 | 看到 `Received: 你好` |
| 4 | | 输入 `你好啊` 回车 → 服务器收到 |
| 5 | 看到 `Received: 你好啊` | |
| 6 | `Ctrl+D` 结束 | `Ctrl+D` 结束 |

双方可同时收发消息，按 `Ctrl+D` 结束输入。

#### 练习 3：多客户端服务器

需要 **3+ 个终端**：

| 步骤 | 终端 A（服务器） | 终端 B（客户端 1） | 终端 C（客户端 2） |
|------|------------------|--------------------|--------------------|
| 1 | 运行程序，输入 `5` | | |
| 2 | 等待连接... | `nc localhost 8080` | |
| 3 | 看到 `[Client X] Connected` | | `nc localhost 8080` |
| 4 | 看到 `[Client Y] Connected` | 输入 `hello` 回车 | |
| 5 | 看到 `[Client X] Received: hello` | | 输入 `world` 回车 |
| 6 | 看到 `[Client Y] Received: world` | | |
| 7 | `Ctrl+C` 停止服务器 | `Ctrl+C` 断开 | `Ctrl+C` 断开 |

服务器持续运行，每个客户端连接后创建独立线程处理，用 `nc`（netcat）模拟客户端。

#### 练习 4：命令执行服务器

需要 **2 个终端**：

| 步骤 | 终端 A（服务器） | 终端 B（客户端） |
|------|------------------|--------------------|
| 1 | 运行程序，输入 `6` | |
| 2 | 等待连接... | `nc localhost 8080` |
| 3 | | 输入 `ls -la` 回车 |
| 4 | 看到 `Received: ls -la` | 收到命令执行结果 |
| 5 | 程序自动结束 | 连接关闭 |

客户端发送一条命令，服务器用 `popen` 执行后将结果返回。

#### 练习 5：HTTP 服务器

需要 **1 个终端 + 浏览器**：

| 步骤 | 操作 |
|------|------|
| 1 | 运行程序，输入 `7` 启动 HttpServer |
| 2 | 打开浏览器，访问 `http://localhost:8080` |
| 3 | 终端显示收到的 HTTP 请求内容 |
| 4 | 浏览器显示 `Hello from C++ Server!` |
| 5 | 程序自动结束 |

服务器接收一次 HTTP 请求，返回固定 HTML 页面后退出。

</details>

## 4.6 epoll 高并发 I/O

- **练习目标**：理解 I/O 多路复用演进；掌握 epoll 事件驱动模型；理解高并发服务器基础。
- **练习任务**：
    1. 用 `select` 实现多客户端服务器（理解 fd_set 限制）。
    2. 用 `poll` 实现多客户端服务器（理解改进点）。
    3. 用 `epoll` 实现多客户端服务器（ET/LT 模式）。
    4. 对比三者高并发表现。
    5. 实现 epoll 聊天服务器：转发消息。
    6. 实现非阻塞 epoll 服务器。
- **巩固标准**：
  - [ ] 理解 epoll 相比 select/poll 的优势（O(1) 事件通知、无 fd 数量限制）。
    > **知识讲解**：`select`/`poll` 的痛点：① 每次调用都要把**全部** fd 集合从用户态拷贝到内核态；② 内核需**遍历所有** fd 检查就绪状态，复杂度 O(n)；③ 返回后应用还要**再遍历一遍**找出就绪的 fd；④ `select` 有 `FD_SETSIZE`（默认 1024）上限，`poll` 虽无硬上限但同样 O(n)。`epoll` 的优势：① `epoll_ctl` 注册 fd 到内核**红黑树**，只需注册一次，无需每次全量传递；② 就绪 fd 通过**回调**放入就绪链表，`epoll_wait` 直接返回就绪列表，无需遍历全部，复杂度 O(1)（与总 fd 数无关，只与就绪数相关）；③ 无 fd 数量限制（受系统内存约束）。因此 epoll 在**连接数多但活跃比例低**的高并发场景（如万级连接的服务器）性能远超 select/poll，是 Linux 高并发 I/O 的首选。
  - [ ] 掌握 ET 模式下非阻塞 IO + 循环读写的必要性。
    > **知识讲解**：epoll 有两种触发模式：**LT（水平触发，默认）**——只要 fd 还有数据未读完，`epoll_wait` 就会**持续**通知，编程简单，一次读不完下次还会提醒；**ET（边缘触发）**——仅在状态**发生变化**（从无数据到有数据）时通知**一次**，因此必须在这次通知里**循环读/写直到 `EAGAIN`**，否则剩余数据不会再触发通知，造成数据滞留。ET 模式**必须配合非阻塞 fd**（`O_NONBLOCK`）：因为循环读到没数据时，阻塞 fd 会一直卡住，而非阻塞 fd 会立即返回 `EAGAIN`/`EWOULDBLOCK`，据此判断“读完了”退出循环。写法：`while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) { 处理 }`，直到 `n == -1 && errno == EAGAIN`。ET 减少了 `epoll_wait` 触发次数、效率更高，但编程更复杂易错。
  - [ ] 能搭建可支撑万级并发的服务器框架。
    > **知识讲解**：万级并发服务器的核心是 **I/O 多路复用 + 非阻塞 I/O + 单线程或少量线程的事件循环**，即 Reactor 模式。基本框架：① 创建监听 socket 并设为非阻塞，`bind`+`listen`；② 创建 epoll 实例（`epoll_create1(0)`，优于旧的 `epoll_create`），把监听 fd 以 `EPOLLIN` 注册；③ 进入事件循环 `while(true) { epoll_wait(...) }`；④ 若就绪的是监听 fd，说明有新连接，`accept` 后把新连接 fd 设为非阻塞并注册到 epoll；⑤ 若就绪的是普通连接 fd，`recv` 处理数据，对端关闭（recv 返回 0）则 `epoll_ctl(DEL)` + `close`。这样单线程即可管理成千上万连接（因为不再“一连接一线程”，避免了线程创建/切换和内存开销）。进一步提升可用**主从 Reactor 多线程**（主 reactor 只管 accept，从 reactor 线程池处理 I/O），客户端集合常用 `unordered_set` 存储以支持 O(1) 增删查。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 4.6 epoll练习框架 ===
// 项目结构:
// 4-6-epoll/
// ├── NetConstants.h
// ├── SelectServer.h / SelectServer.cpp
// ├── PollServer.h / PollServer.cpp
// ├── EpollServer.h / EpollServer.cpp
// ├── EpollChatServer.h / EpollChatServer.cpp
// └── main.cpp

// ---------- NetConstants.h ----------
#pragma once

inline constexpr int PORT = 8080;
inline constexpr int BUF_SIZE = 1024;
inline constexpr int MAX_EVENTS = 1024;

// ---------- SelectServer.h ----------
#pragma once

// 练习1: select 多客户端服务器
void selectServer();

// ---------- SelectServer.cpp ----------
#include "SelectServer.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

void selectServer() {
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    // TODO: bind, listen
    fd_set readfds;
    // TODO: FD_ZERO / FD_SET 初始化，select 循环，遍历 fd_set 处理新连接和数据
}

// ---------- PollServer.h ----------
#pragma once

// 练习2: poll 多客户端服务器
void pollServer();

// ---------- PollServer.cpp ----------
#include "PollServer.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

void pollServer() {
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    // TODO: bind, listen
    std::vector<struct pollfd> fds;
    // TODO: poll 循环，处理 POLLIN 事件
}

// ---------- EpollServer.h ----------
#pragma once

// 练习3: epoll 多客户端服务器（LT 模式）
void epollServerLT();

// 练习3 补充: epoll ET 模式
void epollServerET();

// 练习6: 非阻塞 epoll 服务器
void nonBlockingEpollServer();

// ---------- EpollServer.cpp ----------
#include "EpollServer.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

void epollServerLT() {
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    // TODO: bind, listen, 设置非阻塞
    int epoll_fd = ::epoll_create1(0);
    // TODO: epoll_ctl 添加 server_fd
    struct epoll_event events[MAX_EVENTS];
    while (true) {
        int n = ::epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == server_fd) {
                // TODO: accept 新连接，epoll_ctl 添加 client_fd
            } else {
                // TODO: recv 数据，转发或回声
            }
        }
    }
}

void epollServerET() {
    // TODO: epoll_ctl 时设置 EPOLLET 标志
    // TODO: recv 时循环读取直到 EAGAIN
}

void nonBlockingEpollServer() {
    // TODO: 所有 fd 设置 O_NONBLOCK
    // TODO: ET 模式下循环 read/write 直到 EAGAIN
}

// ---------- EpollChatServer.h ----------
#pragma once

// 练习5: epoll 聊天服务器
void epollChatServer();

// ---------- EpollChatServer.cpp ----------
#include "EpollChatServer.h"
#include "NetConstants.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_set>

void epollChatServer() {
    // TODO: 维护客户端列表，收到消息后转发给其他所有客户端
}

// ---------- main.cpp ----------
#include "EpollChatServer.h"
#include "EpollServer.h"
#include "PollServer.h"
#include "SelectServer.h"

int main() {
    // TODO: 选择一种模式启动服务器测试
    return 0;
}
```

</details>

<details>
<summary>🧪 测试操作步骤</summary>

> 端口固定为 **8080**，需要多个终端窗口配合测试。所有练习共用同一套编译流程，仅菜单选项不同。

#### 练习 1：select 多客户端服务器

需要 **3 个终端**：

| 步骤 | 终端 A（服务器） | 终端 B（客户端 1） | 终端 C（客户端 2） |
|------|------------------|--------------------|--------------------|
| 1 | 运行程序，输入 `1` | | |
| 2 | 阻塞等待中（无提示） | `nc localhost 8080` | |
| 3 | 无输出（静默 accept） | | `nc localhost 8080` |
| 4 | 无输出 | 输入 `hello` 回车 | |
| 5 | 看到 `Received: hello` | 无回复（服务器只接收不回传） | 输入 `world` 回车 |
| 6 | 看到 `Received: world` | | |
| 7 | `Ctrl+C` 停止服务器 | `Ctrl+C` 断开 | `Ctrl+C` 断开 |

select/poll 服务器只打印 `Received: ...`，不打印连接/断开信息，也不向客户端回传数据。

#### 练习 2：poll 多客户端服务器

与练习 1 步骤相同，运行程序时输入 `2`。验证 poll 无 fd 数量限制（相比 select 的 FD_SETSIZE=1024）。

#### 练习 3：epoll LT 模式

需要 **3 个终端**：

| 步骤 | 终端 A（服务器） | 终端 B（客户端 1） | 终端 C（客户端 2） |
|------|------------------|--------------------|--------------------|
| 1 | 运行程序，输入 `3` | | |
| 2 | 阻塞等待中 | `nc localhost 8080` | |
| 3 | 无输出 | | `nc localhost 8080` |
| 4 | 无输出 | 输入 `hello` 回车 | |
| 5 | 看到 `Received: hello` | 收到 `Echo(resp): hello` | 输入 `world` 回车 |
| 6 | 看到 `Received: world` | | 收到 `Echo(resp): world` |
| 7 | `Ctrl+C` 停止服务器 | `Ctrl+C` 断开 | `Ctrl+C` 断开 |

epoll 回声服务器会向客户端回传 `Echo(resp): ...`。

#### 练习 3 补充：epoll ET 模式

与练习 3（LT 模式）步骤相同，运行程序时输入 `4`。功能一致，但底层使用边沿触发 + 非阻塞循环读取。

#### 练习 5：epoll 聊天服务器

需要 **3 个终端**：

| 步骤 | 终端 A（服务器） | 终端 B（客户端 1） | 终端 C（客户端 2） |
|------|------------------|--------------------|--------------------|
| 1 | 运行程序，输入 `5` | | |
| 2 | 阻塞等待中 | `nc localhost 8080` | |
| 3 | 看到 `[Client X] connected. Total: 1` | | `nc localhost 8080` |
| 4 | 看到 `[Client Y] connected. Total: 2` | 输入 `你好` 回车 | |
| 5 | 看到 `[Client X] 你好` | | 收到 `你好` |
| 6 | | | 输入 `世界` 回车 |
| 7 | 看到 `[Client Y] 世界` | 收到 `世界` | |
| 8 | | `Ctrl+C` 断开 | |
| 9 | 看到 `[Client X] disconnected. Total: 1` | | |
| 10 | | | `Ctrl+C` 断开 |
| 11 | 看到 `[Client Y] disconnected. Total: 0` | | |
| 12 | `Ctrl+C` 停止服务器 | | |

与回声服务器不同：客户端 B 发的消息只转发给客户端 C，不会回给 B 自己。

#### 练习 6：非阻塞 epoll 服务器

与练习 3（LT 模式）步骤相同，运行程序时输入 `6`。所有 fd 均设为 `O_NONBLOCK`，ET 模式下循环读写直到 `EAGAIN`。

</details>

---

# 阶段五：Linux 系统编程进阶

> **定位**：综合项目的前置知识之一。补齐阶段四中未覆盖的 Linux 系统级 API，为后续网络服务器、多进程架构、高性能 IO 打基础。

## 5.1 信号处理

- **练习目标**：
    - 理解信号的本质（异步事件通知）与常见信号（SIGINT / SIGTERM / SIGKILL / SIGCHLD / SIGPIPE / SIGHUP / SIGALRM）。
    - 掌握 `signal()` 与 `sigaction()` 的区别，能说出为什么生产代码应优先使用 `sigaction`。
    - 理解信号的阻塞（`sigprocmask`）、挂起（`sigsuspend`）与 `signalfd` 的异步转同步用法。
    - 掌握 SIGPIPE 的成因（向已关闭的 socket 写数据）与处理方式（忽略或捕获），避免服务器意外崩溃。
    - 掌握 SIGCHLD 的处理：配合 `waitpid` 回收僵尸子进程。
- **练习任务**：
    1. 用 `sigaction` 注册 SIGINT / SIGTERM 处理器，实现服务器的"优雅退出"标志位。
    2. 忽略 SIGPIPE，验证向已关闭 socket 写数据不再导致进程退出。
    3. 父进程 fork 多个子进程，用 SIGCHLD + `waitpid(-1, ..., WNOHANG)` 循环回收所有退出子进程，避免僵尸进程。
    4. 使用 `signalfd` + epoll 把信号处理纳入事件循环（不阻塞主线程）。
- **巩固标准**：
    - [ ] 能说出 `signal()` 在不同 Unix 实现中的语义差异，并解释为什么 `sigaction()` 更可移植。
    > **知识讲解**：`signal()` 的早期 Unix 实现中，信号处理函数可能在执行期间被重置为 `SIG_DFL`（即"一次性"语义），且 `sleep()` 等系统调用被信号中断后不会自动重启。不同系统（Linux、BSD、System V）对这些行为的实现各不相同，导致同一段代码在不同平台上表现不一致。`sigaction()` 通过显式的 `struct sigaction` 结构体精确控制：`sa_flags` 中的 `SA_RESETHAND` 决定是否一次性、`SA_RESTART` 决定是否自动重启被中断的系统调用、`SA_SIGINFO` 决定是否传递详细信号信息——所有行为都由程序员显式指定，不依赖平台默认语义，因此是可移植的首选。
    - [ ] 能解释 SIGPIPE 在长连接服务器中的触发场景，并给出至少两种处理方式。
    > **知识讲解**：SIGPIPE 的触发场景：TCP 连接中，客户端异常断开（如 `kill -9`、网线拔掉），服务端不知情仍调用 `send()`/`write()` 向已关闭的 socket 写数据，内核先发送 RST，再次写入时内核产生 SIGPIPE 信号，默认行为是**终止进程**。处理方式：① `signal(SIGPIPE, SIG_IGN)` 忽略信号，`write()` 返回 -1 且 `errno = EPIPE`，程序可优雅处理错误；② `send()` 使用 `MSG_NOSIGNAL` 标志，单次调用不触发 SIGPIPE；③ 用 `sigaction()` 捕获 SIGPIPE 在 handler 中处理。长连接服务器（如聊天服务器、数据库连接池）必须处理 SIGPIPE，否则一个客户端断开就会导致整个服务崩溃。
    - [ ] 能写出回收多个子进程的正确循环（`while (waitpid(-1, &status, WNOHANG) > 0)`），并解释为什么必须循环。
    > **知识讲解**：信号可能合并——如果多个子进程几乎同时退出，内核可能只向父进程投递一次 SIGCHLD（标准信号不支持排队）。如果 handler 中只调用一次 `waitpid()`，只能回收一个僵尸进程，其余子进程继续处于僵尸状态。因此必须用 `while` 循环反复调用 `waitpid(-1, &status, WNOHANG)`，直到返回 0（没有更多已退出的子进程）或 -1（出错或无子进程）。`WNOHANG` 保证没有僵尸时立即返回而非阻塞。`-1` 表示等待任意子进程。返回值 > 0 是回收到的子进程 PID，`status` 可通过 `WIFEXITED`/`WEXITSTATUS` 宏解析退出状态。
    - [ ] 能对比"信号处理函数"与"signalfd + epoll"两种方案的优缺点。
    > **知识讲解**：**信号处理函数（sigaction）**：优点是实现简单，适合"设标志位退出"的简单场景；缺点是 handler 中只能调用 async-signal-safe 函数（不能用 `std::cout`、`malloc`、`std::vector` 等），多线程环境下信号可能投递到任意线程，处理逻辑受限。**signalfd + epoll**：先用 `sigprocmask()` 阻塞信号（防止内核默认处理），再通过 `signalfd()` 将信号转为文件描述符可读事件，纳入 epoll 事件循环统一处理。优点是信号处理在主循环中执行，没有 async-signal-safe 限制，可以使用所有 C++ 特性；多线程场景下信号只投递到监听的线程，行为可预测。缺点是代码量更多，且需要配合 epoll 事件循环使用。总结：**简单退出场景用 sigaction，事件驱动服务器用 signalfd + epoll**。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 5.1 信号处理 练习框架 ===
// 项目结构:
// 5-1-signal/
// ├── SignalExercises.h
// ├── SignalExercises.cpp
// └── main.cpp

// ---------- SignalExercises.h ----------
#pragma once

// 练习1: 用 sigaction 注册 SIGINT/SIGTERM 处理器，实现优雅退出标志位
void installGracefulQuitHandler();

// 练习2: 忽略 SIGPIPE，验证向已关闭 socket 写数据不会崩溃
void ignoreSigpipeAndTest();

// 练习3: 父进程 fork 多个子进程，用 SIGCHLD + waitpid 循环回收避免僵尸
void reapChildrenWithSigchld();

// 练习4: 用 signalfd + epoll 把信号处理纳入事件循环
void signalFdWithEpoll();

// ---------- SignalExercises.cpp ----------
#include "SignalExercises.h"

#include <atomic>
#include <csignal>
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

// 练习1
static std::atomic<bool> g_quit{false};
static void sigHandler(int /*sig*/, siginfo_t* info, void* /*ucontext*/) {
    // TODO: 设置 g_quit = true，输出收到的信号
}

void installGracefulQuitHandler() {
    // TODO: 用 sigaction 注册 SIGINT / SIGTERM 到 sigHandler
    // TODO: 主循环 while (!g_quit) { sleep(1); }
}

// 练习2
void ignoreSigpipeAndTest() {
    // TODO: signal(SIGPIPE, SIG_IGN) 或 sigaction 忽略
    // TODO: 创建 socketpair，关闭一端，向另一端 write，验证进程未退出
}

// 练习3
static void sigchldHandler(int /*sig*/, siginfo_t* /*info*/, void* /*ucontext*/) {
    // TODO: while (waitpid(-1, nullptr, WNOHANG) > 0) 循环回收所有退出子进程
}

void reapChildrenWithSigchld() {
    // TODO: sigaction 注册 SIGCHLD 到 sigchldHandler
    // TODO: fork 3~5 个子进程，子进程 sleep 随机时间后退出
    // TODO: 父进程 sleep 足够长时间，用 ps 验证无僵尸进程
}

// 练习4
void signalFdWithEpoll() {
    // TODO: sigprocmask 阻塞 SIGINT / SIGTERM
    // TODO: signalfd(-1, &mask, SFD_NONBLOCK) 创建 signalfd
    // TODO: 创建 epoll fd，把 signalfd 加入监听 EPOLLIN
    // TODO: epoll_wait 循环，读到 signalfd 事件时解析 signalfd_siginfo 并退出
}

// ---------- main.cpp ----------
#include "SignalExercises.h"

int main() {
    // TODO: 依次测试以上四个练习
    return 0;
}
```

</details>

## 5.2 内存映射 mmap

- **练习目标**：
    - 理解虚拟内存、页、缺页中断的基本概念。
    - 掌握 `mmap` / `munmap` 的用法：匿名映射（进程间共享内存）与文件映射（高效文件 IO）。
    - 理解 `msync` 的作用：把 mmap 的修改刷回磁盘。
    - 掌握 POSIX 共享内存（`shm_open` / `ftruncate` / `mmap`）实现无亲缘关系进程间通信。
- **练习任务**：
    1. 用 `mmap` 映射一个文件，实现"内存式"读取（对比 `read` 系统调用的性能差异）。
    2. 用匿名 mmap（`MAP_SHARED | MAP_ANONYMOUS`）在父子进程间共享一块计数器内存。
    3. 用 POSIX 共享内存实现两个独立进程之间的字符串传递。
    4. 用 `mmap` 实现一个简单的"共享内存日志"：多进程追加写入，另一进程读取。
- **巩固标准**：
    - [ ] 能说出 `mmap` 相比 `read/write` 的优势（减少一次用户态拷贝、利用页缓存）与适用场景。
      > **知识讲解**：传统 `read()` 系统调用需要两次数据拷贝：① 内核从磁盘读取文件到内核态页缓存（内核缓冲区），② 再从内核缓冲区拷贝到用户态缓冲区（应用程序内存）。`mmap` 将文件直接映射到进程的用户空间虚拟地址，应用程序通过**指针直接访问**映射区域的内存，省去了"内核缓冲区→用户缓冲区"这一次拷贝，零额外内存分配。同时 `mmap` 利用操作系统的**页缓存**机制——文件已在缓存中时直接映射物理页，无需真正读盘；大文件中只访问的部分会被加载（按需调页），未访问部分不占内存。适用场景：大文件随机读取（如搜索引擎索引文件、数据库数据文件）、共享内存通信；不适用场景：小文件（映射本身有系统调用开销，可能反而比 `read` 慢）、需要顺序写一次的文件（`write` 更简单直接）。
    - [ ] 能解释 `MAP_SHARED` 与 `MAP_PRIVATE`（写时复制）的区别。
      > **知识讲解**：`MAP_SHARED` 表示映射区域由所有映射该文件的进程**共享**——任一进程对映射内存的写入会**直接写回底层文件**（或通过 `msync` 显式刷盘），其他进程能立即看到修改，适合进程间共享数据和协同写入。`MAP_PRIVATE` 表示**私有映射（写时复制 COW）**——进程读取时看到的是文件原始内容，但一旦写入，内核会为该页创建一份**私有副本**（只复制被写入的那一页，而非整个文件），后续写入只影响副本，**不会修改原文件**，其他进程也看不到。典型用途：只读加载大文件到内存（如加载配置、字典）时用 `MAP_PRIVATE | PROT_READ`，既高效又安全，意外写入也不会破坏原文件；需要多进程共享数据时用 `MAP_SHARED`。
    - [ ] 能说明共享内存配合互斥锁（`pthread_mutex` 放在共享内存中 + `PTHREAD_PROCESS_SHARED`）的必要性。
      > **知识讲解**：共享内存（`mmap MAP_SHARED` 或 `shm_open`）只解决了"多进程看到同一块内存"的问题，但**不提供任何同步机制**——多个进程同时读写共享区域时，与多线程一样存在**数据竞争**（读到的可能是写到一半的中间状态）。因此必须配合互斥锁。但普通 `std::mutex` / `pthread_mutex_t` 默认只能在**同一进程的线程间**工作，无法跨进程使用。要让互斥锁跨进程生效，必须：① 把 `pthread_mutex_t` 对象**放在共享内存区域内**（而非进程私有堆栈），② 初始化时调用 `pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)` 设置跨进程属性，再用该 attr 初始化 mutex。这样多个进程对同一把锁加锁/解锁才能正确互斥。若忘记设置 `PTHREAD_PROCESS_SHARED`，锁只在单进程内有效，跨进程使用时形同虚设，数据竞争依然存在。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 5.2 mmap 练习框架 ===
// 项目结构:
// 5-2-mmap/
// ├── MmapExercises.h
// ├── MmapExercises.cpp
// └── main.cpp

// ---------- MmapExercises.h ----------
#pragma once
#include <cstddef>

// 练习1: 用 mmap 映射文件，实现内存式读取（对比 read 的性能）
void mmapReadFile(const char* path);

// 练习2: 用匿名 mmap 在父子进程间共享计数器
void anonymousMmapCounter();

// 练习3: 用 POSIX 共享内存实现两个独立进程间的字符串传递
void posixSharedMemory();

// 练习4: 用 mmap 实现简单的共享内存日志（多进程追加写，另一进程读）
void sharedMemoryLog();

// ---------- MmapExercises.cpp ----------
#include "MmapExercises.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

// 练习1
void mmapReadFile(const char* path) {
    // TODO: open(path, O_RDONLY)
    // TODO: fstat 获取文件大小
    // TODO: mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0)
    // TODO: 直接遍历内存指针打印内容（对比 read 系统调用）
    // TODO: munmap + close
}

// 练习2
void anonymousMmapCounter() {
    // TODO: mmap(nullptr, 4096, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)
    // TODO: fork 出子进程
    // TODO: 父子进程各自对计数器 ++N 次，最后打印结果验证共享
    // TODO: munmap
}

// 练习3
void posixSharedMemory() {
    // TODO: shm_open("/my_shm", O_CREAT|O_RDWR, 0666)
    // TODO: ftruncate 设置大小
    // TODO: mmap 映射
    // TODO: 写入字符串，另一进程读取
    // TODO: shm_unlink 清理
}

// 练习4
struct LogHeader {
    // TODO: size_t write_pos;  // 当前写入位置
    // TODO: char data[4096 - sizeof(size_t)];
};

void sharedMemoryLog() {
    // TODO: shm_open + ftruncate + mmap 创建共享内存日志
    // TODO: fork 多个子进程，各自用 __sync_fetch_and_add 原子推进 write_pos 并写入日志
    // TODO: 父进程 sleep 后读取整块日志打印
    // TODO: munmap + shm_unlink
}

// ---------- main.cpp ----------
#include "MmapExercises.h"

int main() {
    // TODO: 依次测试以上四个练习
    return 0;
}
```

</details>

## 5.3 进程间通信（IPC）

- **练习目标**：
    - 系统掌握 Linux IPC 的几种方式：管道（pipe / FIFO）、消息队列、共享内存、信号量、信号、socketpair、Unix Domain Socket。
    - 能根据场景（数据量、是否跨机、是否需要同步）选择合适的 IPC。
    - 理解无名管道（pipe）与有名管道（FIFO）的区别。
    - 掌握 System V IPC 与 POSIX IPC 的对应关系。
- **练习任务**：
    1. 用 `pipe()` 实现父子进程间的单向数据传递。
    2. 用 `FIFO` 实现两个独立进程之间的命令通道。
    3. 用 `socketpair()` 实现全双工的父子进程通信（对比 pipe 的半双工）。
    4. 用 POSIX 共享内存 + 互斥锁实现一个简单的"生产者-消费者"模型。
    5. 对比五种 IPC 方式的性能（吞吐量、延迟）与适用场景，写一份小结。
- **巩固标准**：
    - [ ] 能画出"pipe / FIFO / socketpair / 共享内存 / 消息队列"的数据流图。
      > **知识讲解**：五种 IPC 方式的数据流路径各不相同。**pipe（无名管道）**：父进程 `pipe()` 创建一对 fd（read/write），`fork` 后子进程继承 fd，关闭不需要的端，形成单向数据流（父→子或子→父），生命周期随进程结束。**FIFO（有名管道）**：`mkfifo` 在文件系统创建一个特殊文件节点，两个**无亲缘关系**的进程通过 `open` 同一 FIFO 路径建立通道，数据仍经内核缓冲区传递，写端关闭后读端收到 EOF。**socketpair**：`socketpair(AF_UNIX, SOCK_STREAM)` 创建一对互联的 socket fd，`fork` 后父子各持一端，支持**全双工**双向读写（对比 pipe 的半双工），数据在内核缓冲区中流转，不经过网络栈。**共享内存（mmap/shm_open）**：多个进程把同一块物理内存映射到各自虚拟地址空间，数据**零拷贝**直接在内存中读写，最快但无同步——需配合信号量或互斥锁。**消息队列（System V `msgget`/POSIX `mq_open`）**：数据以"消息"为单位存放在内核队列中，进程通过 `msgsnd`/`msgrcv` 收发，自带消息边界，但每次读写需内核拷贝（用户态↔内核态），吞吐量低于共享内存。
    - [ ] 能解释为什么"共享内存 + 同步原语"是本地最快 IPC，而 Unix Domain Socket 是本地最通用的 IPC。
      > **知识讲解**：**共享内存最快**的原因：数据写入后其他进程**直接通过指针读取**，全程零拷贝——不需要像 pipe/FIFO/消息队列那样把数据从写进程的用户态拷贝到内核缓冲区、再从内核缓冲区拷贝到读进程的用户态（两次拷贝）。唯一开销是同步原语（mutex/semaphore）的加解锁。但共享内存"只管存不管序"，必须外部同步，且只适用于同一台机器上的进程。**Unix Domain Socket 最通用**的原因：它使用 `AF_UNIX` 地址族，数据在内核中传递（不经过网络栈），性能接近 pipe；但同时支持 `SOCK_STREAM`（流式、类 TCP 语义）和 `SOCK_DGRAM`（数据报、类 UDP 语义，保留消息边界），接口与 TCP socket 完全一致（`send`/`recv`/`bind`/`listen`/`accept`），可以无缝替换网络 socket——同一套代码改个地址就能从本地通信切换到跨机通信，是本地最通用的 IPC。
    - [ ] 能说出 pipe 的容量限制（`PIPE_BUF`）以及超过容量时的阻塞行为。
      > **知识讲解**：Linux 中 pipe 的内核缓冲区大小有限——`PIPE_BUF`（通常 4096 字节）是保证原子写入的最大长度：写入 ≤ `PIPE_BUF` 字节时，内核保证该次写入是原子的（不会被其他进程的写入交错插入）；写入 > `PIPE_BUF` 时，内核可能拆分为多次写入，与其他进程的写入可能交错。pipe 的总缓冲区大小（`/proc/sys/fs/pipe-max-size`，默认 64KB）是管道能缓存的最大数据量。当管道已满时，`write()` 会**阻塞**直到读端取走数据腾出空间；当管道为空时，`read()` 会**阻塞**直到有数据写入。写端全部关闭后，读端 `read()` 返回 0（EOF）；读端全部关闭后，写端 `write()` 触发 **SIGPIPE** 信号（默认终止进程）。利用这些阻塞特性可以实现简单的进程间同步——生产者写满时自动等待消费者消费。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 5.3 IPC 练习框架 ===
// 项目结构:
// 5-3-ipc/
// ├── IpcExercises.h
// ├── IpcExercises.cpp
// └── main.cpp

// ---------- IpcExercises.h ----------
#pragma once

// 练习1: 用 pipe 实现父子进程间的单向数据传递
void pipeParentChild();

// 练习2: 用 FIFO 实现两个独立进程间的命令通道
void fifoChannel();

// 练习3: 用 socketpair 实现全双工的父子进程通信
void socketpairDuplex();

// 练习4: 用 POSIX 共享内存 + 互斥锁实现生产者-消费者
void producerConsumer();

// 练习5: 对比五种 IPC 方式的性能与适用场景，写一份小结
void ipcBenchmark();

// ---------- IpcExercises.cpp ----------
#include "IpcExercises.h"

#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

// 练习1
void pipeParentChild() {
    int pipefd[2];
    // TODO: pipe(pipefd)
    // TODO: fork
    // TODO: 子进程关闭读端，向写端发送数据
    // TODO: 父进程关闭写端，从读端读取并打印
}

// 练习2
void fifoChannel() {
    const char* path = "/tmp/my_fifo";
    // TODO: mkfifo(path, 0666)
    // TODO: 启动两个终端，一个 open(O_WRONLY) 写命令，另一个 open(O_RDONLY) 读命令
    // TODO: unlink 清理
}

// 练习3
void socketpairDuplex() {
    int sv[2];
    // TODO: socketpair(AF_UNIX, SOCK_STREAM, 0, sv)
    // TODO: fork
    // TODO: 父子进程各自关闭一端，双向读写
}

// 练习4
struct SharedData {
    pthread_mutex_t mtx;
    int buffer[8];
    int head, tail, count;
};

void producerConsumer() {
    // TODO: shm_open + ftruncate + mmap 创建共享内存
    // TODO: pthread_mutexattr_setpshared(PTHREAD_PROCESS_SHARED) 初始化互斥锁
    // TODO: fork 出生产者与消费者进程
    // TODO: 生产者加锁 → 写 buffer → 解锁
    // TODO: 消费者加锁 → 读 buffer → 解锁
}

// 练习5
void ipcBenchmark() {
    // TODO: 分别对 pipe / FIFO / socketpair / 共享内存 / 消息队列 发送 1MB 数据
    // TODO: 记录耗时，输出对比表格
    // TODO: 写一份小结：各自适用场景
}

// ---------- main.cpp ----------
#include "IpcExercises.h"

int main() {
    // TODO: 依次测试以上五个练习
    return 0;
}
```

</details>

## 5.4 守护进程与进程管理

- **练习目标**：
    - 理解守护进程的特征（无控制终端、后台运行、长生命周期）。
    - 掌握守护进程的编写步骤：fork + setsid + fork + 关闭/重定向 FD + 修改工作目录。
    - 理解 PID 文件的作用：防止重复启动、便于停止脚本。
    - 掌握进程池的基本模型（master + workers）。
- **练习任务**：
    1. 实现一个守护进程化的"心跳程序"：每秒写一次时间戳到日志文件。
    2. 实现 PID 文件锁（`flock`），保证同一时刻只能运行一个实例。
    3. 实现一个 master + N workers 的进程池模型：master 接收任务（从 pipe），分发给空闲 worker。
    4. 用 `prctl(PR_SET_PDEATHSIG, ...)` 让子进程在父进程退出时自动退出，避免孤儿进程。
- **巩固标准**：
    - [ ] 能手写"双 fork + setsid"的守护进程模板，并解释每一步的作用。
      > **知识讲解**：守护进程的标准创建步骤——第一次 `fork()` 产生子进程，父进程退出，子进程被 init 收养（确保后续 `setsid` 能成功，因为 `setsid` 要求调用者不是会话领袖）；`setsid()` 使子进程创建新会话、脱离原会话和控制终端；第二次 `fork()` 再产生孙进程并让子进程退出，孙进程不再是会话领袖，按 POSIX 规则只有会话领袖才能重新获得控制终端，从而**彻底防止守护进程重新关联终端**。之后关闭或重定向 stdin/stdout/stderr 到 `/dev/null`（避免占用终端或挂载点），`chdir("/")` 修改工作目录（防止守护进程的工作目录在某个挂载点上导致该文件系统无法卸载）。有些实现还会加上 `umask(0)` 清除文件权限掩码，使后续创建文件的权限完全由程序自己控制。
    - [ ] 能说出为什么守护进程要关闭或重定向 stdin/stdout/stderr（避免占用挂载点、避免写入终端）。
      > **知识讲解**：守护进程在后台运行，没有控制终端。若保留 stdin 打开指向原终端，终端关闭时会向守护进程发送 `SIGHUP` 信号导致意外终止；若 stdout/stderr 仍指向终端，一方面输出会显示在终端上干扰用户，另一方面终端关闭后守护进程继续 `printf`/`std::cout` 写入已断开的终端会触发 `SIGPIPE` 或写入被丢弃导致不可预期行为。正确做法是 `close(0); close(1); close(2);` 后 `open("/dev/null", ...)` 将 fd 0/1/2 重定向到 `/dev/null`。注意必须**重定向到 `/dev/null` 而非简单关闭**——因为某些库函数可能默认使用 fd 0/1/2，关闭后新打开的文件可能恰好获得这些 fd 编号，导致库函数意外写入错误目标。
    - [ ] 能对比"进程池"与"线程池"的适用场景（CPU 密集 vs IO 密集、隔离性、上下文切换成本）。
      > **知识讲解**：**进程池**适合 CPU 密集型任务——每个进程有独立地址空间，能充分利用多核并行（无 GIL 限制），且故障隔离好（一个 worker 崩溃不影响 master 和其他 worker）；缺点是进程间通信需借助管道/共享内存/消息队列等 IPC，数据交换成本较高，进程创建和上下文切换开销大（需切换页表、刷新 TLB）。**线程池**适合 IO 密集型任务——线程共享地址空间，通信方便（直接读写共享内存），线程切换成本远低于进程切换（共享页表，只需切换寄存器和栈），IO 等待时线程让出 CPU 不会增加 CPU 竞争；缺点是需要同步保护共享数据（mutex/condition_variable），一个线程崩溃可能导致整个进程段错误。选择原则：**需要隔离性和 CPU 并行用进程池，需要高并发 IO 和便捷通信用线程池**。也可以混合使用——主从 Reactor 模型中 master 用多进程 accept，worker 用多线程处理 IO。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 5.4 守护进程与进程管理 练习框架 ===
// 项目结构:
// 5-4-daemon/
// ├── DaemonExercises.h
// ├── DaemonExercises.cpp
// └── main.cpp

// ---------- DaemonExercises.h ----------
#pragma once

// 练习1: 实现守护进程化的心跳程序（每秒写时间戳到日志文件）
void daemonHeartbeat();

// 练习2: 实现 PID 文件锁，保证同一时刻只能运行一个实例
bool acquirePidLock(const char* pidFile);
void releasePidLock(const char* pidFile);

// 练习3: 实现 master + N workers 的进程池模型
void processPool();

// 练习4: 用 prctl(PR_SET_PDEATHSIG) 让子进程在父进程退出时自动退出
void preventOrphan();

// ---------- DaemonExercises.cpp ----------
#include "DaemonExercises.h"

#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <ctime>

// 练习1
void daemonHeartbeat() {
    // TODO: fork
    // TODO: setsid 创建新会话
    // TODO: 再次 fork 防止重新获得控制终端
    // TODO: 关闭 stdin/stdout/stderr，重定向到 /dev/null
    // TODO: chdir("/") 修改工作目录
    // TODO: 循环 sleep(1)，写时间戳到 /tmp/heartbeat.log
}

// 练习2
bool acquirePidLock(const char* pidFile) {
    // TODO: open(pidFile, O_CREAT|O_RDWR, 0644)
    // TODO: flock(fd, LOCK_EX|LOCK_NB) 尝试加锁，失败说明已有实例在跑
    // TODO: ftruncate + write(getpid())
    // TODO: 返回 true 表示加锁成功
    return false;
}

void releasePidLock(const char* pidFile) {
    // TODO: flock(fd, LOCK_UN) + close(fd) + unlink(pidFile)
}

// 练习3
void processPool() {
    const int N = 4;
    int taskPipe[2];
    // TODO: pipe(taskPipe) 创建任务管道
    // TODO: fork N 个 worker，每个 worker 循环从读端读任务并执行
    // TODO: master 从写端写入任务，实现分发
    // TODO: master 关闭写端，waitpid 等待所有 worker 退出
}

// 练习4
void preventOrphan() {
    // TODO: fork 出子进程
    // TODO: 子进程中 prctl(PR_SET_PDEATHSIG, SIGTERM)
    // TODO: 子进程循环 sleep，父进程主动退出
    // TODO: 验证子进程在父进程退出后也自动退出（ps 查看）
}

// ---------- main.cpp ----------
#include "DaemonExercises.h"

int main() {
    // TODO: 依次测试以上四个练习
    return 0;
}
```

</details>

---

# 阶段六：网络编程进阶

> **定位**：综合项目的核心前置知识。从阶段四的"能写 socket / epoll"升级到"能写一个健壮的长连接服务器"。

## 6.1 TCP 协议细节

- **练习目标**：
    - 深入理解三次握手 / 四次挥手的过程与每个状态（LISTEN / SYN_SENT / SYN_RECV / ESTABLISHED / FIN_WAIT_1 / FIN_WAIT_2 / CLOSE_WAIT / LAST_ACK / TIME_WAIT）。
    - 能用 `netstat` / `ss` 观察连接状态，定位 TIME_WAIT 堆积、CLOSE_WAIT 泄漏等问题。
    - 理解 TCP 的可靠传输机制：序号、确认、重传、滑动窗口、拥塞控制（概念层面）。
- **练习任务**：
    1. 用 `tcpdump` 抓包观察一次完整的 TCP 三次握手与四次挥手，标注每个包的状态变化。
    2. 写一个客户端"中途 kill"的场景，观察服务端的 CLOSE_WAIT 堆积，分析原因并修复。
    3. 写一个短连接压测脚本，观察 TIME_WAIT 堆积，通过 `sysctl` 调整 `net.ipv4.tcp_tw_reuse` 缓解。
    4. 用 `ss -s` 查看系统级 TCP 统计，解读各状态数量。
- **巩固标准**：
    - [ ] 能画出 TCP 状态机，并解释每个状态转换的触发条件。
    > **知识讲解**：TCP 状态机分为连接建立、数据传输、连接关闭三个阶段。建立阶段：`CLOSED` → 主动方 `send(SYN)` → `SYN_SENT` → 被动方收到后 `recv(SYN)+send(SYN+ACK)` → `SYN_RECV` → 主动方 `recv(ACK)` → 双方进入 `ESTABLISHED`。关闭阶段（四次挥手）：主动方 `send(FIN)` → `FIN_WAIT_1` → 被动方 `recv(FIN)+send(ACK)` → 主动方进入 `FIN_WAIT_2`、被动方进入 `CLOSE_WAIT` → 被动方 `send(FIN)` → 主动方 `recv(FIN)+send(ACK)` → 主动方进入 `TIME_WAIT`、被动方进入 `LAST_ACK` → 主动方 `recv(ACK)` → 被动方进入 `CLOSED`；主动方在 `TIME_WAIT` 等待 2MSL 后也进入 `CLOSED`。常见面试陷阱：`CLOSE_WAIT` 是被动关闭方的状态，如果长期停留说明应用层没有调用 `close()`，属于程序 bug。
    >
    > **注意**：上述 `send(SYN)`、`recv(FIN)` 等是 **TCP 协议层的行为描述**（内核自动完成的报文收发），不是代码中需要调用的函数。下表列出协议动作与实际 socket API 的对应关系：
    >
    > | 协议层动作 | 代码中触发的 API | 说明 |
    > |---|---|---|
    > | `send(SYN)` | `connect()` | 客户端调用 `connect()`，内核自动发出 SYN |
    > | `recv(SYN)` + `send(SYN+ACK)` | `listen()` + `accept()` | 服务端 `accept()` 时内核完成 SYN 接收与 SYN+ACK 回复 |
    > | `send(FIN)` | `close(fd)` 或 `shutdown(fd, SHUT_WR)` | 应用层关闭写端，内核自动发 FIN |
    > | `recv(FIN)` | `read()` / `recv()` 返回 0 | 对端发出 FIN 后，本端 `read()` 返回 0 表示对端已关闭 |
    > | `send(ACK)` | 无需手动调用 | ACK 由内核 TCP 栈自动发送，应用层不感知 |
    - [ ] 能解释 TIME_WAIT 为什么由"主动关闭方"进入，持续 2MSL 的原因。
    > **知识讲解**：`TIME_WAIT` 由主动关闭方（最后发送 ACK 的一方）进入，有两个核心原因：① **保证最后一个 ACK 能到达对端**——如果该 ACK 丢失，被动关闭方会重发 FIN，主动关闭方必须在 `TIME_WAIT` 状态下才能重新发送 ACK，否则对端永远无法关闭。② **让网络中该连接的残余报文消散**——MSL（Maximum Segment Lifetime）是报文在网络中的最大生存时间（Linux 默认 60 秒），等待 2MSL 可以确保：本方发出的最后一个 ACK 的 MSL 内到达 + 对端重发 FIN 的 MSL 内到达，从而旧连接的所有报文都已从网络中消失，不会干扰新连接。2MSL 期间该连接的四元组（源 IP、源端口、目的 IP、目的端口）不能被复用，这就是短连接高频场景下 `TIME_WAIT` 堆积导致端口耗尽的原因。
    - [ ] 能定位 CLOSE_WAIT 泄漏的根因（未调用 close / 上层逻辑未处理对端 FIN）。
    > **知识讲解**：`CLOSE_WAIT` 是被动关闭方的状态，表示本端已收到对端的 FIN（即对端关闭了写端），但本端应用层还没有调用 `close()`。正常情况下，应用层收到 `read()` 返回 0（表示对端已关闭）后应立即 `close()`，连接会快速经过 `LAST_ACK` → `CLOSED`。如果大量连接停留在 `CLOSE_WAIT`，说明应用层存在 bug：① 没有检查 `read()` 返回值是否为 0，导致没有触发 `close()`；② 业务逻辑中遗漏了异常分支的 `close()` 调用；③ 使用了 epoll 但没有监听 `EPOLLHUP` 或 `EPOLLRDHUP` 事件。排查方法：`ss -tan state close-wait` 查看堆积数量，结合日志确认对应连接的业务处理流程是否走到了 `close()` 分支。与 `TIME_WAIT` 不同，`CLOSE_WAIT` 泄漏是纯粹的代码问题，无法通过内核参数缓解。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 6.1 TCP 协议细节 练习框架 ===
// 项目结构:
// 6-1-tcp/
// ├── TcpExercises.h
// ├── TcpExercises.cpp
// └── main.cpp

// ---------- TcpExercises.h ----------
#pragma once

// 练习2: 写一个客户端中途 kill 的场景，观察服务端的 CLOSE_WAIT 堆积
void startEchoServer();       // 服务端（故意不 close），用于练习 2
void startBrokenClient();     // 客户端，故意不发 FIN

// 练习3: 短连接压测脚本，观察 TIME_WAIT 堆积
void startStressEchoServer(); // 练习3 专用：echo 后立即 close 的循环服务端
void shortConnectionStress(int count);

// 练习4: 用 ss -s 查看系统级 TCP 统计
void printTcpStats();

// ---------- TcpExercises.cpp ----------
#include "TcpExercises.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>

void startEchoServer() {
    // TODO: socket + bind + listen
    // TODO: accept 循环，read 后 write 回去
    // TODO: 注意：客户端断开时不要主动 close，观察 CLOSE_WAIT
}

void startBrokenClient() {
    // TODO: connect 到 127.0.0.1:port
    // TODO: send 一些数据后直接 _exit(0)（不发 FIN）
    // TODO: 用 ss -tan 观察服务端连接状态
}

void shortConnectionStress(int count) {
    // TODO: 循环 count 次，每次 connect + send + recv + close
    // TODO: 用 ss -tan state time-wait 观察 TIME_WAIT 堆积
    // TODO: 提示用户用 sysctl 调整 net.ipv4.tcp_tw_reuse
}

void startStressEchoServer() {
    // TODO: socket + setsockopt(SO_REUSEADDR) + bind(8080) + listen
    // TODO: accept 循环，recv → write 回显 → 立即 close(client_fd)
    // 与 startEchoServer 区别：这里必须 close，才能让客户端进入 TIME_WAIT
}

void printTcpStats() {
    // TODO: system("ss -s") 或 popen 读取输出
}

// ---------- main.cpp ----------
#include "TcpExercises.h"

int main() {
    // TODO: 依次测试以上练习
    return 0;
}
```

</details>

<details>
<summary>🧪 测试操作步骤</summary>

> 端口固定为 **8080**，本节练习需要**多个终端窗口**配合：一个跑服务端、一个跑客户端、一个用 `ss` 观察状态。所有练习共用同一套编译流程，仅菜单选项不同——程序通过 `main.cpp` 的菜单分发：`1`=startEchoServer，`2`=startBrokenClient，`3`=shortConnectionStress，`4`=printTcpStats。

#### 练习 2：观察 CLOSE_WAIT 堆积

需要 **3 个终端**：

| 步骤 | 终端 A（服务端） | 终端 B（坏客户端） | 终端 C（观察） |
|------|------------------|--------------------|----------------|
| 1 | 运行程序，输入 `1` 启动 startEchoServer，阻塞在 accept | | |
| 2 | | 运行程序，输入 `2` 启动 startBrokenClient | |
| 3 | 打印 `read returned 6, no close(cfd=...)`，之后**故意不 close** | 发送 `Hello!` 后 `_exit(0)` 直接退出 | |
| 4 | 保持运行 | | 执行 `ss -tan state close-wait` |
| 5 | | | 看到一条 8080 连接长期停在 **CLOSE_WAIT** |
| 6 | `Ctrl+C` 停止服务端 | | |

**验证要点**：服务端收到客户端 FIN 后进入 CLOSE_WAIT，但因为**从不调用 `close(client_fd)`**，连接会一直滞留。多次运行客户端可看到 CLOSE_WAIT 连接数持续增加（同时服务端 fd 泄漏）——这正是生产中「CLOSE_WAIT 堆积 = 应用层忘记关闭 fd」的根因，对应巩固标准第 3 条。

### 练习 3：观察 TIME_WAIT 堆积

> ⚠️ **前置条件**：TIME_WAIT 出现在**主动关闭方**，且必须**收到对端 FIN** 后才能进入。上面的 startEchoServer **故意不 close**，若直接拿它做压测服务端，客户端只会停在 **FIN_WAIT_2**、服务端停在 **CLOSE_WAIT**，**观察不到 TIME_WAIT**。因此练习 3 必须换一个「收数据 → 回显 → 正常 close」的服务端：本项目已内置 `startStressEchoServer`（菜单选项 `5`，accept 循环 + 回显 + 立即 close，并带 `SO_REUSEADDR` 方便反复重启），直接用它即可。

需要 **2 个终端**：

| 步骤 | 终端 A（正常关闭的回声服务端） | 终端 B（压测客户端） |
|------|--------------------------------|----------------------|
| 1 | 运行程序，输入 `5` 启动 startStressEchoServer（echo 后立即 close，阻塞在 accept 循环） | |
| 2 | | 运行程序，输入 `3`，再输入压测次数（如 `2000`） |
| 3 | 循环 accept → 回显 → close | 循环 connect → send → recv → **close**（客户端主动关闭） |
| 4 | | 结束后执行 `ss -tan state time-wait \| wc -l` |
| 5 | | 看到大量 **TIME_WAIT**（客户端侧，持续 2MSL≈60s） |

**验证要点**：每条短连接都由客户端主动 close，走完四次挥手后客户端进入 TIME_WAIT，`count` 越大堆积越多。缓解手段：`sudo sysctl -w net.ipv4.tcp_tw_reuse=1`（允许复用 TIME_WAIT 端口，主要对客户端有效）。

### 练习 4：查看系统级 TCP 统计

需要 **1 个终端**：

| 步骤 | 操作 |
|------|------|
| 1 | 运行程序，输入 `4` 调用 printTcpStats |
| 2 | 内部执行 `ss -s`，打印整机 TCP 汇总 |
| 3 | 关注 `estab` / `timewait` / `closed`（含 CLOSE_WAIT）各状态数量 |

**验证要点**：在练习 2、练习 3 的前后分别运行一次，对比 `timewait`、`closed` 数量变化，即可量化前面制造的现象。若想在程序里自动做前后差值，可把 `system("ss -s")` 换成 `popen` 读取输出再解析。

</details>

## 6.2 粘包与半包处理

- **练习目标**：
    - 理解 TCP 是字节流协议，没有"消息边界"，应用层必须自己拆包。
    - 掌握三种主流拆包方案：定长包、分隔符包、Length-Field（长度字段）包。
    - 能结合 epoll 缓冲区实现一个健壮的"读半包 → 拼包 → 拆包"流程。
- **练习任务**：
    1. 实现"定长包"协议：每条消息固定 64 字节，不足补齐。
    2. 实现"分隔符包"协议：以 `\n` 为分隔（类似文本协议），处理粘包与半包。
    3. 实现"Length-Field"协议：前 4 字节为消息长度（大端），后接消息体。
    4. 写一个压测客户端：故意分片发送（每次只发 1 字节 / 一次发多条），验证服务端的拆包逻辑。
- **巩固标准**：
    - [ ] 能画出"接收缓冲区 → 拆包循环 → 应用层回调"的流程图。
    > **知识讲解**：整条流水线分三步：**① 追加缓冲区**——`epoll` 通知可读后，调用 `read()`/`recv()` 把收到的字节**追加**到该连接专属的接收缓冲区尾部（如 `std::string` / `std::vector<char>`），注意是追加而非覆盖，因为一次 `read` 返回的可能是半包或多包。**② 拆包循环**——`while` 循环检查缓冲区：剩余字节数是否 ≥ 帧头长度？够则解析帧头得到期望包长，再检查剩余字节数是否 ≥ 帧头 + 包长；不够则 `break` 退出循环，保留缓冲区数据等待下一次可读事件（这就是"半包"的自然去向）；够则从缓冲区头部截取一条完整消息并 `erase` 已消费的部分——一次 `read` 收了多条消息（"粘包"）就在这一轮循环里被连续拆出多条。**③ 应用层回调**——每拆出一条完整消息，立即通过回调（如 `MessageCallback`）交给业务处理，业务代码只见完整消息、不见字节流。核心思想：**缓冲区负责攒字节，循环负责剥完整包，剩下的永远是不完整的尾巴**。常见错误：忘记从缓冲区头部删除已消费的字节，导致同一条消息被反复拆出。
    - [ ] 能解释为什么 Length-Field 是二进制协议最常用的方案（效率 + 通用性）。
    > **知识讲解**：**效率层面**——定长包在消息长度参差时浪费严重（短消息补零占带宽、长消息还得拆片）；分隔符包必须逐字节扫描整个消息体才能找到边界，且扫描结果决定包长，无法预分配缓冲区；Length-Field 只需读固定 4 字节帧头就已知整包大小：半包时直接对比"已收字节数 vs 声明长度"，定位边界是 O(1) 而非 O(n)，还能按声明长度一次性预留缓冲区。**通用性层面**——消息体可以是任意二进制数据（含 `\0`、含分隔符字节都无所谓，因为根本不扫描内容），且帧头天然可扩展为携带版本号、消息类型、校验和等字段（TLV 思想），protobuf、gRPC、HTTP/2 帧、MySQL 协议底层全是"长度前缀"结构。工程注意事项：帧头字节序要统一（网络字节序大端，配 `htonl`/`ntohl` 转换）；解析长度字段后必须**上限校验**（如限制单包 ≤ 1 MB），否则恶意构造的超大长度字段会让服务端分配巨量内存直接 OOM。
    - [ ] 能处理"消息体中又出现分隔符"的转义问题。
    > **知识讲解**：以 `\n` 分隔的文本协议有个固有缺陷：如果消息体本身包含 `\n`（如用户输入的日志行、多行文本），朴素的 `find('\n')` 会把一条消息从中间错误切断。解决思路有三：**① 转义（escaping）**——约定一个转义字符（如 `\`），发送端编码时把消息体中出现的 `\n` 替换为两个字面字符 `\` + `n`、`\` 自身替换为 `\\`，接收端解码时逆向还原；Redis RESP、JSON 字符串、CSV 引号包裹都是这一思路。关键在于**编解码必须严格互逆**，且转义要先替换转义符本身再替换分隔符，否则会产生二义性。**② 改用 Length-Field**——不扫描分隔符、按长度切边界，消息体内容是什么都无关紧要，这正是二进制协议偏爱长度前缀的根本原因。**③ 校验 + 重新同步**——拆出的"包"若校验失败（长度字段异常、checksum 不匹配）说明帧已错位，丢弃缓冲区内数据等待连接重置或扫描下一个合法帧头。面试高频陷阱："为什么分隔符协议不适合传图片/文件等二进制数据"——因为二进制内容中任何字节都可能出现，分隔符方案要么转义开销巨大，要么根本无法保证边界可靠。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 6.2 粘包与半包 练习框架 ===
// 项目结构:
// 6-2-packet/
// ├── PacketCodec.h
// ├── PacketCodec.cpp
// └── main.cpp

// ---------- PacketCodec.h ----------
#pragma once
#include <cstdint>
#include <functional>
#include <string>

// 拆包回调：每拆出一条完整消息就回调一次
using MessageCallback = std::function<void(const std::string& msg)>;

// 练习1: 定长包协议（每条消息固定 64 字节）
class FixedLengthCodec {
public:
    explicit FixedLengthCodec(size_t fixedLen = 64);
    // 输入接收缓冲区新数据，内部拆包并回调
    void onData(const char* data, size_t len, const MessageCallback& cb);
private:
    size_t fixedLen_;
    std::string buffer_;
};

// 练习2: 分隔符包协议（以 '\n' 为分隔）
class DelimiterCodec {
public:
    explicit DelimiterCodec(char delim = '\n');
    void onData(const char* data, size_t len, const MessageCallback& cb);
private:
    char delim_;
    std::string buffer_;
};

// 练习3: Length-Field 协议（前 4 字节大端长度 + 消息体）
class LengthFieldCodec {
public:
    void onData(const char* data, size_t len, const MessageCallback& cb);
private:
    std::string buffer_;
};

// ---------- PacketCodec.cpp ----------
#include "PacketCodec.h"
#include <arpa/inet.h>

// 练习1
FixedLengthCodec::FixedLengthCodec(size_t fixedLen) : fixedLen_(fixedLen) {}

void FixedLengthCodec::onData(const char* data, size_t len, const MessageCallback& cb) {
    // TODO: buffer_.append(data, len)
    // TODO: while (buffer_.size() >= fixedLen_) { 取出前 fixedLen_ 字节回调; buffer_.erase(0, fixedLen_); }
}

// 练习2
DelimiterCodec::DelimiterCodec(char delim) : delim_(delim) {}

void DelimiterCodec::onData(const char* data, size_t len, const MessageCallback& cb) {
    // TODO: buffer_.append(data, len)
    // TODO: while (auto pos = buffer_.find(delim_)) != npos { 回调 substr(0,pos); buffer_.erase(0,pos+1); }
}

// 练习3
void LengthFieldCodec::onData(const char* data, size_t len, const MessageCallback& cb) {
    // TODO: buffer_.append(data, len)
    // TODO: while (buffer_.size() >= 4) {
    //   读取前 4 字节为 bodyLen（大端转主机序）
    //   if (buffer_.size() < 4 + bodyLen) break;  // 半包
    //   取出 body 回调
    //   buffer_.erase(0, 4 + bodyLen);
    // }
}

// ---------- main.cpp ----------
#include "PacketCodec.h"
#include <iostream>

int main() {
    // 练习4: 压测客户端，故意分片发送验证拆包逻辑
    auto cb = [](const std::string& m) {
        std::cout << "[msg] " << m << "\n";
    };

    FixedLengthCodec fc(8);
    const char* d1 = "helloXXXworldXXX";  // 两条定长消息
    fc.onData(d1, 16, cb);

    DelimiterCodec dc;
    dc.onData("hello\nwor", 8, cb);  // 半包
    dc.onData("ld\nfoo\n", 8, cb);   // 粘包 + 补齐

    LengthFieldCodec lc;
    // TODO: 构造 Length-Field 数据测试
    return 0;
}
```

</details>

## 6.3 Socket 选项与高级特性

- **练习目标**：
    - 掌握常用 socket 选项：`SO_REUSEADDR`、`SO_REUSEPORT`、`TCP_NODELAY`、`SO_KEEPALIVE`、`SO_LINGER`、`SO_RCVBUF` / `SO_SNDBUF`。
    - 理解每个选项的生效时机（`listen` 前 / `connect` 前 / 任意时刻）。
    - 掌握 TCP Keepalive 与应用层心跳的区别。
- **练习任务**：
    1. 给服务器加上 `SO_REUSEADDR`，验证重启后不再报 "Address already in use"。
    2. 用 `SO_REUSEPORT` 实现多进程负载均衡（多个进程 bind 同一端口，内核分配连接）。
    3. 开启 `TCP_NODELAY`，对比小消息场景下的延迟变化。
    4. 开启 `SO_KEEPALIVE` 并调整内核参数（`tcp_keepalive_time` / `tcp_keepalive_intvl` / `tcp_keepalive_probes`），验证能检测到对端异常断开。
    5. 用 `SO_LINGER` 控制 close 时的行为（立即 RST vs 等待发送完毕）。
- **巩固标准**：
    - [ ] 能说出 `SO_REUSEADDR` 与 `SO_REUSEPORT` 的区别（前者允许端口复用，后者还负责负载均衡）。
    - [ ] 能解释 TCP Keepalive 的默认时长（2 小时）为什么不适合应用层，以及应用层心跳的设计要点。
    - [ ] 能解释 `SO_LINGER` 设置 linger=0 时 close 会发 RST 而非 FIN 的场景。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 6.3 Socket 选项 练习框架 ===
// 项目结构:
// 6-3-sockopt/
// ├── SockOptExercises.h
// ├── SockOptExercises.cpp
// └── main.cpp

// ---------- SockOptExercises.h ----------
#pragma once

// 练习1: SO_REUSEADDR 避免重启报 "Address already in use"
void reuseAddrServer(int port);

// 练习2: SO_REUSEPORT 实现多进程负载均衡
void reusePortMultiProcess(int port);

// 练习3: TCP_NODELAY 对比小消息场景下的延迟
void tcpNoDelayTest(bool enable);

// 练习4: SO_KEEPALIVE 检测对端异常断开
void keepAliveServer(int port);

// 练习5: SO_LINGER 控制 close 行为
void lingerTest(bool enableLinger, int lingerSec);

// ---------- SockOptExercises.cpp ----------
#include "SockOptExercises.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

// 练习1
void reuseAddrServer(int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // TODO: int opt = 1; setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // TODO: bind + listen + accept
    // TODO: 重启程序验证不再报 "Address already in use"
}

// 练习2
void reusePortMultiProcess(int port) {
    // TODO: fork N 个子进程，每个都 socket + setsockopt(SO_REUSEPORT) + bind + listen + accept
    // TODO: 用客户端并发连接，观察内核把连接分配到不同进程
}

// 练习3
void tcpNoDelayTest(bool enable) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // TODO: int flag = enable ? 1 : 0; setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    // TODO: 发送大量 1 字节小包，记录耗时对比
}

// 练习4
void keepAliveServer(int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // TODO: int opt = 1; setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    // TODO: 调整内核参数 tcp_keepalive_time / intvl / probes
    // TODO: accept 后等待，客户端掉线时验证能检测到
}

// 练习5
void lingerTest(bool enableLinger, int lingerSec) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // TODO: struct linger lg = { enableLinger, lingerSec };
    // TODO: setsockopt(fd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));
    // TODO: connect 后 close，用 tcpdump 观察是发 FIN 还是 RST
}

// ---------- main.cpp ----------
#include "SockOptExercises.h"

int main() {
    // TODO: 依次测试以上五个练习
    return 0;
}
```

</details>

<details>
<summary>🧪 测试操作步骤</summary>

> 端口默认 **8888**（可在菜单中自定义）。所有练习共用同一套编译流程，仅菜单选项不同——程序通过 `main.cpp` 的菜单分发：`1`=reuseAddrServer，`2`=reusePortMultiProcess，`3`=tcpNoDelayTest，`4`=keepAliveServer，`5`=lingerTest。

### 练习 2：SO_REUSEPORT 多进程负载均衡

需要 **2 个终端**：终端 A 跑多进程服务端，终端 B 用 `nc` 发起并发连接制造负载。

| 步骤 | 终端 A（服务端） | 终端 B（并发客户端） |
|------|------------------|----------------------|
| 1 | 运行程序，输入 `2`，端口回车用默认 8888 | |
| 2 | fork 出 4 个 worker，各自打印 `[worker PID] listening on port 8888`；父进程打印 `press Ctrl+C to quit` 后阻塞等待 | |
| 3 | | 执行 `for i in $(seq 1 20); do (echo hi \| nc -q0 127.0.0.1 8888 &); done` 发起 20 条并发短连接 |
| 4 | 每条连接被某个 worker accept，打印 `[worker PID] connection #N` | |
| 5 | 观察连接被分散到**不同的 worker PID** 上（大致均匀，小样本下略有偏差属正常） | |
| 6 | `Ctrl+C` 停止：父进程 `kill(0, SIGTERM)` 通知整个进程组，阻塞 `waitpid` 回收全部 worker，打印 `[parent] all workers exited` 后退出 | |

**验证要点**：4 个进程能同时 `bind` 同一端口 8888 而不报 `Address already in use`，正是 `SO_REUSEPORT` 的作用；内核按连接四元组哈希把新连接分配到某个监听 socket，因此并发连接会分散到不同 worker。退出后可用 `ps aux \| grep sockopt` 确认无残留 / 僵尸进程。

> 💡 **与练习 1 的 `SO_REUSEADDR` 区别**：`SO_REUSEADDR` 只解决「端口处于 TIME_WAIT 时无法重新 bind」，同一时刻仍只允许一个监听 socket；`SO_REUSEPORT` 则允许多个 socket 同时 bind 同一端口，并由内核负责把连接负载均衡到这些 socket 上。

### 练习 4：SO_KEEPALIVE 检测对端异常断开

需要 **2 个终端**：终端 A 运行服务端，终端 B 用 `nc` 连接后模拟异常断开。代码中 keepalive 参数为 `idle=5s, intvl=2s, probes=3`，即空闲 5 秒后开始探测，每 2 秒探测一次，共 3 次，总计约 **11 秒**检测到断连。

**方法 A：`iptables` 丢包模拟网络中断（推荐，可观察完整 keepalive 超时）**

> 需要 `sudo` 权限。原理：用 `iptables` 丢弃目标端口 8888 的所有出站包，使服务端发出的 keepalive 探测包无法到达客户端，从而触发完整超时。

| 步骤 | 终端 A（服务端） | 终端 B（客户端 / iptables） |
|------|------------------|---------------------------|
| 1 | 运行程序，输入 `4`，端口回车用默认 8888 | |
| 2 | 打印 `listening on port 8888 ...` | |
| 3 | | 执行 `nc 127.0.0.1 8888` 建立连接 |
| 4 | 打印 `client connected, keepalive: idle = 5s intvl=2s probes=3` | |
| 5 | | 新开终端 C，执行 `sudo iptables -A OUTPUT -p tcp --dport 8888 -j DROP` 丢弃所有发往 8888 端口的包 |
| 6 | 保持等待，约 **11 秒**后 `read` 返回 -1，打印 `read (keepalive detected peer death): Connection timed out` | |
| 7 | | 执行 `sudo iptables -F` 清除规则，恢复网络 |

**验证要点**：服务端在约 11 秒后检测到断连，正是 `idle + intvl × probes = 5 + 2×3 = 11` 秒的 keepalive 超时时间。`errno` 为 `ETIMEDOUT`（Connection timed out），说明是 keepalive 探测超时而非对端主动关闭。

> 💡 **为什么需要 `iptables`？** 如果直接 `kill -9 nc`，内核会立即发送 RST 包，服务端瞬间收到 `ECONNRESET` 而检测到断连——这走的是 RST 快速路径，**不经过 keepalive 机制**。`iptables` 丢弃出站包后，服务端的探测包被丢弃、对端无法回复，才能触发完整的 keepalive 超时流程。

**方法 B：`kill -9` 快速验证（检测异常退出路径）**

| 步骤 | 终端 A（服务端） | 终端 B（客户端） |
|------|------------------|------------------|
| 1 | 运行程序，输入 `4`，端口回车用默认 8888 | |
| 2 | 打印 `listening on port 8888 ...` | |
| 3 | | 执行 `nc 127.0.0.1 8888` 建立连接 |
| 4 | 打印 `client connected, keepalive: idle = 5s intvl=2s probes=3` | |
| 5 | | 另开终端 C，执行 `kill -9 $(pgrep -f 'nc 127.0.0.1')` 强杀 nc |
| 6 | **立即**打印 `client disconnected (normal close)`（若 nc 有未读数据则打印 `read (keepalive detected peer death): Connection reset by peer`） | nc 被 Killed |

**验证要点**：`kill -9` 后内核立即关闭 socket 并发送 FIN 或 RST（取决于 nc 接收缓冲区是否有未读数据），服务端 `read` **立即**返回而非等待 11 秒 keepalive 超时。此路径**不经过 keepalive 机制**，但验证了服务端能快速感知对端进程异常退出。

> 💡 **FIN vs RST 的触发条件**：`kill -9` 时，若进程 TCP 接收队列为空，内核发送 FIN（正常关闭语义，`read` 返回 0）；若接收队列中有未读数据，内核发送 RST（异常重置，`read` 返回 -1 且 `errno = ECONNRESET`）。本练习 `nc` 连接后无数据传输，因此走 FIN 路径。

> 💡 **方法 A vs 方法 B**：方法 A 验证 keepalive 的完整超时机制（约 11 秒延迟检测），方法 B 验证 RST 快速检测路径（立即检测）。实际生产中，keepalive 主要用于检测**网络中断、对端宕机**等不发 RST 的场景；正常进程退出时内核总会发送 RST，无需等 keepalive。

### 练习 5：SO_LINGER 控制 close 行为

需要 **2 个终端**：终端 A 运行程序，终端 B 用 `tcpdump` 抓包观察关闭连接时发 FIN 还是 RST。程序内部 `fork` 子进程充当 echo 服务器（端口 19899），父进程作为客户端设置 `SO_LINGER` 后连接、发送数据、关闭。

**场景 A：`linger=0`（立即 RST，异常重置）**

> 需要 `sudo` 权限运行 `tcpdump`。

| 步骤 | 终端 A（程序） | 终端 B（tcpdump 抓包） |
|------|---------------|----------------------|
| 1 | | 执行 `sudo tcpdump -i lo -nn 'tcp port 19899'` 开始抓包 |
| 2 | 运行程序，输入 `5` | |
| 3 | 启用 SO_LINGER 输入 `1`，linger 秒数输入 `0` | |
| 4 | 打印 `SO_LINGER: l_onoff=1, l_linger=0` | |
| 5 | 打印 `closing socket...` | 观察抓包结果 |
| 6 | 程序退出 | 最后一行看到 `Flags [R.]`（**RST**），**无四次挥手** |

**验证要点**：`l_onoff=1, l_linger=0` 使 `close()` 立即发送 RST 而非 FIN，连接被一步强制重置。对端 `read()` 会返回 `-1` 且 `errno = ECONNRESET`，而非返回 `0`（EOF）。

**场景 B：默认行为（优雅 FIN 关闭）**

| 步骤 | 终端 A（程序） | 终端 B（tcpdump 抓包） |
|------|---------------|----------------------|
| 1 | | 执行 `sudo tcpdump -i lo -nn 'tcp port 19899'` 开始抓包 |
| 2 | 运行程序，输入 `5` | |
| 3 | 启用 SO_LINGER 输入 `0`，linger 秒数回车用默认 `0` | |
| 4 | 打印 `SO_LINGER: l_onoff=0, l_linger=0` | |
| 5 | 打印 `closing socket...` | 观察抓包结果 |
| 6 | 程序退出 | 最后三行看到 `Flags [F.]` → `Flags [F.]` → `Flags [.]`（**标准四次挥手**） |

**验证要点**：`l_onoff=0` 为默认行为，`close()` 正常发送 FIN 启动四次挥手（FIN → ACK → FIN → ACK），对端 `read()` 返回 `0`（EOF）。

> 💡 **RST vs FIN 的本质区别**：FIN 是「我说完了」的优雅关闭语义，对端仍可发送剩余数据；RST 是「连接作废」的异常信号，双方立即释放连接资源，未传数据直接丢弃。`SO_LINGER` 的 `linger=0` 是唯一让内核发 RST 而非 FIN 的场景，适用于需要立即踢掉对端的场景（如服务器主动断开恶意客户端）。

</details>

## 6.4 心跳、超时与优雅退出

- **练习目标**：
    - 掌握应用层心跳协议设计（ping / pong 消息、超时判定、重试策略）。
    - 掌握读写超时的三种实现：`SO_RCVTIMEO` / `SO_SNDTIMEO`、`select` / `poll` 超时、epoll + 定时器。
    - 理解"优雅退出"的完整流程：停止接收新连接 → 等待 in-flight 请求处理完 → 关闭旧连接。
- **练习任务**：
    1. 设计一个心跳协议：客户端每 10 秒发 PING，服务端 30 秒未收到任何消息则断开。
    2. 用 epoll + 最小堆定时器实现"连接超时检测"：每个连接记录最后活跃时间，定时扫描超时连接。
    3. 实现服务器的优雅退出：捕获 SIGTERM 后停止 accept，等待所有连接空闲或超时后退出。
    4. 用 `shutdown(fd, SHUT_WR)` 实现半关闭，验证对端能读完剩余数据后收到 EOF。
- **巩固标准**：
    - [ ] 能画出"心跳超时检测"的定时器堆变化过程。
    > **知识讲解**：定时器堆底层是 `std::priority_queue<TimerNode, vector<TimerNode>, greater<TimerNode>>`（最小堆），每个 `TimerNode` 存 `{fd, expireMs}`，按过期时间升序排列，堆顶永远是**最近到期**的定时器。变化过程：① 新连接 `accept` 后入堆，`expireMs = now + timeout`；② 收到客户端消息（心跳续期）时，**不删除旧节点**，而是直接 `push` 一个新的 `TimerNode`（同一个 fd 在堆中有多条记录）；③ `timerLoop()` 每次从堆顶弹出，先做**惰性检查**——`lastActive_` 中找不到该 fd（已关闭）则跳过，`lastActive_[fd] + timeout > now`（已续期，实际过期时间还没到）也跳过。这种"惰性删除"策略避免了 `priority_queue` 无法高效删除中间元素的限制（标准库无 `remove` 操作），代价是堆中可能存在过期冗余节点，但每个节点最多被弹出一次，总体复杂度仍为 O(n log n)。
    - [ ] 能解释 `close()` 与 `shutdown()` 的区别（前者销毁 FD，后者只关闭方向）。
    > **知识讲解**：`close(fd)` 销毁文件描述符并释放内核中该 fd 的全部资源，同时向对端发送 FIN 启动四次挥手（完全关闭读写两个方向），调用后 fd 不可再使用。`shutdown(fd, how)` 只关闭连接的**指定方向**，fd 本身仍然有效：`SHUT_RD` 关闭读端（后续 `read` 返回 0，但不影响发送 FIN）、`SHUT_WR` 关闭写端（向对端发送 FIN，对端 `read` 最终返回 0/EOF，但本端仍可 `read`）、`SHUT_RDWR` 同时关闭两个方向。核心区别：`close` 是**资源管理**操作（释放 fd + 关闭连接），`shutdown` 是**连接控制**操作（精细控制方向）。典型场景：服务器处理完请求后 `shutdown(fd, SHUT_WR)` 发送 FIN 告知"我说完了"，客户端读完剩余数据后收到 EOF，实现"半关闭"优雅结束——这是 `close` 无法做到的，因为 `close` 会同时关闭读端，无法再读取客户端可能发送的剩余数据。注意 `close` 有引用计数：如果 fd 被 `fork` 复制到多个进程，只有所有副本都 `close` 后内核才真正发送 FIN。
    - [ ] 能说明为什么"in-flight 请求"必须处理完才能退出（避免客户端收到不完整响应）。
    > **知识讲解**：in-flight 请求指服务器已经接收但**尚未处理完毕**的请求——可能正在查数据库、计算结果、或 `send` 到一半被中断。如果此时直接 `close` 退出，客户端会收到不完整响应（截断的 JSON、半条消息）或连接被重置（RST），导致客户端解析失败、数据丢失、用户体验差。优雅退出的完整流程：① 捕获 `SIGTERM`/`SIGINT` 后设置 `running_ = false`，停止 `accept` 新连接（不再接待新客人）；② 继续事件循环，等待所有已连接客户端的请求处理完毕或超时断开（把在座的客人服务完）；③ 所有连接关闭后，再 `close` 监听 socket 和 epoll 实例（关门打烊）。本练习中通过 `eventfd` + `gracefulShutdown()` 实现：信号处理函数设置标志位并写 `eventfd` 唤醒 `epoll_wait`（避免阻塞在超时上），主循环检查 `running_` 退出后统一清理。如果服务器承载长连接（如 WebSocket、聊天室），还需考虑主动给客户端发送"服务器即将关闭"的通知消息，让客户端有机会重连其他节点。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 6.4 心跳、超时与优雅退出 练习框架 ===
// 项目结构:
// 6-4-heartbeat/
// ├── HeartbeatServer.h
// ├── HeartbeatServer.cpp
// └── main.cpp

// ---------- HeartbeatServer.h ----------
#pragma once
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

// 最小堆定时器：每个连接记录最后活跃时间
struct TimerNode {
    int fd;
    int64_t expireMs;  // 绝对过期时间（毫秒）
    bool operator>(const TimerNode& o) const { return expireMs > o.expireMs; }
};

class HeartbeatServer {
public:
    HeartbeatServer(int port, int heartbeatIntervalSec = 10, int timeoutSec = 30);
    void start();
    void gracefulShutdown();  // 练习3

private:
    void onNewConnection(int fd);
    void onReadable(int fd);
    void onClose(int fd);
    void processMessage(int fd, const std::string& msg);
    void timerLoop();  // 练习2

    int port_;
    int heartbeatIntervalSec_;
    int timeoutSec_;
    std::atomic<bool> running_{true};
    int listenFd_{-1};
    int epollFd_{-1};
    int wakeupFd_{-1};  // eventfd，供信号处理函数唤醒 epoll_wait
    std::unordered_map<int, int64_t> lastActive_;  // fd -> 最后活跃时间
    std::priority_queue<TimerNode, std::vector<TimerNode>, std::greater<TimerNode>> timerHeap_;
};

// 练习4: shutdown 半关闭测试
void halfCloseDemo();

// ---------- HeartbeatServer.cpp ----------
#include "HeartbeatServer.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <cstring>

static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

HeartbeatServer::HeartbeatServer(int port, int hb, int to)
    : port_(port), heartbeatIntervalSec_(hb), timeoutSec_(to) {}

void HeartbeatServer::start() {
    // TODO: socket + setsockopt(SO_REUSEADDR) + bind + listen
    // TODO: fcntl 设置 listenFd 为非阻塞（否则 accept 循环会卡死）
    // TODO: epoll_create1 + 把 listenFd 加入监听
    // TODO: eventfd 创建 wakeupFd_，加入 epoll（用于信号唤醒）
    // TODO: while (running_) {
    //   epoll_wait 超时 = 最近一个定时器的剩余时间
    //   处理事件（listenFd / wakeupFd_ / 客户端 fd）
    //   扫描超时连接并关闭
    // }
}

void HeartbeatServer::onNewConnection(int fd) {
    // TODO: 加入 epoll，记录 lastActive_[fd] = nowMs()
    // TODO: 加入 timerHeap_，expire = nowMs + timeoutSec*1000
}

void HeartbeatServer::onReadable(int fd) {
    // TODO: read 数据
    // TODO: 如果读到 PING 或任何消息，更新 lastActive_[fd]，重新加入 timerHeap_
    // TODO: 处理业务消息
}

void HeartbeatServer::onClose(int fd) {
    // TODO: epoll_ctl DEL, close(fd), lastActive_.erase(fd)
}

void HeartbeatServer::processMessage(int fd, const std::string& msg) {
    // TODO: 回包或业务处理
}

void HeartbeatServer::gracefulShutdown() {
    // 注意：此函数从信号处理函数调用，只能使用 async-signal-safe 操作
    // TODO: running_ = false
    // TODO: 往 eventfd 写一个值，唤醒 server 线程的 epoll_wait
}

void HeartbeatServer::timerLoop() {
    // TODO: while (!timerHeap_.empty() && timerHeap_.top().expireMs <= nowMs()) {
    //   取出 fd，检查 lastActive_[fd] 是否真的过期
    //   若过期则 close(fd)
    // }
}

// 练习4
void halfCloseDemo() {
    int sv[2];
    // TODO: socketpair(AF_UNIX, SOCK_STREAM, 0, sv)
    // TODO: 一端 shutdown(fd, SHUT_WR)
    // TODO: 另一端 read 直到返回 0（EOF），验证仍能继续写回
}

// ---------- main.cpp ----------
#include "HeartbeatServer.h"

int main() {
    HeartbeatServer server(8888);
    // TODO: 启动服务器，用客户端连接后停止发送消息，观察超时断开
    // TODO: 测试 gracefulShutdown
    return 0;
}
```

</details>

## 6.5 IO 多路复用对比与 Reactor 模式

- **练习目标**：
    - 对比 select / poll / epoll / io_uring 的原理、性能特征、适用场景。
    - 理解 Reactor 模式的三种变体：单线程 Reactor、多线程 Reactor、主从 Reactor（main-reactor + sub-reactor）。
    - 理解 Proactor 模式（异步 IO）与 Reactor（同步 IO 就绪通知）的区别。
    - 能设计 Acceptor + Worker 的经典服务器架构。
- **练习任务**：
    1. 用 select / poll / epoll 分别实现同一个 echo 服务器，对比代码复杂度与性能。
    2. 实现单线程 Reactor：一个线程负责 accept + read + write。
    3. 实现主从 Reactor：main-reactor 只负责 accept，把新连接交给 sub-reactor 处理。
    4. 实现主从 Reactor + 线程池：sub-reactor 把 IO 事件派发给线程池执行（IO 与业务分离）。
    5. 用 `sysbench` 或自写压测工具，对比四种架构的 QPS 与延迟。
- **巩固标准**：
    - [ ] 能画出 select / poll / epoll 的内核遍历方式差异（线性遍历 vs 红黑树 + 就绪链表）。
      > **知识讲解**：差异在于**内核如何管理 fd 及告知就绪事件**。**select/poll**：每次调用都要把全量 fd 集合拷贝进内核，内核**线性遍历**所有 fd 逐个探测就绪，返回后用户态还要再遍历一遍找就绪 fd，复杂度 O(监控总数)；poll 仅去掉了 select 的 `FD_SETSIZE`（1024）硬限制，遍历本质没变。**epoll**：`epoll_ctl` 把 fd **一次性**注册到内核的**红黑树**中；数据到达时由中断路径触发回调（`ep_poll_callback`）将就绪 fd 挂入**就绪链表**；`epoll_wait` 只把就绪链表拷贝返回，复杂度降为 O(就绪数)。这就是 epoll 在高并发场景性能碾压的根本原因——10 万连接、每秒 5 千活跃时，select/poll 每次扫 10 万个 fd，epoll 只处理 5 千个。
    - [ ] 能解释 epoll 的 LT（水平触发）与 ET（边缘触发）的区别，以及各自的使用场景。
      > **知识讲解**：区别在于**就绪通知的触发时机**。LT（默认）：只要内核缓冲区**还有数据未读完**，每次 `epoll_wait` 都会**重复报告**，允许一次只读一部分。ET：只在状态**发生跳变的边缘**（无数据→有数据）报告一次，之后即使有剩余数据也不再通知。因此 ET 有两条铁律：① fd 必须设为**非阻塞**（O_NONBLOCK），否则最后一次 `read` 会阻塞线程；② 事件到来后必须**循环读到 `EAGAIN`**，否则漏事件。场景：LT 编程简单、容错性好，适合大多数业务服务器；ET 减少重复唤醒，性能更好，Nginx、Redis 及本练习的 6-5-reactor 均用 ET。易错点：ET 下用 if 而不是 while 读、忘设非阻塞、accept 只接受一个连接是三大经典 bug。
    - [ ] 能说出 Redis、Nginx、Netty 分别使用哪种 Reactor 模型。
      > **知识讲解**：**Redis——单线程 Reactor**：主线程用 epoll 监听所有事件（accept + 读写 + 定时器），命令在同一线程串行执行。成立前提是命令为**纯内存操作、耗时极短**，单线程反而避免锁竞争并天然保证原子性；代价是慢命令（大 key `DEL`）会阻塞整个实例，需 `UNLINK`/lazyfree 异步化。**Nginx——多进程 each-reactor**：master 不处理连接，每个 worker 进程持有独立的单线程 Reactor（accept + read + write + 业务同线程），通过 `SO_REUSEPORT`（或共享监听 fd + `accept_mutex`）分担新连接，worker 数 = CPU 核数，进程隔离无锁竞争。**Netty——主从多线程 Reactor**：boss 线程组（main-reactor）只负责 accept，把连接注册到 worker 线程组（sub-reactor）的某个 EventLoop，该连接的读写由固定线程处理，耗时业务再提交给独立**业务线程池**，实现 IO 与业务分离（即练习任务 4 的架构）。一句话对比：Redis 是"1 个 Reactor 干所有事"，Nginx 是"N 个互不相干的进程级 Reactor"，Netty 是"1 main + N sub + 业务线程池"。
    - [ ] 能解释 io_uring 相比 epoll 的优势（真正的异步、批量提交、共享环形缓冲区）。
      > **知识讲解**：io_uring（Linux 5.1 引入）从三个层面突破 epoll 的上限。**① 真正的异步**：epoll 本质是"就绪通知"——`epoll_wait` 只告诉你"fd 可以读了"，数据搬运仍需自己调 `read`/`write`；io_uring 提交 SQE（如 `IORING_OP_READ`）后，内核把数据**直接搬运到你指定的缓冲区**并通过 CQE 通知完成，应用拿到的就是结果数据本身。**② 批量提交/收割**：SQE（提交队列）/CQE（完成队列）放在**用户态与内核态共享（mmap）的环形缓冲区**里，一次 `io_uring_submit` 可提交多个操作，收割 CQE 甚至可**零系统调用**（内核直接写入 CQ 环，应用轮询即可），把系统调用次数从 O(操作数) 降为接近 O(1)。**③ 附带收益**：支持超时、链式依赖（`IOSQE_IO_LINK`）、零拷贝发送（`SEND_ZC`）、注册固定缓冲区/fd。代价：接口比 epoll 复杂得多，且部分内核版本有安全漏洞历史；高吞吐存储引擎、代理网关是典型受益者，工程上建议用 liburing 库而非裸 syscall。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 6.5 Reactor 模式 练习框架 ===
// 项目结构:
// 6-5-reactor/
// ├── Channel.h
// ├── Channel.cpp
// ├── EventLoop.h
// ├── EventLoop.cpp
// ├── Acceptor.h
// ├── Acceptor.cpp
// ├── EchoServer.h
// ├── EchoServer.cpp
// └── main.cpp

// ---------- Channel.h ----------
#pragma once
#include <functional>

class EventLoop;

// 封装 fd + 关注事件 + 回调
class Channel {
public:
    using EventCallback = std::function<void()>;
    Channel(EventLoop* loop, int fd);
    void setReadCallback(EventCallback cb)  { readCb_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCb_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCb_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCb_ = std::move(cb); }
    void enableReading(bool on = true);
    void enableWriting(bool on = true);
    void handleEvent(int revents);  // 由 EventLoop 调用
    int fd() const { return fd_; }
    int events() const { return events_; }  // 返回当前关注的 epoll 事件掩码

private:
    EventLoop* loop_;
    int fd_;
    int events_{0};  // 当前关注的 epoll 事件掩码
    EventCallback readCb_, writeCb_, closeCb_, errorCb_;
};

// ---------- Channel.cpp ----------
#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>

Channel::Channel(EventLoop* loop, int fd) : loop_(loop), fd_(fd) {}

void Channel::enableReading(bool on) {
    // TODO: 根据 on 设置 events_ |= EPOLLIN | EPOLLET 或 events_ &= ~(EPOLLIN | EPOLLET)
    // TODO: 调用 loop_->updateChannel(this)
}

void Channel::enableWriting(bool on) {
    // TODO: 根据 on 设置 events_ |= EPOLLOUT 或 events_ &= ~EPOLLOUT
    // TODO: 调用 loop_->updateChannel(this)
}

void Channel::handleEvent(int revents) {
    // TODO: 根据 revents 位调用对应回调
    // EPOLLERR | EPOLLHUP → errorCb_
    // EPOLLRDHUP         → closeCb_
    // EPOLLIN            → readCb_
    // EPOLLOUT           → writeCb_
}

// ---------- EventLoop.h ----------
#pragma once
#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void updateChannel(Channel* ch);
    void removeChannel(Channel* ch);
    // 练习2：跨线程投递任务
    void runInLoop(std::function<void()> fn);
    void queueInLoop(std::function<void()> fn);

private:
    void wakeup();  // 练习2：eventfd 唤醒
    void handlePendingTasks();

    int epollFd_;
    int wakeupFd_;  // eventfd
    std::atomic<bool> quit_{false};
    std::thread::id threadId_;  // 所属线程 ID，用于 runInLoop 判断
    std::mutex mtx_;
    std::queue<std::function<void()>> pendingTasks_;
    std::map<int, Channel*> channels_;  // fd -> Channel
};

// ---------- EventLoop.cpp ----------
#include "EventLoop.h"
#include "Channel.h"
#include <cstdlib>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <thread>
#include <unistd.h>
#include <array>
#include <utility>

EventLoop::EventLoop() : threadId_(std::this_thread::get_id()) {
    // TODO: epoll_create1(0) 创建 epoll 实例
    // TODO: eventfd(0, EFD_NONBLOCK) 创建唤醒 fd
    // TODO: 将 wakeupFd_ 注册到 epoll，监听 EPOLLIN
}

EventLoop::~EventLoop() {
    // TODO: 先从 epoll 移除 wakeupFd_，再 close(wakeupFd_) 和 close(epollFd_)
}

void EventLoop::loop() {
    std::array<struct epoll_event, 1024> events;
    while (!quit_) {
        // TODO: epoll_wait 阻塞等待事件（注意处理 EINTR）
        // TODO: 遍历就绪事件
        //   - 如果是 wakeupFd_ 触发：读取 eventfd 并调用 handlePendingTasks()
        //   - 否则：通过 channels_ 找到 Channel，调用 ch->handleEvent(ev.events)
    }
}

void EventLoop::quit() {
    quit_ = true;
    wakeup();  // 唤醒阻塞在 epoll_wait 的线程
}

void EventLoop::updateChannel(Channel* ch) {
    // TODO: 构造 epoll_event，设置 ch->events() 和 ch->fd()
    // TODO: 如果 channels_ 中没有此 Channel → epoll_ctl EPOLL_CTL_ADD
    // TODO: 如果已存在 → epoll_ctl EPOLL_CTL_MOD
    // TODO: 更新 channels_[ch->fd()] = ch
}

void EventLoop::removeChannel(Channel* ch) {
    // TODO: epoll_ctl EPOLL_CTL_DEL，然后 channels_.erase(ch->fd())
}

void EventLoop::wakeup() {
    // TODO: uint64_t val = 1; write(wakeupFd_, &val, sizeof(val)) 唤醒 epoll_wait
}

void EventLoop::runInLoop(std::function<void()> fn) {
    // TODO: 如果当前线程 == threadId_ 则直接执行 fn()
    // TODO: 否则调用 queueInLoop(std::move(fn))
}

void EventLoop::queueInLoop(std::function<void()> fn) {
    // TODO: 加锁 push 到 pendingTasks_，然后调用 wakeup()
}

void EventLoop::handlePendingTasks() {
    // TODO: 加锁 swap 出 pendingTasks_，逐个执行
}

// ---------- Acceptor.h ----------
#pragma once
#include <functional>
#include <utility>

class EventLoop;
class Channel;

class Acceptor {
public:
    using NewConnCallback = std::function<void(int fd)>;
    Acceptor(EventLoop* loop, int port);
    ~Acceptor();
    void setNewConnCallback(NewConnCallback cb) { newConnCb_ = std::move(cb); }
    void listen();

private:
    void handleRead();
    EventLoop* loop_;
    Channel* acceptChannel_;
    int listenFd_;
    NewConnCallback newConnCb_;
};

// ---------- Acceptor.cpp ----------
#include "Acceptor.h"
#include "EventLoop.h"
#include "Channel.h"
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

Acceptor::Acceptor(EventLoop* loop, int port) : loop_(loop) {
    // TODO: socket(AF_INET, SOCK_STREAM, 0) 创建监听 socket
    // TODO: setsockopt SO_REUSEADDR
    // TODO: bind 到 0.0.0.0:port
    // TODO: fcntl 设置非阻塞
    // TODO: acceptChannel_ = new Channel(loop_, listenFd_)
    // TODO: acceptChannel_->setReadCallback([this]() { handleRead(); })
}

Acceptor::~Acceptor() {
    // TODO: loop_->removeChannel(acceptChannel_), delete acceptChannel_, close(listenFd_)
}

void Acceptor::listen() {
    // TODO: ::listen(listenFd_, SOMAXCONN)
    // TODO: acceptChannel_->enableReading()
}

void Acceptor::handleRead() {
    // ET 模式下需要循环 accept 直到 EAGAIN
    while (true) {
        // TODO: accept4(listenFd_, ..., SOCK_NONBLOCK) 接受新连接
        // TODO: connFd < 0 时，EAGAIN/EWOULDBLOCK 则 break，EINTR 则 continue
        // TODO: 调用 newConnCb_(connFd)
    }
}

// ---------- EchoServer.h ----------
#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <map>

class EventLoop;
class Acceptor;
class Channel;

// 练习3：主从 Reactor 模型
struct TcpConnection {
    int fd;
    Channel* channel;
    EventLoop* loop;
    char buf[4096];
};

class EchoServer {
public:
    EchoServer(int port, int subReactorCount = 3);
    ~EchoServer();
    void start();
    void stop();

private:
    void onNewConnection(int fd);
    void handleRead(TcpConnection* conn);
    void removeConnection(TcpConnection* conn);

    std::unique_ptr<EventLoop> mainLoop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::vector<std::unique_ptr<EventLoop>> subLoops_;
    std::vector<std::thread> threads_;
    int nextSub_{0};
    std::mutex connMtx_;
    std::map<int, std::unique_ptr<TcpConnection>> connections_;  // fd -> conn
};

// ---------- EchoServer.cpp ----------
#include "EchoServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Channel.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <sys/socket.h>
#include <unistd.h>

EchoServer::EchoServer(int port, int subReactorCount) {
    mainLoop_ = std::make_unique<EventLoop>();
    // TODO: 创建 subReactorCount 个 subLoop
    // TODO: 创建 acceptor_，设置 newConnCallback 调用 onNewConnection
}

EchoServer::~EchoServer() {
    // TODO: 清理仍活跃的连接（removeChannel、delete channel、close fd）
    // TODO: 退出所有 sub-reactor，join 所有线程
}

void EchoServer::start() {
    // TODO: 为每个 subLoop 启动独立线程运行 loop()
    // TODO: acceptor_->listen()
    // TODO: mainLoop_->loop() 在当前线程运行
}

void EchoServer::onNewConnection(int fd) {
    // TODO: Round-Robin 选择 subLoops_[nextSub_]
    // TODO: sub->runInLoop 在 sub-reactor 线程中：
    //   1. 创建 TcpConnection，设置 fd、loop、channel
    //   2. 设置 channel 的 readCallback 和 closeCallback
    //   3. channel->enableReading()
    //   4. 加锁存入 connections_
}

void EchoServer::handleRead(TcpConnection* conn) {
    // ET 模式：循环读取直到 EAGAIN
    while (true) {
        // TODO: recv 读取数据
        //   n > 0: send 回显
        //   n == 0: 对端关闭，break
        //   n < 0: EAGAIN/EWOULDBLOCK 则 break，EINTR 则 continue
    }
}

void EchoServer::removeConnection(TcpConnection* conn) {
    // TODO: 在所属 sub-reactor 线程中安全移除：
    //   加锁从 connections_ 找到并移除
    //   removeChannel、delete channel、close fd
}

// ---------- main.cpp ----------
#include "EchoServer.h"
#include <csignal>

// TODO: 声明全局 EchoServer* g_server 指针，供信号处理器访问
// TODO: 实现 signalHandler：调用 g_server->stop()

int main() {
    EchoServer server(8888, 3);
    // TODO: 设置 g_server = &server，注册 signal(SIGINT, signalHandler)
    server.start(); // Ctrl+C → handler → quit() → wakeup → loop() 退出 → 析构
    return 0;
}
```

</details>

<details>
<summary>🧪 测试步骤</summary>

需要 **2 个终端**：

| 步骤 | 终端 A（服务器） | 终端 B（客户端） |
|------|------------------|------------------|
| 1 | 编译运行 `./reactor` | |
| 2 | 打印 `EchoServer started on port, 3 sub-reactors`（主从 Reactor 启动完成） | |
| 3 | | 新开终端执行 `nc localhost 8888` |
| 4 | | 输入 `hello` 回车 |
| 5 | | 看到服务器回显 `hello` |
| 6 | | 再输入 `world` 回车 |
| 7 | | 看到服务器回显 `world` |
| 8 | | `Ctrl+D` 或 `Ctrl+C` 关闭连接 |
| 9 | 无输出（静默 close，连接从 connections_ 移除） | |

**验证要点**：

- **多连接并发**：新开多个终端分别 `nc localhost 8888`，每个连接都能独立收发回显，互不干扰——说明连接被 Round-Robin 分配到不同的 sub-reactor 线程。
- **主从 Reactor 分工**：main-reactor 只负责 accept 新连接（不处理读写），连接建立后读写事件由所属 sub-reactor 处理——这是 Netty 主从 Reactor 模型的经典实现。
- **ET 模式行为**：服务器使用 `EPOLLET` 边缘触发，`handleRead` 必须循环读到 `EAGAIN` 为止，否则会漏事件；这也是本练习的核心练习点。
- **优雅关闭**：服务器 `Ctrl+C` 退出时，析构函数会 `quit()` 所有 sub-reactor 并 `join` 工作线程，无僵尸线程或内存泄漏。

</details>

---

# 阶段七：数据库、缓存与 HTTP

> **定位**：综合项目的前置知识——为项目补齐数据存储（MySQL）、缓存加速（Redis）与 Web 协议解析（HTTP）三大能力。

## 7.1 MySQL 基础与 C++ 接入

- **练习目标**：
    - 理解关系型数据库的基本概念：表、行、列、主键、索引、事务。
    - 掌握 SQL 基础：SELECT / INSERT / UPDATE / DELETE、JOIN、GROUP BY、索引。
    - 掌握 C++ 接入 MySQL 的方式：MySQL Connector/C++ 或 libmariadbclient。
    - 理解预处理语句（Prepared Statement）的作用：防 SQL 注入、提升批量性能。
- **练习任务**：
    1. 设计一张"用户表"（id / username / password_hash / created_at），建表并加索引。
    2. 用 C++ 实现用户注册：插入一条记录，密码用 bcrypt 哈希存储。
    3. 用预处理语句实现用户登录：按 username 查询，比对密码哈希。
    4. 实现一个"用户信息 CRUD"类，封装所有 SQL 操作，使用 RAII 管理连接与 statement。
    5. 演示 SQL 注入场景（拼接字符串 vs 预处理），理解为什么必须用预处理。
- **巩固标准**：
    - [ ] 能解释主键、唯一索引、普通索引的区别与适用场景。
    > **知识讲解**：主键（PRIMARY KEY）是唯一标识一行数据的列，自动创建聚簇索引（InnoDB 中数据按主键顺序物理存储），隐含 NOT NULL + UNIQUE 约束，每张表只能有一个。唯一索引（UNIQUE INDEX）保证列值不重复，允许 NULL（NULL 不参与唯一性检查），适合 username、email 等业务唯一字段，一张表可有多个。普通索引（INDEX）仅加速查询，不施加任何约束，适合高频 WHERE 条件列（如 created_at、status）。选择原则：**必须唯一标识行 → 主键；业务要求不重复 → 唯一索引；纯粹为了查询加速 → 普通索引**。注意索引并非越多越好——每个索引都会增加 INSERT/UPDATE/DELETE 的开销（需同步维护索引树），且占用额外磁盘空间，通常一张表 3~5 个索引为宜。
    - [ ] 能说出预处理语句为什么能防 SQL 注入（参数与 SQL 分离）。
    > **知识讲解**：SQL 注入的本质是用户输入被当作 SQL 语法解析——例如拼接 `"WHERE username = '" + input + "'"`，当 input 为 `' OR '1'='1` 时，SQL 变为 `WHERE username = '' OR '1'='1'`，恒真条件返回全部行。预处理语句（Prepared Statement）通过**两阶段执行**从根本上杜绝此问题：① 先发送 SQL 模板 `SELECT * FROM users WHERE username = ?` 给服务器，服务器完成语法解析、生成执行计划；② 再单独发送参数值，参数被严格当作纯数据处理，不会被重新解析为 SQL 语法。无论参数内容是什么（引号、分号、注释符），都只影响数据比较结果，不可能改变 SQL 结构。此外预处理语句还有性能优势：同一模板多次执行时，服务器复用已编译的执行计划，省去重复解析开销，批量插入场景尤为明显。
    - [ ] 能用 EXPLAIN 分析一条 SQL 的执行计划，判断是否命中索引。
    > **知识讲解**：`EXPLAIN SELECT ...` 在 SQL 前加 EXPLAIN 关键字即可让 MySQL 返回执行计划而不真正执行查询。MySQL 26.x 默认输出树形简略格式（如 `-> Rows fetched before execution (cost=0..0 rows=1)`），适合日常快速查看；加 `EXPLAIN FORMAT=TRADITIONAL` 可切换为传统表格格式，适合深度调优。传统表格核心关注以下列：**type**（访问类型，从优到劣依次为 system > const > eq_ref > ref > range > index > ALL，ALL 即全表扫描应避免）、**key**（实际使用的索引名，NULL 表示未命中索引）、**rows**（预估扫描行数，越小越好）、**Extra**（附加信息，`Using index` 表示覆盖索引最优，`Using where` 表示回表过滤，`Using filesort` / `Using temporary` 表示需要额外排序或临时表，应优化）。以本项目为例：`EXPLAIN FORMAT=TRADITIONAL SELECT * FROM users WHERE username = 'alice'` 应显示 type=const、key=username，说明命中了唯一索引；若去掉索引则 type=ALL、key=NULL，全表扫描。养成"写复杂查询前先 EXPLAIN"的习惯，是数据库性能调优的基本功。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 7.1 MySQL 基础 练习框架 ===
// 项目结构:
// 7-1-mysql-basic/
// ├── UserRepo.h
// ├── UserRepo.cpp
// └── main.cpp

// ---------- UserRepo.h ----------
#pragma once
#include <string>

struct User {
    int id;
    std::string username;
    std::string passwordHash;
    std::string createdAt;
};

// 练习4: RAII 封装的 CRUD 类
class UserRepo {
public:
    UserRepo(const char* host, const char* user, const char* pwd, const char* db, int port = 3306);
    ~UserRepo();

    // 练习2: 用户注册（密码用 bcrypt 哈希存储）
    bool registerUser(const std::string& username, const std::string& password);
    // 练习3: 用预处理语句实现登录
    bool login(const std::string& username, const std::string& password);
    // 练习5: 演示 SQL 注入场景
    void demoInjection();

private:
    void* conn_;  // MYSQL* 实际类型
};

// ---------- UserRepo.cpp ----------
#include "UserRepo.h"
#include <mysql/mysql.h>
#include <crypt.h>
#include <cstdio>

UserRepo::UserRepo(const char* host, const char* user, const char* pwd, const char* db, int port) {
    // TODO: conn_ = mysql_init(nullptr)
    // TODO: mysql_real_connect(...)
}

UserRepo::~UserRepo() {
    // TODO: mysql_close((MYSQL*)conn_)
}

bool UserRepo::registerUser(const std::string& username, const std::string& password) {
    // TODO: 用预处理语句 INSERT INTO users(username, password_hash) VALUES (?, ?)
    // TODO: 密码用 bcrypt 哈希（可用第三方库）
    return false;
}

bool UserRepo::login(const std::string& username, const std::string& password) {
    // TODO: 预处理 SELECT password_hash FROM users WHERE username = ?
    // TODO: 比对 bcrypt 哈希
    return false;
}

void UserRepo::demoInjection() {
    // TODO: 演示拼接字符串 "WHERE username = '" + input + "'" 被 SQL 注入
    // TODO: 对比预处理语句的安全性
}

// ---------- main.cpp ----------
#include "UserRepo.h"

int main() {
    UserRepo repo("127.0.0.1", "root", "password", "testdb");
    // TODO: 测试 registerUser / login / demoInjection
    return 0;
}
```

</details>

## 7.2 连接池与事务

- **练习目标**：
    - 理解为什么需要连接池（避免频繁建连开销）。
    - 实现一个简单的 MySQL 连接池（基于 `std::queue` + `std::mutex` + `std::condition_variable`）。
    - 理解事务的 ACID 特性与四种隔离级别（读未提交 / 读已提交 / 可重复读 / 串行化）。
    - 掌握 C++ 中的事务封装（BEGIN / COMMIT / ROLLBACK）。
- **练习任务**：
    1. 实现一个连接池类：`acquire()` 获取连接、`release()` 归还、支持最大连接数限制与超时。
    2. 用连接池压测：100 个线程并发执行 1000 次查询，对比"每次新建连接"与"使用连接池"的耗时。
    3. 实现一个"转账"事务：A 减 100、B 加 100，模拟中途失败回滚。
    4. 演示"脏读 / 不可重复读 / 幻读"场景，切换隔离级别观察变化。
- **巩固标准**：
    - [ ] 能画出连接池的获取 / 归还 / 扩容流程。
    > **知识讲解**：连接池的核心数据结构是 `std::queue<MYSQL*>` + `std::mutex` + `std::condition_variable`。**获取流程**（acquire）：加锁 → 若队列为空且当前连接数未达 `maxConn_`，则 `mysql_init` + `mysql_real_connect` 新建连接并返回（扩容）；若队列空且已达上限，则 `cv.wait(lock, pred, timeout)` 阻塞等待，超时返回 `nullptr`；若队列非空则 `pop` 取出连接返回。**归还流程**（release）：加锁 → `push` 连接回队列 → `cv.notify_one()` 唤醒一个等待线程。**扩容策略**：通常在构造时预创建 `minConn_` 个连接（预热），按需增长直到 `maxConn_`；也有实现采用 LIFO 栈（`std::stack`）代替 FIFO 队列，使最近使用过的连接优先被复用，利用 CPU 缓存局部性提升性能。关键不变量：**任意时刻，池中连接数 + 被借出连接数 = 已创建总连接数 ≤ maxConn_**。
    - [ ] 能解释四种隔离级别分别解决了什么问题、引入了什么开销。
    > **知识讲解**：并发事务带来的三类问题是递进关系——**脏读**（读到别的事务未提交的中间状态，对方回滚后数据即"脏"）→ **不可重复读**（同一事务内两次 SELECT 结果不同，因为别的事务在此期间 COMMIT 了）→ **幻读**（两次范围查询结果行数不同，因为别的事务 INSERT/DELETE 了新行）。四种隔离级别从低到高：**READ UNCOMMITTED**（读未提交）几乎无额外开销，但允许脏读，生产环境极少使用；**READ COMMITTED**（读已提交）通过行级锁保证每次 SELECT 只读到已提交数据，解决脏读，但同一事务内两次查询可能结果不同（不可重复读），Oracle 默认级别；**REPEATABLE READ**（可重复读）是 MySQL 默认级别，通过 MVCC（多版本并发控制）快照读保证同一事务内多次 SELECT 结果一致，解决不可重复读，InnoDB 还配合 Gap Lock（间隙锁）在索引范围上加锁阻止其他事务插入新行，从而大幅减少幻读，但 Gap Lock 会锁定索引范围导致并发写入性能下降；**SERIALIZABLE**（串行化）强制所有事务串行执行，彻底解决三类问题，但并发性能最差。工程权衡：绝大多数业务使用默认的 REPEATABLE READ 即可，只有金融对账等极端场景才需要 SERIALIZABLE，而高并发读多写少场景可降级到 READ COMMITTED 换取吞吐量。
    - [ ] 能说出"连接必须归还"的 RAII 封装方式（`std::shared_ptr` + 自定义 deleter）。
    > **知识讲解**：裸指针方式获取连接后若中途 `return` 或抛异常，极易忘记调用 `release()` 导致连接泄漏、池逐渐枯竭。RAII 封装方案：`acquire()` 返回 `std::shared_ptr<MYSQL>`，构造时传入自定义 deleter `[pool](MYSQL* conn) { pool->release(conn); }`，这样连接指针离开作用域时（无论是正常返回、提前 return 还是异常抛出），`shared_ptr` 析构自动调用 deleter 将连接归还池中，无需手动管理。示例：`auto conn = pool.acquire(); mysql_query(conn.get(), "SELECT ..."); // 函数结束 conn 自动归还`。为什么不选 `unique_ptr`？因为 `unique_ptr` 的 deleter 类型是模板参数，会在编译期实例化，导致头文件必须暴露 `ConnectionPool` 完整定义（否则无法实例化 deleter）；而 `shared_ptr` 的 deleter 是类型擦除的运行时多态，头文件只需前置声明即可，编译隔离性更好——这也是本练习框架中 `ConnPtr` 定义为 `std::shared_ptr<void>` 的原因。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 7.2 连接池与事务 练习框架 ===
// 项目结构:
// 7-2-pool/
// ├── ConnectionPool.h
// ├── ConnectionPool.cpp
// └── main.cpp

// ---------- ConnectionPool.h ----------
#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

// 练习1: 连接池
class ConnectionPool {
public:
    ConnectionPool(const std::string& host, const std::string& user,
                   const std::string& pwd, const std::string& db,
                   size_t maxConn = 10);
    ~ConnectionPool();

    // RAII 包装：返回 shared_ptr，自定义 deleter 自动归还连接
    using ConnPtr = std::shared_ptr<void>;
    ConnPtr acquire(int timeoutMs = 3000);

    // 练习3: 事务封装
    bool transferMoney(int fromId, int toId, double amount);

private:
    void* createConnection();
    void destroyConnection(void* conn);

    std::string host_, user_, pwd_, db_;
    size_t maxConn_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<void*> pool_;
    size_t totalCreated_{0};
};

// ---------- ConnectionPool.cpp ----------
#include "ConnectionPool.h"
#include <mysql/mysql.h>

ConnectionPool::ConnectionPool(const std::string& h, const std::string& u,
                               const std::string& p, const std::string& d, size_t max)
    : host_(h), user_(u), pwd_(p), db_(d), maxConn_(max) {
    // TODO: 预创建几个连接放入 pool_
}

ConnectionPool::~ConnectionPool() {
    // TODO: 销毁所有连接
}

void* ConnectionPool::createConnection() {
    // TODO: mysql_init + mysql_real_connect
    return nullptr;
}

void ConnectionPool::destroyConnection(void* conn) {
    // TODO: mysql_close
}

ConnectionPool::ConnPtr ConnectionPool::acquire(int timeoutMs) {
    std::unique_lock<std::mutex> lk(mtx_);
    // TODO: cv_.wait_for 等待 pool_ 非空或超时
    // TODO: 若 pool_ 空且 totalCreated_ < maxConn_，新建连接
    // TODO: 返回 shared_ptr<void>(conn, [this](void* c){ 归还到 pool_; cv_.notify_one(); })
    return nullptr;
}

bool ConnectionPool::transferMoney(int fromId, int toId, double amount) {
    auto conn = acquire();
    if (!conn) return false;
    MYSQL* sql = static_cast<MYSQL*>(conn.get());
    // TODO: mysql_query(sql, "START TRANSACTION")
    // TODO: UPDATE accounts SET balance = balance - ? WHERE id = ?
    // TODO: UPDATE accounts SET balance = balance + ? WHERE id = ?
    // TODO: 任一失败则 mysql_query(sql, "ROLLBACK") 并 return false
    // TODO: mysql_query(sql, "COMMIT")
    return false;
}

// ---------- main.cpp ----------
#include "ConnectionPool.h"
#include <iostream>
#include <thread>
#include <vector>

int main() {
    ConnectionPool pool("127.0.0.1", "root", "password", "testdb", 10);
    // 练习2: 压测对比
    // TODO: 100 个线程并发执行 1000 次查询，对比“每次新建连接”与“使用连接池”的耗时
    return 0;
}
```

</details>

## 7.3 Redis 基础与 C++ 接入

- **练习目标**：
    - 理解 Redis 的数据结构：string / hash / list / set / zset，以及常见应用场景。
    - 掌握 Redis 的持久化：RDB（快照）与 AOF（追加日志）。
    - 掌握 C++ 接入 Redis 的方式：hiredis 或 redis-plus-plus。
    - 理解 Redis 的常见应用：缓存、会话、限流、排行榜、分布式锁。
- **练习任务**：
    1. 用 redis-cli 练习五种基本数据结构的常用命令。
    2. 用 redis-plus-plus 实现"用户会话"：登录后 set session_id → user_id，TTL 30 分钟。
    3. 实现"接口限流"：用 INCR + EXPIRE 做滑动窗口计数。
    4. 实现"排行榜"：用 zset 存用户分数，按分数排序取 top 10。
    5. 配置 Redis 的 RDB + AOF 持久化，验证重启后数据恢复。
- **巩固标准**：
    - [ ] 能说出五种数据结构的典型应用场景（至少各 2 个）。
    > **知识讲解**：**string**——缓存会话信息（session_id → user_id）、计数器（INCR 限流/点赞数）、分布式锁（SET key value NX EX）。**hash**——存储对象（用户信息 user:id → {name, age, email}）、购物车（cart:userId → {商品id: 数量}）。**list**——消息队列（LPUSH 生产 + BRPOP 消费）、最新动态（LPUSH + LRANGE 取最新 N 条）。**set**——标签系统（SADD 文章标签）、共同关注（SINTER 两个用户的关注集合交集）、去重（SISMEMBER 判断是否已读）。**zset（有序集合）**——排行榜（ZADD 分数 + ZREVRANGE 取 top N）、延迟队列（score 为执行时间戳）、带权重的任务调度。选择原则：简单 KV → string；结构化对象且需部分更新 → hash；有序列表/队列 → list；集合运算（交并差） → set；需要按分数排序 → zset。
    - [ ] 能对比 RDB 与 AOF 的优缺点（恢复速度 vs 数据完整性）。
    > **知识讲解**：**RDB（快照）**在指定条件触发时生成某一时刻的全量二进制快照（dump.rdb），优点是文件紧凑、恢复速度快（直接加载二进制到内存），适合备份和灾难恢复；缺点是两次快照之间的写入可能丢失（如 `save 60 1` 最多丢 60 秒数据），且 fork 子进程生成快照时大内存可能阻塞。**AOF（追加日志）**将每条写命令追加到日志文件（appendonly.aof），优点是可以配置 `appendfsync everysec`（每秒刷盘，最多丢 1 秒数据）或 `always`（每条刷盘，零丢失但最慢），数据完整性更高；缺点是文件比 RDB 大、恢复速度慢（需重放所有命令）。**工程实践**：生产环境通常 RDB + AOF 同时开启——AOF 保证数据安全，RDB 用于快速恢复和定期备份。Redis 7+ 采用 Multi-part AOF（base.rdb + incr.aof + manifest），结合了 RDB 的快速加载和 AOF 的增量记录，恢复时先加载 base.rdb 再重放 incr.aof，兼顾两者优势。
    - [ ] 能解释 Redis 单线程为什么这么快（纯内存、IO 多路复用、高效数据结构）。
    > **知识讲解**：Redis 核心命令处理采用单线程模型（Redis 6+ 引入多线程处理网络 IO，但命令执行仍是单线程），快的原因有三：① **纯内存操作**——所有数据在内存中，读写延迟纳秒级，无磁盘 IO 瓶颈，相比数据库（磁盘随机访问毫秒级）快 3~4 个数量级；② **IO 多路复用**——基于 epoll/kqueue 实现事件驱动，单线程同时监听成千上万个连接的读写事件，避免线程切换开销；③ **高效数据结构**——每种操作都针对底层数据结构优化，如 dict（哈希表）O(1) 查找、skiplist（跳表）O(logN) 范围查询、ziplist/listpack（压缩列表）小数据量下内存连续减少 cache miss、SDS（简单动态字符串）预分配减少内存重分配。此外单线程避免了多线程的锁竞争和上下文切换开销，命令执行是原子的，无需考虑并发一致性。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 7.3 Redis 基础 练习框架 ===
// 项目结构:
// 7-3-redis/
// ├── RedisClient.h
// ├── RedisClient.cpp
// └── main.cpp

// ---------- RedisClient.h ----------
#pragma once
#include <string>
#include <vector>
#include <utility>
#include <memory>

// 前置声明 redis-plus-plus 类型
namespace sw { namespace redis { class Redis; } }

// 练习2~4: RAII 封装的 Redis 客户端
class RedisClient {
public:
    RedisClient(const std::string& host = "tcp://127.0.0.1:6379");
    ~RedisClient();

    // 练习2: 用户会话
    bool setSession(const std::string& sessionId, const std::string& userId, int ttlSec = 1800);
    std::string getSession(const std::string& sessionId);

    // 练习3: 接口限流（INCR + EXPIRE 滑动窗口）
    bool isRateLimited(const std::string& key, int maxCount, int windowSec);

    // 练习4: 排行榜（zset）
    bool updateScore(const std::string& key, const std::string& user, double score);
    std::vector<std::pair<std::string, double>> topN(const std::string& key, int n);

private:
    std::unique_ptr<sw::redis::Redis> redis_;
};

// ---------- RedisClient.cpp ----------
#include "RedisClient.h"
#include <sw/redis++/redis++.h>
#include <chrono>

RedisClient::RedisClient(const std::string& host) {
    // TODO: redis_ = std::make_unique<sw::redis::Redis>(host)
}

RedisClient::~RedisClient() = default;  // unique_ptr 自动析构

bool RedisClient::setSession(const std::string& sessionId, const std::string& userId, int ttlSec) {
    // TODO: redis_->set(sessionId, userId, std::chrono::seconds(ttlSec))
    return false;
}

std::string RedisClient::getSession(const std::string& sessionId) {
    // TODO: auto val = redis_->get(sessionId)
    // TODO: return val ? *val : ""
    return "";
}

bool RedisClient::isRateLimited(const std::string& key, int maxCount, int windowSec) {
    // TODO: auto cnt = redis_->incr(key)
    // TODO: if (cnt == 1) redis_->expire(key, std::chrono::seconds(windowSec))
    // TODO: return cnt > maxCount
    return false;
}

bool RedisClient::updateScore(const std::string& key, const std::string& user, double score) {
    // TODO: redis_->zadd(key, user, score)
    return false;
}

std::vector<std::pair<std::string, double>> RedisClient::topN(const std::string& key, int n) {
    // TODO: std::vector<std::pair<std::string, double>> result
    // TODO: redis_->zrevrange(key, 0, n - 1, std::back_inserter(result))
    // TODO: return result
    return {};
}

// ---------- main.cpp ----------
#include "RedisClient.h"
#include <iostream>

int main() {
    RedisClient cli;
    // TODO: 测试 setSession / getSession / isRateLimited / updateScore / topN
    return 0;
}
```

</details>

<details>
<summary>🧪 测试步骤</summary>

需要 **1 个终端运行 Redis（Docker）** + **1 个终端运行程序**：

**前置：启动 Redis 服务器**

```bash
# 启动 Redis 容器，映射端口并开启 RDB + AOF 持久化
# -p 6379:6379    将容器内 6379 端口映射到宿主机，程序才能连接
# -v .../data:/data  挂载数据目录到宿主机，方便观察持久化文件
docker run --name some-redis -d -p 6379:6379 \
    -v ~/workspace/linux-backend/code/stage07/7-3-redis/data:/data \
    redis redis-server --save 60 1 --appendonly yes --loglevel warning

# 验证 Redis 已启动
docker exec -it some-redis redis-cli ping
# 应返回: PONG
```

**练习 2~4：运行程序测试功能**

| 步骤 | 操作 | 预期输出 |
|------|------|----------|
| 1 | 编译运行 `./build/redisExercises` | `=== 测试用户会话 ===` |
| 2 | 观察会话测试结果 | `Session session_001 -> userId: user_123` |
| 3 | 观察限流测试结果 | 前 3 次 `ALLOWED`，第 4、5 次 `LIMITED` |
| 4 | 观察排行榜测试结果 | Top 3: david(97.8) > alice(95.5) > charlie(92.3) |

**练习 5：持久化验证（RDB + AOF）**

| 步骤 | 操作 | 说明 |
|------|------|------|
| 1 | `docker exec -it some-redis redis-cli` 进入 redis-cli | 连接 Redis 服务器 |
| 2 | `SET test:persistence "hello_redis"` | 写入测试数据（string 类型） |
| 3 | `SET test:user:1 "alice"` | 写入第二条测试数据 |
| 4 | `ZADD test:leaderboard 100 "player_a"` | 写入有序集合（排行榜数据） |
| 5 | `ZADD test:leaderboard 200 "player_b"` | 写入第二条有序集合数据 |
| 6 | `GET test:persistence` 确认返回 `"hello_redis"` | 验证数据写入成功 |
| 7 | `SAVE` 手动触发 RDB 快照保存 | 强制生成 dump.rdb，不用等 60 秒条件 |
| 8 | `exit` 退出 redis-cli | |
| 9 | `ls ~/workspace/linux-backend/code/stage07/7-3-redis/data/` | 应看到 `dump.rdb` 和 `appendonlydir/` |
| 10 | `docker restart some-redis` 重启容器 | 模拟服务器重启，验证持久化效果 |
| 11 | `docker exec -it some-redis redis-cli` 重新连接 | |
| 12 | `GET test:persistence` 确认仍返回 `"hello_redis"` | 验证数据从持久化文件恢复成功 |
| 13 | `ZREVRANGE test:leaderboard 0 -1 WITHSCORES` 确认排行榜数据还在 | 验证有序集合也成功恢复 |

**验证要点**：

- **用户会话**：`setSession` 写入带 TTL 的 key，`getSession` 正确读取——验证 SET/GET 命令与 `std::chrono::seconds` TTL 参数生效。
- **接口限流**：`isRateLimited` 首次 INCR 返回 1 时设置 EXPIRE，后续请求递增计数，超过 `maxCount` 后返回 `true`——验证 INCR + EXPIRE 组合实现滑动窗口限流。
- **排行榜**：`updateScore` 用 ZADD 写入分数，`topN` 用 ZREVRANGE 按分数降序取 top N——验证有序集合的写入与查询。
- **RDB 与 AOF 对比**：

| 对比项 | RDB（快照） | AOF（追加日志） |
|------|------------|----------------|
| 文件 | `dump.rdb`（二进制快照） | `appendonlydir/`（base.rdb + incr.aof） |
| 触发方式 | `SAVE`/`BGSAVE` 或条件触发（如 `save 60 1`） | 每条写命令自动追加 |
| 恢复速度 | 快（直接加载二进制到内存） | 慢（需重放命令日志） |
| 数据完整性 | 可能丢失最后一次快照后的写入 | 最多丢 1 秒数据（`appendfsync everysec`） |
| 文件大小 | 小（压缩二进制） | 大（文本命令日志） |
| 适用场景 | 灾难恢复、定期备份 | 数据安全优先的场景 |

生产环境通常 **RDB + AOF 同时开启**——RDB 用于快速恢复和备份，AOF 用于更高的数据安全性。
- **连接异常**：如果 Redis 未启动或端口未映射（`-p 6379:6379`），所有操作会抛异常被 catch 捕获，输出 `Connection refused`——说明 Docker 容器必须映射端口才能从宿主机访问。

</details>

## 7.4 缓存策略与一致性

- **练习目标**：
    - 掌握常见的缓存策略：Cache Aside、Read/Write Through、Write Behind。
    - 理解缓存穿透、缓存击穿、缓存雪崩的成因与解决方案。
    - 理解数据库与缓存一致性的挑战与常用方案（延迟双删、Binlog 订阅）。
- **练习任务**：
    1. 实现 Cache Aside 模式：先查缓存，未命中再查 DB，结果回填缓存。
    2. 模拟"缓存穿透"：对不存在的 key 频繁查询，加"空值缓存 + 短 TTL"或布隆过滤器解决。
    3. 模拟"缓存击穿"：热点 key 过期瞬间大量请求打到 DB，用互斥锁或永不过期 + 异步刷新解决。
    4. 实现"延迟双删"：更新 DB 前删缓存、更新 DB 后延迟再删一次，验证一致性。
- **巩固标准**：
    - [ ] 能画出 Cache Aside 的读写流程。
      > **知识讲解**：**Cache Aside（旁路缓存）** 是最常用的缓存策略，核心流程为——**读操作**：先查缓存，命中直接返回；未命中则查 DB，将结果回填缓存并设 TTL，下次请求直接命中缓存。**写操作**：先删缓存，再更新 DB（也可先更新 DB 再删缓存）。选择"先删缓存再更新 DB"的原因是：如果先更新 DB 再删缓存，在删缓存之前另一个线程读到旧缓存并回填，会导致缓存与 DB 不一致。Cache Aside 的优势是简单可靠，缓存只保存被访问过的数据（按需加载），不会浪费内存存冷数据；缺点是首次访问和缓存失效时会有"缓存穿透到 DB"的延迟。TTL 的选择需要权衡：太短则缓存命中率低、DB 压力大；太长则数据陈旧、一致性差。通常业务热点数据设 60~300 秒，配合业务更新时主动删除缓存来保证最终一致性。
    - [ ] 能对比"缓存穿透 / 击穿 / 雪崩"三种问题的区别与各自解决方案。
      > **知识讲解**：三者的区别在于**触发条件**和**影响范围**不同：
      >
      > | 问题 | 触发条件 | 影响范围 | 解决方案 |
      > |------|----------|----------|----------|
      > | **缓存穿透** | 查询**不存在**的 key，缓存和 DB 都无数据 | 每次请求都打到 DB | 空值缓存 + 短 TTL（5s）；布隆过滤器拦截 |
      > | **缓存击穿** | **单个热点** key 过期瞬间，大量并发请求同时查 DB | 单个 key 对应的 DB 查询暴增 | 互斥锁（只允许一个线程查 DB 并回填）；热点 key 永不过期 + 异步刷新 |
      > | **缓存雪崩** | **大量** key 同时过期，或 Redis 整体宕机 | 所有请求打到 DB，可能导致 DB 崩溃 | TTL 加随机偏移避免集中过期；多级缓存（本地缓存 + Redis）；限流降级 |
      >
      > **工程实践**：穿透用空值缓存最简单，但如果攻击者构造大量不存在的 key，空值缓存本身也会占满内存，此时布隆过滤器更合适（O(1) 判断 key 是否存在，误判率可控在 1% 以内，内存占用极小）。击穿用互斥锁时注意锁粒度——应该 per-key 而非全局锁，否则一个 key 的慢查询会阻塞所有其他 key 的访问。雪崩的根因是"集中过期"，给 TTL 加随机值（如 `base + rand(0, 60)`）就能大幅缓解。
    - [ ] 能解释为什么"强一致性"在缓存场景很难做到，以及业务上如何取舍。
      > **知识讲解**：缓存与 DB 的**强一致性**要求"任何时刻缓存和 DB 的数据完全相同"，但这在分布式系统中几乎不可能做到，原因有三：① **非原子操作**——删缓存和更新 DB 是两个独立操作，中间存在时间窗口，其他线程可能读到旧缓存或旧 DB； **并发竞争**——线程 A 删缓存 → 线程 B 读旧 DB 回填旧缓存 → 线程 A 更新 DB，最终缓存是旧值；③ **网络延迟**——缓存和 DB 可能是不同节点，同步存在延迟。**工程取舍**：大多数业务接受"最终一致性"——通过延迟双删（删缓存 → 更新 DB → sleep 200ms → 再删缓存）将不一致窗口从秒级压缩到毫秒级；更严格的场景用 Binlog 订阅（如 Canal）监听 DB 变更事件，异步删除/更新缓存，保证最终一致。对于读多写少的场景（如商品详情），Cache Aside + TTL 已经足够；对于写多读少或强一致性要求的场景（如金融余额），应该绕过缓存直接读 DB。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 7.4 缓存策略 练习框架 ===
// 项目结构:
// 7-4-cache/
// ├── CacheAside.h
// ├── CacheAside.cpp
// └── main.cpp

// ---------- CacheAside.h ----------
#pragma once
#include <string>
#include <functional>
#include <memory>

// 前置声明 redis-plus-plus 类型
namespace sw { namespace redis { class Redis; } }

// 模拟 DB 查询接口（只读）
using DbQuery = std::function<std::string(const std::string& key)>;
// 模拟 DB 更新接口（写）
using DbUpdater = std::function<void(const std::string& key, const std::string& value)>;

// 练习1: Cache Aside 模式
// 读: 先查缓存 → 未命中查 DB → 回填缓存(带 TTL)
// 写: 先删缓存 → 更新 DB → 回填缓存
// 注意: DbQuery 只能读，写操作需要额外注入 DbUpdater
class CacheAside {
public:
    explicit CacheAside(DbQuery dbQuery);
    void setDbUpdater(DbUpdater updater);
    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value, int ttlSec = 60);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    DbQuery dbQuery_;
    DbUpdater dbUpdater_;
};

// 练习2: 缓存穿透防护（空值缓存 + 短 TTL）
// DB 查不到时存 "NULL" 哨兵值（短 TTL 5s），后续请求命中哨兵直接返回空
class AntiPenetrationCache {
public:
    explicit AntiPenetrationCache(DbQuery dbQuery);
    std::string get(const std::string& key);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    DbQuery dbQuery_;
};

// 练习3: 缓存击穿防护（互斥锁）
// 热点 key 过期瞬间，用 per-key 互斥锁保证只有一个线程查 DB 并回填
class AntiBreakdownCache {
public:
    explicit AntiBreakdownCache(DbQuery dbQuery);
    std::string get(const std::string& key);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    DbQuery dbQuery_;
};

// 练习4: 延迟双删
// 删缓存 → 更新 DB → sleep(200ms) → 再删缓存
// 目的：清除"更新 DB 期间"被其他线程读旧值回填的脏缓存
class DelayDoubleDelete {
public:
    explicit DelayDoubleDelete(DbQuery dbQuery);
    void setDbUpdater(DbUpdater updater);
    void update(const std::string& key, const std::string& newValue);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    DbQuery dbQuery_;
    DbUpdater dbUpdater_;
};

// ---------- CacheAside.cpp ----------
#include "CacheAside.h"
#include <sw/redis++/redis++.h>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

// 空值缓存哨兵值与短 TTL
// TODO: constexpr const char* NULL_SENTINEL = "NULL";
// TODO: constexpr int NULL_TTL_SEC = 5;

// 练习1
CacheAside::CacheAside(DbQuery q)
    : redis_(std::make_shared<sw::redis::Redis>("tcp://127.0.0.1:6379")),
      dbQuery_(std::move(q)) {}

void CacheAside::setDbUpdater(DbUpdater updater) {
    // TODO: dbUpdater_ = std::move(updater)
}

std::string CacheAside::get(const std::string& key) {
    // TODO: auto val = redis_->get(key)
    // TODO: if (val) 命中缓存，返回 *val
    // TODO: 未命中则调 dbQuery_(key)
    // TODO: 结果不为空则 redis_->set(key, value, std::chrono::seconds(60))
    return "";
}

void CacheAside::set(const std::string& key, const std::string& value, int ttlSec) {
    // TODO: redis_->del(key) 删除旧缓存
    // TODO: if (dbUpdater_) dbUpdater_(key, value) 更新 DB
    // TODO: redis_->set(key, value, std::chrono::seconds(ttlSec)) 回填缓存
}

// 练习2
AntiPenetrationCache::AntiPenetrationCache(DbQuery q)
    : redis_(std::make_shared<sw::redis::Redis>("tcp://127.0.0.1:6379")),
      dbQuery_(std::move(q)) {}

std::string AntiPenetrationCache::get(const std::string& key) {
    // TODO: auto val = redis_->get(key)
    // TODO: if (val && *val == NULL_SENTINEL) 命中哨兵，返回空
    // TODO: if (val) 正常命中，返回 *val
    // TODO: 未命中则查 DB
    // TODO: DB 有值则 redis_->set(key, value, 60s)
    // TODO: DB 无值则 redis_->set(key, NULL_SENTINEL, NULL_TTL_SEC)
    return "";
}

// 练习3
AntiBreakdownCache::AntiBreakdownCache(DbQuery q)
    : redis_(std::make_shared<sw::redis::Redis>("tcp://127.0.0.1:6379")),
      dbQuery_(std::move(q)) {}

std::string AntiBreakdownCache::get(const std::string& key) {
    // TODO: auto val = redis_->get(key)，命中则返回
    // TODO: 未命中则 lock_guard<std::mutex> 加锁
    // TODO: 加锁后再次查缓存（双重检查），命中则返回
    // TODO: 仍未命中则查 DB 并回填缓存
    return "";
}

// 练习4
DelayDoubleDelete::DelayDoubleDelete(DbQuery q)
    : redis_(std::make_shared<sw::redis::Redis>("tcp://127.0.0.1:6379")),
      dbQuery_(std::move(q)) {}

void DelayDoubleDelete::setDbUpdater(DbUpdater updater) {
    // TODO: dbUpdater_ = std::move(updater)
}

void DelayDoubleDelete::update(const std::string& key, const std::string& newValue) {
    // TODO: redis_->del(key) 第一次删除缓存
    // TODO: if (dbUpdater_) dbUpdater_(key, newValue) 更新 DB
    // TODO: std::this_thread::sleep_for(std::chrono::milliseconds(200)) 延迟
    // TODO: redis_->del(key) 第二次删除缓存
}

// ---------- main.cpp ----------
#include "CacheAside.h"
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <unordered_map>

int main() {
    // 练习1：Cache Aside
    // TODO: 用 unordered_map 模拟 DB，构造 DbQuery lambda
    // TODO: 构造 CacheAside，注入 DbQuery 和 DbUpdater
    // TODO: 测试 get（首次 MISS 查 DB，二次 HIT 缓存）
    // TODO: 测试 set（更新 DB 并回填缓存）

    // 练习2：缓存穿透防护
    // TODO: 构造 AntiPenetrationCache
    // TODO: 对不存在的 key 连续查询 3 次，观察第 1 次查 DB、后 2 次命中哨兵

    // 练习3：缓存击穿防护
    // TODO: 构造慢查询 DbQuery（sleep 200ms + atomic 计数）
    // TODO: 启动 5 个线程并发访问同一个 hot_key
    // TODO: join 后打印 dbCount，验证互斥锁减少了 DB 查询次数

    // 练习4：延迟双删
    // TODO: 用 unordered_map 模拟 DB，构造 DbQuery 和 DbUpdater
    // TODO: 调用 update(key, newValue)，观察三步流程
    // TODO: 验证 DB 中数据已更新为 newValue

    return 0;
}
```

</details>

<details>
<summary>🧪 测试步骤</summary>

需要 **1 个终端运行 Redis（Docker）** + **1 个终端运行程序**：

**前置：启动 Redis 服务器**

```bash
# 启动 Redis 容器，映射端口
# 如果已有容器，先 docker start some-redis
docker run --name some-redis -d -p 6379:6379 redis

# 验证 Redis 已启动
docker exec -it some-redis redis-cli ping
# 应返回: PONG
```

**编译运行**

```bash
cd code/stage07/7-4-cache
cmake --preset default
cmake --build build
./build/cacheExercises
```

**验证要点**：

| 练习 | 预期行为 | 验证方法 |
|------|----------|----------|
| **Cache Aside** | 第 1 次 `get(user:1)` MISS 查 DB，第 2 次 HIT 缓存；`set` 后 DB 和缓存都更新 | 观察输出中 `MISS` → `HIT` 的转换，以及 `set` 后再次 `get` 返回新值 |
| **穿透防护** | 第 1 次查 DB 返回空，缓存 `"NULL"` 哨兵；第 2、3 次命中哨兵直接返回空，不再查 DB | 只有第 1 次输出 `MISS -> query DB`，后 2 次输出 `HIT sentinel` |
| **击穿防护** | 5 个线程并发查同一个 key，互斥锁保证只有 1 个线程查 DB | `DB query count` 应为 1（或极少数），而非 5 |
| **延迟双删** | 依次输出 step1（删缓存）→ step2（更新 DB）→ step3（延迟再删） | 观察三步日志顺序，验证 `DB after update` 输出 `new_value` |

**关键观察**：
- **Cache Aside**：如果 Redis 未启动，所有 `get` 操作会抛异常——说明缓存层依赖 Redis 可用性，生产环境需要降级策略（缓存不可用时直接查 DB）。
- **穿透 vs 击穿**：穿透是"key 不存在"导致每次请求都穿透到 DB；击穿是"key 存在但过期"导致并发请求同时穿透。两者的解决方案不同——穿透用空值缓存/布隆过滤器，击穿用互斥锁。
- **延迟双删的 200ms 延迟**：这个值需要根据业务调整——太短可能删不干净（其他线程还在回填旧值），太长则影响写入性能。经验值是主从复制延迟的 2~3 倍。

</details>

---

## 7.5 HTTP 协议基础

- **练习目标**：
    - 理解 HTTP/1.1 的请求与响应格式：请求行、Header、Body。
    - 掌握常见请求方法（GET / POST / PUT / DELETE / HEAD / OPTIONS）与状态码（200 / 301 / 304 / 400 / 403 / 404 / 500）。
    - 理解 Header 语义：Content-Length / Transfer-Encoding: chunked / Connection: keep-alive / Host / User-Agent / Content-Type。
    - 理解 URL 编码、MIME 类型。
- **练习任务**：
    1. 用 `curl -v` 观察一次完整 HTTP 请求的请求行、Header、响应。
    2. 实现一个 HTTP 请求解析器：解析请求行、Header、Body（Content-Length 与 chunked 两种）。
    3. 实现一个 HTTP 响应构造器：支持状态码、Header、Body、chunked 编码。
    4. 实现 MIME 类型映射：根据文件后缀返回 Content-Type。
- **巩固标准**：
    - [ ] 能手绘 HTTP 请求与响应的报文结构。
      > **知识讲解**：HTTP 是**无状态的请求-响应协议**，基于 TCP 传输。请求报文由三部分组成：① **请求行**——`<Method> <URI> <Version>\r\n`，如 `GET /index.html HTTP/1.1\r\n`；② **请求头**——每行 `<Key>: <Value>\r\n`，如 `Host: example.com`、`Content-Length: 256`，Header 结束于一个空行 `\r\n`；③ **请求体**（可选）——POST/PUT 方法携带的数据，长度由 Content-Length 指定或通过 `Transfer-Encoding: chunked` 分块传输。响应报文同样三部分：① **状态行**——`<Version> <StatusCode> <ReasonPhrase>\r\n`，如 `HTTP/1.1 200 OK\r\n`；② **响应头**——与请求头格式相同；③ **响应体**——服务器返回的数据（HTML、JSON、图片等）。常见状态码分五类：`1xx`（信息）、`2xx`（成功，200 OK、204 No Content）、`3xx`（重定向，301 永久、302 临时、304 Not Modified 用于缓存）、`4xx`（客户端错误，400 Bad Request、403 Forbidden、404 Not Found）、`5xx`（服务端错误，500 Internal Server Error、502 Bad Gateway、503 Service Unavailable）。
    - [ ] 能解释 GET 与 POST 在语义、幂等性、缓存、请求体上的区别。
      > **知识讲解**：GET 和 POST 的核心区别在于**语义**而非技术实现。GET 用于**获取资源**，语义上是**幂等**的（多次请求结果相同、无副作用），因此可被浏览器缓存、可收藏书签、可被 CDN 缓存；POST 用于**提交数据**，语义上**非幂等**（每次提交可能产生不同效果，如创建订单），默认不被缓存。技术层面的区别：① GET 将参数编码在 URL 中（`?key=value`），受 URL 长度限制（浏览器通常 2KB~8KB），POST 将数据放在请求体中，理论上无大小限制；② GET 请求是幂等的，重复发送不会产生副作用，POST 则可能重复提交（如重复下单）；③ GET 只支持 URL 编码（`application/x-www-form-urlencoded`），POST 支持多种 Content-Type（`multipart/form-data` 上传文件、`application/json` 等）。**工程实践**：RESTful API 中 GET 用于查询、POST 用于创建、PUT 用于更新、DELETE 用于删除，严格遵循语义而非技术差异。
    - [ ] 能处理 chunked 编码的解码（读取 chunk size → 读取 chunk data → 直到 0 长度 chunk）。
      > **知识讲解**：chunked 编码是 HTTP/1.1 引入的**分块传输机制**，用于服务器事先不知道响应体总大小的场景（如动态生成内容、流式输出）。格式为：每个 chunk 由**一行十六进制的 chunk size**（不含 `\r\n` 长度本身）+ `\r\n` + **chunk data**（恰好 size 字节）+ `\r\n` 组成；最后一个 chunk 的 size 为 `0`，表示传输结束，之后可能跟随 trailer headers（可选）。解码流程：① 读一行直到 `\r\n`，解析为十六进制整数 size；② 若 size == 0，解码结束；③ 读取 size 字节的数据；④ 读取 2 字节 `\r\n`（chunk 尾部分隔符）；⑤ 重复①。与 Content-Length 的对比：Content-Length 需要服务器提前计算总大小，适合静态文件；chunked 不需要预知大小，适合动态流式响应，但增加了 chunk 头尾的解析开销。**工程实践**：HTTP/2 已废弃 chunked 编码，改用帧（frame）机制传输数据，但 HTTP/1.1 场景仍需支持。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 7.5 HTTP 协议基础 练习框架 ===
// 项目结构:
// 7-5-http-proto/
// ├── HttpRequest.h
// ├── HttpRequest.cpp
// ├── HttpResponse.h
// ├── HttpResponse.cpp
// └── main.cpp

// ---------- HttpRequest.h ----------
#pragma once
#include <string>
#include <unordered_map>

enum class ParseState { RequestLine, Headers, Body, Complete, Error };

class HttpRequest {
public:
    const std::string& method() const { return method_; }
    const std::string& uri() const { return uri_; }
    const std::string& version() const { return version_; }
    const std::string& header(const std::string& key) const;
    const std::string& body() const { return body_; }
    ParseState state() const { return state_; }

    bool parseRequestLine(const std::string& line);
    bool parseHeaderLine(const std::string& line);
    void setBody(const std::string& b) { body_ = b; }
    void setState(ParseState s) { state_ = s; }

    // 练习4: URL 解码
    static std::string urlDecode(const std::string& s);

private:
    std::string method_, uri_, version_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
    ParseState state_{ParseState::RequestLine};
};

// ---------- HttpRequest.cpp ----------
#include "HttpRequest.h"

bool HttpRequest::parseRequestLine(const std::string& line) {
    // TODO: 解析 "GET /path HTTP/1.1"
    // TODO: 提取 method / uri / version
    return false;
}

bool HttpRequest::parseHeaderLine(const std::string& line) {
    // TODO: 解析 "Host: example.com"
    // TODO: 按 ':' 分割，存入 headers_
    return false;
}

std::string HttpRequest::urlDecode(const std::string& s) {
    // TODO: 遍历 s，遇到 %xx 转义为对应字符，+ 转为空格
    return "";
}

// ---------- HttpResponse.h ----------
#pragma once
#include <string>
#include <unordered_map>

class HttpResponse {
public:
    explicit HttpResponse(int statusCode = 200);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    void setContentType(const std::string& mime);
    std::string serialize() const;

    // 练习4: MIME 类型映射
    static std::string mimeFromExt(const std::string& ext);

private:
    int statusCode_;
    std::string reason_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
};

// ---------- HttpResponse.cpp ----------
#include "HttpResponse.h"

HttpResponse::HttpResponse(int code) : statusCode_(code) {
    // TODO: 根据 code 设置 reason_
}

void HttpResponse::setHeader(const std::string& k, const std::string& v) { headers_[k] = v; }

void HttpResponse::setBody(const std::string& body) {
    body_ = body;
    setHeader("Content-Length", std::to_string(body.size()));
}

void HttpResponse::setContentType(const std::string& mime) { setHeader("Content-Type", mime); }

std::string HttpResponse::serialize() const {
    // TODO: "HTTP/1.1 <code> <reason>\r\n" + headers + "\r\n" + body
    return "";
}

std::string HttpResponse::mimeFromExt(const std::string& ext) {
    // TODO: html -> text/html, css -> text/css, js -> application/javascript, png -> image/png
    return "application/octet-stream";
}

// ---------- main.cpp ----------
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <iostream>

int main() {
    HttpRequest req;
    req.parseRequestLine("GET /index.html HTTP/1.1");
    req.parseHeaderLine("Host: localhost");
    // TODO: 测试 URL 解码、MIME 映射、响应序列化
    return 0;
}
```

</details>

<details>
<summary>🧪 测试步骤</summary>

**验证要点**：

| 练习 | 预期行为 | 验证方法 |
|------|----------|----------|
| **请求解析** | 解析 `GET /index.html HTTP/1.1` 后，method = `GET`、uri = `/index.html`、version = `HTTP/1.1` | 打印各字段确认 |
| **Header 解析** | 解析 `Host: localhost` 后，`header("Host")` 返回 `localhost` | 打印 header 值确认 |
| **URL 解码** | `%E4%BD%A0%E5%A5%BD` → `你好`，`hello+world` → `hello world` | 打印解码结果确认 |
| **MIME 映射** | `html` → `text/html`，`png` → `image/png`，未知后缀 → `application/octet-stream` | 打印各后缀的 MIME 确认 |
| **响应序列化** | 200 + `Content-Type: text/html` + body `<h1>OK</h1>` → 完整响应报文 | 打印序列化结果，对比标准 HTTP 响应格式 |

**关键观察**：
- **请求行解析**：按空格分割即可，但要注意 URI 可能包含查询参数（`/search?q=hello`），需要单独处理 `?` 后的 query string。
- **Header 解析**：按第一个 `:` 分割（Value 中可能包含 `:`，如 `Date: Mon, 06 Oct 2026 00:00:00 GMT`），不能简单用 `split(':')`。
- **chunked 解码**：每个 chunk 的 size 是十六进制字符串，需要用 `std::stoul(sizeStr, nullptr, 16)` 解析，而非十进制。

</details>

---

# 阶段八：综合项目 —— 高性能 HTTP 服务器

> **定位**：整条学习路线的**重心**。把前面所有知识点（STL / 现代 C++ / OOP / Linux 系统调用 / 网络进阶 / 数据库）串联成一个完整的项目。
>
> **项目目标**：实现一个可实际使用的 HTTP/1.1 服务器，支持静态文件、简单 CGI、长连接、多线程 Reactor 架构，能跑 WebBench 压测并达到可观的 QPS。
>
> **可选扩展**：在完成基础 HTTP 服务器后，可任选一个方向继续深化：
> - **方向 A**：升级为 WebSocket 服务器，实现一个实时聊天室。
> - **方向 B**：加入 MySQL + Redis，实现一个"短链接服务"或"用户系统"。
> - **方向 C**：实现 HTTP 反向代理 / 负载均衡器。

## 8.1 项目架构设计

- **练习目标**：
    - 设计一个清晰的项目目录结构（include / src / tests / conf / www / CMakeLists.txt）。
    - 设计核心模块划分：EventLoop、Channel、Acceptor、TcpConnection、HttpRequest/Response、HttpServer、ThreadPool、Timer、Logger。
    - 设计类之间的协作关系（UML 或文字描述）。
    - 制定编码规范（命名、注释、错误处理、日志分级）。
- **练习任务**：
    1. 画出项目的模块依赖图：哪些模块是核心 IO 层、哪些是协议层、哪些是业务层。
    2. 设计 EventLoop 类：封装 epoll fd、事件分发、pending 任务队列、定时器。
    3. 设计 Channel 类：封装 fd + 关注事件 + 回调（读/写/错误/关闭）。
    4. 设计 TcpConnection 类：封装连接状态、输入输出缓冲区、HTTP 解析状态机。
    5. 设计 HttpServer 类：组装所有模块，提供 start / stop 接口。
- **巩固标准**：
    - [ ] 能画出"主线程 accept → sub-reactor 读请求 → 线程池处理 → 响应写回"的完整流程。
    - [ ] 能解释为什么每个连接要绑定到一个 EventLoop（避免跨线程操作 fd 的竞态）。
    - [ ] 能说出"one loop per thread"的设计哲学（muduo 网络库的核心思想）。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 8.1 项目架构设计 练习框架 ===
// 项目结构（推荐）:
// http-server/
// ├── CMakeLists.txt
// ├── include/
// │   ├── EventLoop.h
// │   ├── Channel.h
// │   ├── Acceptor.h
// │   ├── TcpConnection.h
// │   ├── HttpServer.h
// │   ├── HttpRequest.h
// │   ├── HttpResponse.h
// │   ├── ThreadPool.h
// │   ├── TimerQueue.h
// │   └── Logger.h
// ├── src/
// │   ├── EventLoop.cpp
// │   ├── Channel.cpp
// │   ├── Acceptor.cpp
// │   ├── TcpConnection.cpp
// │   ├── HttpServer.cpp
// │   ├── HttpRequest.cpp
// │   ├── HttpResponse.cpp
// │   ├── ThreadPool.cpp
// │   ├── TimerQueue.cpp
// │   ├── Logger.cpp
// │   └── main.cpp
// ├── conf/
// │   └── server.conf
// ├── tests/
// │   └── test_http_parser.cpp
// └── www/
//     ├── index.html
//     └── 404.html

// ---------- include/EventLoop.h ----------
#pragma once
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void updateChannel(Channel* ch);
    void removeChannel(Channel* ch);
    void runInLoop(std::function<void()> fn);
    void queueInLoop(std::function<void()> fn);

private:
    void wakeup();
    void handlePendingTasks();
    int epollFd_;
    int wakeupFd_;  // eventfd
    std::atomic<bool> quit_{false};
    std::mutex mtx_;
    std::queue<std::function<void()>> pendingTasks_;
};

// ---------- include/Channel.h ----------
#pragma once
#include <functional>

class EventLoop;

class Channel {
public:
    using EventCallback = std::function<void()>;
    Channel(EventLoop* loop, int fd);
    void setReadCallback(EventCallback cb)   { readCb_ = std::move(cb); }
    void setWriteCallback(EventCallback cb)  { writeCb_ = std::move(cb); }
    void setCloseCallback(EventCallback cb)  { closeCb_ = std::move(cb); }
    void setErrorCallback(EventCallback cb)  { errorCb_ = std::move(cb); }
    void enableReading(bool on = true);
    void enableWriting(bool on = true);
    void handleEvent(int revents);
    int fd() const { return fd_; }
private:
    EventLoop* loop_;
    int fd_;
    EventCallback readCb_, writeCb_, closeCb_, errorCb_;
};

// ---------- include/Acceptor.h ----------
#pragma once
#include <functional>

class EventLoop;
class Channel;

class Acceptor {
public:
    using NewConnCallback = std::function<void(int fd)>;
    Acceptor(EventLoop* loop, int port);
    void setNewConnCallback(NewConnCallback cb) { newConnCb_ = std::move(cb); }
    void listen();
private:
    void handleRead();
    EventLoop* loop_;
    Channel* acceptChannel_;
    int listenFd_;
    NewConnCallback newConnCb_;
};

// ---------- include/TcpConnection.h ----------
#pragma once
#include <memory>
#include <string>

class EventLoop;
class Channel;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(EventLoop* loop, int fd);
    ~TcpConnection();
    void send(const std::string& data);
    void shutdown();
    void setConnectionCallback(std::function<void(const std::shared_ptr<TcpConnection>&)> cb);
    void setMessageCallback(std::function<void(const std::shared_ptr<TcpConnection>&, const std::string&)> cb);
    void setCloseCallback(std::function<void(const std::shared_ptr<TcpConnection>&)> cb);

private:
    void handleRead();
    void handleWrite();
    void handleClose();
    EventLoop* loop_;
    Channel* channel_;
    int fd_;
    std::string inputBuffer_;
    std::string outputBuffer_;
};

// ---------- include/HttpServer.h ----------
#pragma once
#include <memory>
#include <vector>

class EventLoop;
class Acceptor;

class HttpServer {
public:
    HttpServer(int port, int subReactorCount = 3, int threadPoolSize = 4);
    void start();
    void setDocumentRoot(const std::string& root) { docRoot_ = root; }

private:
    void onNewConnection(int fd);
    void onRequest(const std::shared_ptr<TcpConnection>& conn, const std::string& msg);

    int port_;
    std::string docRoot_{"./www"};
    EventLoop mainLoop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::vector<std::unique_ptr<EventLoop>> subLoops_;
    int nextSub_{0};
};

// ---------- src/main.cpp ----------
#include "HttpServer.h"

int main() {
    HttpServer server(8080, 3, 4);
    server.setDocumentRoot("./www");
    server.start();
    return 0;
}
```

</details>

## 8.2 核心模块实现（IO 层）

- **练习目标**：
    - 实现 EventLoop：epoll_wait 循环、事件分发、 wakeup（eventfd / pipe）、pending 任务执行。
    - 实现 Channel：事件回调注册、revents 处理、状态机（读就绪 / 写就绪 / 错误 / 挂起）。
    - 实现 Acceptor：bind + listen + accept，新连接回调给 TcpServer。
    - 实现 TcpConnection：非阻塞 IO、读写缓冲区、连接生命周期管理。
- **练习任务**：
    1. 实现 EventLoop 的最小版本：epoll_wait + Channel 分发。
    2. 给 EventLoop 加上 `runInLoop()` / `queueInLoop()`，支持跨线程投递任务（eventfd 唤醒）。
    3. 实现 Acceptor：监听新连接，回调给上层。
    4. 实现 TcpConnection：读事件 → 读入缓冲区 → 回调；写事件 → 从写缓冲区发送。
    5. 实现非阻塞 connect + 错误处理（EAGAIN / EINTR / EPIPE / ECONNRESET）。
- **巩固标准**：
    - [ ] 能解释 eventfd 相比 pipe 唤醒的优势（一个 fd、语义更清晰）。
    - [ ] 能处理"写缓冲区满"的情况：注册 EPOLLOUT，写完再取消。
    - [ ] 能正确管理 TcpConnection 的生命周期（`std::shared_ptr` + `std::enable_shared_from_this`）。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 8.2 核心模块实现（IO 层） 练习框架 ===
// 项目结构:
// http-server/src/
// ├── EventLoop.cpp
// ├── Channel.cpp
// ├── Acceptor.cpp
// └── TcpConnection.cpp

// ---------- src/EventLoop.cpp ----------
#include "EventLoop.h"
#include "Channel.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

EventLoop::EventLoop() {
    // TODO: epollFd_ = epoll_create1(EPOLL_CLOEXEC)
    // TODO: wakeupFd_ = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)
    // TODO: 把 wakeupFd_ 加入 epoll 监听 EPOLLIN
}

EventLoop::~EventLoop() {
    // TODO: close(epollFd_); close(wakeupFd_)
}

void EventLoop::loop() {
    // TODO: while (!quit_) {
    //   epoll_wait(epollFd_, events, max, timeout)
    //   分发事件给 Channel::handleEvent
    //   handlePendingTasks()
    // }
}

void EventLoop::quit() {
    // TODO: quit_ = true; wakeup()
}

void EventLoop::updateChannel(Channel* ch) {
    // TODO: epoll_ctl ADD 或 MOD
}

void EventLoop::removeChannel(Channel* ch) {
    // TODO: epoll_ctl DEL
}

void EventLoop::runInLoop(std::function<void()> fn) {
    // TODO: 若当前线程就是 EventLoop 线程，直接执行；否则 queueInLoop + wakeup
}

void EventLoop::queueInLoop(std::function<void()> fn) {
    // TODO: 加锁 push 到 pendingTasks_，wakeup
}

void EventLoop::wakeup() {
    // TODO: uint64_t one = 1; write(wakeupFd_, &one, sizeof(one))
}

void EventLoop::handlePendingTasks() {
    // TODO: 加锁 swap 出所有任务，逐个执行
}

// ---------- src/Channel.cpp ----------
#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>

Channel::Channel(EventLoop* loop, int fd) : loop_(loop), fd_(fd) {}

void Channel::enableReading(bool on) {
    // TODO: 设置 events_ |= EPOLLIN 或清除
    // TODO: loop_->updateChannel(this)
}

void Channel::enableWriting(bool on) {
    // TODO: events_ |= EPOLLOUT 或清除
    // TODO: loop_->updateChannel(this)
}

void Channel::handleEvent(int revents) {
    // TODO: if (revents & (EPOLLRDHUP | EPOLLHUP)) closeCb_()
    // TODO: if (revents & EPOLLERR) errorCb_()
    // TODO: if (revents & EPOLLIN) readCb_()
    // TODO: if (revents & EPOLLOUT) writeCb_()
}

// ---------- src/Acceptor.cpp ----------
#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

Acceptor::Acceptor(EventLoop* loop, int port) : loop_(loop) {
    // TODO: listenFd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)
    // TODO: setsockopt(SO_REUSEADDR)
    // TODO: bind + listen
    // TODO: acceptChannel_ = new Channel(loop_, listenFd_)
    // TODO: acceptChannel_->setReadCallback(bind(&Acceptor::handleRead, this))
}

void Acceptor::listen() {
    // TODO: acceptChannel_->enableReading()
}

void Acceptor::handleRead() {
    // TODO: while (true) {
    //   int connfd = accept4(listenFd_, ..., SOCK_NONBLOCK)
    //   if (connfd < 0) break
    //   newConnCb_(connfd)
    // }
}

// ---------- src/TcpConnection.cpp ----------
#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>

TcpConnection::TcpConnection(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd) {
    // TODO: channel_ = new Channel(loop, fd)
    // TODO: channel_->setReadCallback(bind(&TcpConnection::handleRead, this))
    // TODO: channel_->setWriteCallback(bind(&TcpConnection::handleWrite, this))
    // TODO: channel_->setCloseCallback(bind(&TcpConnection::handleClose, this))
    // TODO: channel_->enableReading()
}

TcpConnection::~TcpConnection() {
    // TODO: close(fd_); delete channel_
}

void TcpConnection::handleRead() {
    // TODO: char buf[65536]; ssize_t n = read(fd_, buf, sizeof(buf))
    // TODO: if (n > 0) { inputBuffer_.append(buf, n); messageCb_(shared_from_this(), inputBuffer_); }
    // TODO: else if (n == 0) handleClose()
    // TODO: else if (errno != EAGAIN) handleClose()
}

void TcpConnection::handleWrite() {
    // TODO: ssize_t n = write(fd_, outputBuffer_.data(), outputBuffer_.size())
    // TODO: if (n > 0) { outputBuffer_.erase(0, n); if (empty) enableWriting(false); }
}

void TcpConnection::handleClose() {
    // TODO: closeCb_(shared_from_this())
    // TODO: loop_->removeChannel(channel_)
    // TODO: close(fd_)
}

void TcpConnection::send(const std::string& data) {
    // TODO: 若当前未写，直接 write；否则 append 到 outputBuffer_ 并 enableWriting
}

void TcpConnection::shutdown() {
    // TODO: ::shutdown(fd_, SHUT_WR)
}
```

</details>

## 8.3 HTTP 协议层实现

- **练习目标**：
    - 实现 HTTP 请求解析的状态机（解析请求行 → Header → Body）。
    - 实现 HTTP 响应的构造（状态行、Header、Body）。
    - 实现静态文件处理：读取文件、返回 200 / 404 / 403，支持 Range 请求（可选）。
    - 实现 HEAD 方法、OPTIONS 方法。
- **练习任务**：
    1. 实现 HttpRequest 类：解析状态机（Uninitialized / ParsingRequestLine / ParsingHeaders / ParsingBody / Complete）。
    2. 实现 HttpResponse 类：设置状态码、Header、Body，序列化为字节流。
    3. 实现静态文件处理器：根据 URI 找到 www 目录下的文件，返回内容 + MIME。
    4. 实现 404 / 400 / 500 等错误页面。
    5. 处理 POST 请求：解析 Body，返回 echo 响应。
- **巩固标准**：
    - [ ] 能处理"请求行过长"、"Header 过多"等异常情况，返回 400。
    - [ ] 能实现 HTTP 解析的"零拷贝"优化（直接在缓冲区上解析，不额外拷贝）。
    - [ ] 能处理 URL 解码（%xx 转义、+ 转空格）。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 8.3 HTTP 协议层实现 练习框架 ===
// 项目结构:
// http-server/src/
// ├── HttpHandler.h
// └── HttpHandler.cpp

// ---------- src/HttpHandler.h ----------
#pragma once
#include <memory>
#include <string>

class TcpConnection;
class HttpRequest;
class HttpResponse;

class HttpHandler {
public:
    void onRequest(const std::shared_ptr<TcpConnection>& conn, const std::string& input);

private:
    bool parseRequest(const std::string& input, HttpRequest& req);
    void buildResponse(const HttpRequest& req, HttpResponse& resp);
    void serveStaticFile(const std::string& uri, const std::string& docRoot, HttpResponse& resp);
    void handleError(const std::shared_ptr<TcpConnection>& conn, int code, const std::string& reason);
};

// ---------- src/HttpHandler.cpp ----------
#include "HttpHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "TcpConnection.h"
#include <fstream>
#include <sstream>

void HttpHandler::onRequest(const std::shared_ptr<TcpConnection>& conn, const std::string& input) {
    HttpRequest req;
    HttpResponse resp;
    // TODO: if (!parseRequest(input, req)) { handleError(conn, 400, "Bad Request"); return; }
    // TODO: buildResponse(req, resp)
    // TODO: conn->send(resp.serialize())
}

bool HttpHandler::parseRequest(const std::string& input, HttpRequest& req) {
    // TODO: 实现状态机解析：请求行 → Header → Body
    // TODO: 处理请求行过长、Header 过多等异常
    return false;
}

void HttpHandler::buildResponse(const HttpRequest& req, HttpResponse& resp) {
    const std::string& method = req.method();
    const std::string& uri = req.uri();
    // TODO: if (method == "GET" || method == "HEAD") serveStaticFile(uri, "./www", resp)
    // TODO: else if (method == "POST") { 解析 body，返回 echo }
    // TODO: else if (method == "OPTIONS") { 返回 Allow 头 }
    // TODO: else resp = HttpResponse(405)
}

void HttpHandler::serveStaticFile(const std::string& uri, const std::string& docRoot, HttpResponse& resp) {
    // TODO: 拼接路径 path = docRoot + HttpRequest::urlDecode(uri)
    // TODO: 检查路径合法性（不允许 ../）
    // TODO: std::ifstream 读取文件内容
    // TODO: 设置 Content-Type（根据后缀）、Content-Length
    // TODO: 文件不存在 resp = HttpResponse(404) 并返回 404.html
}

void HttpHandler::handleError(const std::shared_ptr<TcpConnection>& conn, int code, const std::string& reason) {
    // TODO: 构造错误响应并发送
}
```

</details>

## 8.4 多线程 Reactor 与定时器

- **练习目标**：
    - 实现主从 Reactor 模型：main-reactor 只负责 accept，sub-reactor 负责 IO。
    - 实现线程池：任务队列 + 条件变量 + worker 线程。
    - 实现定时器：最小堆 + EventLoop 定时触发（处理超时连接、心跳）。
- **练习任务**：
    1. 实现 ThreadPool 类：`addTask()` 投递任务、worker 线程循环取任务执行。
    2. 实现 TimerQueue：最小堆存 (expire_time, callback)，每次 epoll_wait 后计算下一次超时。
    3. 把"连接超时检测"接入 TcpConnection：每次活跃更新 expire_time，定时扫描关闭超时连接。
    4. 把"业务处理"交给线程池：TcpConnection 读完请求后投递给线程池，处理完再写回响应。
    5. 实现优雅退出：捕获 SIGTERM，停止 accept，等待所有连接空闲或超时。
- **巩固标准**：
    - [ ] 能画出"main-reactor → sub-reactor → 线程池"的完整任务流转图。
    - [ ] 能解释为什么"定时器回调"必须在 EventLoop 线程执行（避免竞态）。
    - [ ] 能处理"任务执行过程中连接已关闭"的情况（弱引用 / 生命周期检查）。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 8.4 多线程 Reactor 与定时器 练习框架 ===
// 项目结构:
// http-server/src/
// ├── ThreadPool.h
// ├── ThreadPool.cpp
// ├── TimerQueue.h
// ├── TimerQueue.cpp
// └── HttpServer.cpp

// ---------- src/ThreadPool.h ----------
#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(int size = 4);
    ~ThreadPool();
    void addTask(std::function<void()> task);

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_{false};
};

// ---------- src/ThreadPool.cpp ----------
#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) {
    // TODO: 启动 size 个 worker 线程
    // TODO: worker 循环：加锁 → cv_.wait → 取任务 → 执行
}

ThreadPool::~ThreadPool() {
    // TODO: stop_ = true; cv_.notify_all()
    // TODO: 遍历 join 所有 worker
}

void ThreadPool::addTask(std::function<void()> task) {
    // TODO: 加锁 push 到 tasks_; cv_.notify_one()
}

// ---------- src/TimerQueue.h ----------
#pragma once
#include <cstdint>
#include <functional>
#include <queue>
#include <vector>

struct TimerNode {
    int64_t expireMs;
    std::function<void()> cb;
    bool operator>(const TimerNode& o) const { return expireMs > o.expireMs; }
};

class TimerQueue {
public:
    void addTimer(int64_t expireMs, std::function<void()> cb);
    void processExpiredTimers();  // 在 EventLoop 线程调用
    int64_t nextExpireMs() const;

private:
    std::priority_queue<TimerNode, std::vector<TimerNode>, std::greater<TimerNode>> heap_;
};

// ---------- src/TimerQueue.cpp ----------
#include "TimerQueue.h"
#include <chrono>

static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void TimerQueue::addTimer(int64_t expireMs, std::function<void()> cb) {
    // TODO: heap_.push({expireMs, std::move(cb)})
}

void TimerQueue::processExpiredTimers() {
    // TODO: while (!heap_.empty() && heap_.top().expireMs <= nowMs()) {
    //   取出并执行回调
    // }
}

int64_t TimerQueue::nextExpireMs() const {
    // TODO: return heap_.empty() ? -1 : heap_.top().expireMs
    return -1;
}

// ---------- src/HttpServer.cpp ----------
#include "HttpServer.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "ThreadPool.h"
#include "TimerQueue.h"

HttpServer::HttpServer(int port, int subCount, int poolSize) : port_(port) {
    // TODO: 创建 subCount 个 sub EventLoop，每个在独立线程 loop()
    // TODO: acceptor_ = std::make_unique<Acceptor>(&mainLoop_, port)
    // TODO: acceptor_->setNewConnCallback(bind(&HttpServer::onNewConnection, this, ...))
    // TODO: threadPool_ = std::make_unique<ThreadPool>(poolSize)
    // TODO: timerQueue_ = std::make_unique<TimerQueue>()
}

void HttpServer::start() {
    // TODO: acceptor_->listen()
    // TODO: mainLoop_.loop()
}

void HttpServer::onNewConnection(int fd) {
    // TODO: 轮询选择一个 subLoop
    // TODO: subLoop->runInLoop 创建 TcpConnection，设置 messageCallback 到 onRequest
}

void HttpServer::onRequest(const std::shared_ptr<TcpConnection>& conn, const std::string& msg) {
    // TODO: 把业务处理投递到 threadPool_
    // TODO: 处理完后 conn->send(resp.serialize())
}
```

</details>

## 8.5 日志、配置与测试

- **练习目标**：
    - 实现一个简易日志类：分级（DEBUG / INFO / WARN / ERROR）、带时间戳与线程 ID、异步写入。
    - 实现配置文件加载：key=value 格式，支持日志级别、端口、线程数、根目录等配置。
    - 编写单元测试：HTTP 解析器、响应构造器、定时器。
- **练习任务**：
    1. 实现 Logger 类：支持 `LOG_DEBUG` / `LOG_INFO` 等宏，输出到文件或 stdout。
    2. 实现异步日志：日志先入队列，后台线程批量写盘（可选，作为进阶）。
    3. 实现 Config 类：加载 `server.conf`，提供 `getInt()` / `getString()` 接口。
    4. 写单元测试：构造各种畸形 HTTP 请求，验证解析器鲁棒性。
    5. 写集成测试：启动服务器，用 curl 验证各种请求。
- **巩固标准**：
    - [ ] 能解释为什么日志要带时间戳、线程 ID、文件名行号。
    - [ ] 能处理"日志文件过大"的问题（轮转或接入 logrotate）。
    - [ ] 能说出"单元测试"与"集成测试"的区别与在本项目中的体现。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 8.5 日志、配置与测试 练习框架 ===
// 项目结构:
// http-server/src/
// ├── Logger.h
// ├── Logger.cpp
// ├── Config.h
// ├── Config.cpp
// └── tests/
//     └── test_http_parser.cpp

// ---------- src/Logger.h ----------
#pragma once
#include <fstream>
#include <mutex>
#include <string>

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {
public:
    static Logger& instance();
    void setLevel(LogLevel lv) { level_ = lv; }
    void setOutput(const std::string& path);
    void log(LogLevel lv, const char* file, int line, const char* fmt, ...);

private:
    Logger() = default;
    LogLevel level_{LogLevel::INFO};
    std::mutex mtx_;
    std::ofstream ofs_;
};

#define LOG_DEBUG(fmt, ...) Logger::instance().log(LogLevel::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  Logger::instance().log(LogLevel::INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  Logger::instance().log(LogLevel::WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::instance().log(LogLevel::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

// ---------- src/Logger.cpp ----------
#include "Logger.h"
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <iostream>

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

void Logger::setOutput(const std::string& path) {
    std::lock_guard<std::mutex> lk(mtx_);
    ofs_.open(path, std::ios::app);
}

void Logger::log(LogLevel lv, const char* file, int line, const char* fmt, ...) {
    if (lv < level_) return;
    // TODO: 获取当前时间戳（精确到毫秒）
    // TODO: 获取当前线程 ID
    // TODO: 格式化 "[时间] [级别] [线程ID] [文件:行号] 消息\n"
    // TODO: 加锁输出到 ofs_ 或 stdout
}

// ---------- src/Config.h ----------
#pragma once
#include <string>
#include <unordered_map>

class Config {
public:
    bool load(const std::string& path);
    int getInt(const std::string& key, int def = 0) const;
    std::string getString(const std::string& key, const std::string& def = "") const;

private:
    std::unordered_map<std::string, std::string> data_;
};

// ---------- src/Config.cpp ----------
#include "Config.h"
#include <fstream>
#include <sstream>

bool Config::load(const std::string& path) {
    // TODO: std::ifstream 读取每行
    // TODO: 跳过 # 注释行
    // TODO: 按 '=' 分割 key=value 存入 data_
    return false;
}

int Config::getInt(const std::string& key, int def) const {
    auto it = data_.find(key);
    return it == data_.end() ? def : std::stoi(it->second);
}

std::string Config::getString(const std::string& key, const std::string& def) const {
    auto it = data_.find(key);
    return it == data_.end() ? def : it->second;
}

// ---------- tests/test_http_parser.cpp ----------
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <cassert>
#include <iostream>

void testParseRequestLine() {
    HttpRequest req;
    assert(req.parseRequestLine("GET /index.html HTTP/1.1"));
    assert(req.method() == "GET");
    assert(req.uri() == "/index.html");
}

void testUrlDecode() {
    assert(HttpRequest::urlDecode("hello%20world") == "hello world");
    assert(HttpRequest::urlDecode("a+b") == "a b");
}

void testMime() {
    assert(HttpResponse::mimeFromExt("html") == "text/html");
    assert(HttpResponse::mimeFromExt("png") == "image/png");
}

int main() {
    testParseRequestLine();
    testUrlDecode();
    testMime();
    std::cout << "all tests passed\n";
    return 0;
}
```

</details>

## 8.6 压测与优化

- **练习目标**：
    - 掌握 WebBench / wrk / ab 等压测工具的使用。
    - 能分析压测结果：QPS、延迟分布、错误率。
    - 能定位性能瓶颈：CPU（热点函数）、IO（系统调用次数）、锁竞争。
- **练习任务**：
    1. 用 WebBench 压测静态文件接口，记录 QPS。
    2. 用 `perf record` + `perf report` 找出 CPU 热点函数。
    3. 优化方向（任选）：
        - 引入 `sendfile()` 零拷贝发送静态文件。
        - 引入 `mmap` + `write` 发送静态文件。
        - 调整读写缓冲区大小。
        - 优化锁粒度（减少锁持有时间）。
    4. 对比优化前后的 QPS 与 CPU 占用。
    5. 用 `strace -c` 统计系统调用次数，找出可优化点。
- **巩固标准**：
    - [ ] 能解释 `sendfile()` 相比 `read + write` 减少的两次数据拷贝。
    - [ ] 能解释为什么"缓冲区不是越大越好"（内存占用、缓存命中率）。
    - [ ] 能画出"压测 → 分析 → 优化 → 再压测"的迭代流程。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 8.6 压测与优化 练习框架 ===
// 项目结构:
// http-server/
// ├── bench/
// │   ├── bench_client.cpp  // 压测客户端
// │   └── run_bench.sh      // 压测脚本
// └── src/
//     └── StaticFileSender.cpp  // 练习3: sendfile 优化

// ---------- bench/bench_client.cpp ----------
// 练习1: 用 WebBench / wrk / ab 压测
// 命令示例:
//   webbench -c 100 -t 10 http://127.0.0.1:8080/index.html
//   wrk -t4 -c100 -d10s http://127.0.0.1:8080/index.html
//   ab -n 10000 -c 100 http://127.0.0.1:8080/index.html

// ---------- bench/run_bench.sh ----------
// #!/bin/bash
// echo "=== WebBench ==="
// webbench -c 100 -t 10 http://127.0.0.1:8080/index.html
// echo "=== perf record ==="
// perf record -p $(pgrep http-server) -g -- sleep 10
// perf report
// echo "=== strace -c ==="
// strace -c -p $(pgrep http-server) sleep 5

// ---------- src/StaticFileSender.cpp ----------
#pragma once
#include <string>

class TcpConnection;

class StaticFileSender {
public:
    // 练习3: 优化发送静态文件
    static void sendWithReadWrite(const std::shared_ptr<TcpConnection>& conn, const std::string& path);
    static void sendWithSendfile(const std::shared_ptr<TcpConnection>& conn, const std::string& path);
    static void sendWithMmap(const std::shared_ptr<TcpConnection>& conn, const std::string& path);
};

// ---------- src/StaticFileSender.cpp ----------
#include "StaticFileSender.h"
#include "TcpConnection.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

void StaticFileSender::sendWithReadWrite(const std::shared_ptr<TcpConnection>& conn, const std::string& path) {
    // TODO: open + fstat + read 到 buf + conn->send(buf) + close
}

void StaticFileSender::sendWithSendfile(const std::shared_ptr<TcpConnection>& conn, const std::string& path) {
    // TODO: int in = open(path, O_RDONLY)
    // TODO: struct stat st; fstat(in, &st)
    // TODO: sendfile(conn->fd(), in, nullptr, st.st_size)  // 零拷贝
    // TODO: close(in)
}

void StaticFileSender::sendWithMmap(const std::shared_ptr<TcpConnection>& conn, const std::string& path) {
    // TODO: int fd = open(path, O_RDONLY)
    // TODO: struct stat st; fstat(fd, &st)
    // TODO: void* p = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)
    // TODO: conn->send(string((char*)p, st.st_size))
    // TODO: munmap + close
}
```

</details>

## 8.7 项目总结与文档

- **练习目标**：
    - 整理项目代码，写 README（项目介绍、特性、编译、运行、压测结果、架构图）。
    - 总结项目中的核心知识点与踩坑记录。
    - 把项目上传 GitHub，作为简历上的亮点。
- **练习任务**：
    1. 写 README.md：项目简介、特性列表、架构图、编译运行说明、压测结果、TODO。
    2. 整理代码：删除调试代码、统一命名风格、补全注释。
    3. 写一份"踩坑记录"：至少 5 条（如 epoll ET 模式踩坑、连接生命周期踩坑等）。
    4. 上传 GitHub，配好 .gitignore、LICENSE。
- **巩固标准**：
    - [ ] README 能让陌生人照着编译运行成功。
    - [ ] 能在 5 分钟内向面试官讲清楚项目架构与亮点。
    - [ ] 能对比自己的实现与 muduo / TinyWebServer 等开源项目的异同。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 8.7 项目总结与文档 练习框架 ===
// 项目结构:
// http-server/
// ├── README.md
// ├── docs/
// │   ├── pitfalls.md   // 踩坑记录
// │   └── architecture.png  // 架构图
// ├── .gitignore
// └── LICENSE

// ---------- README.md 模板 ----------
// # 高性能 HTTP 服务器
//
// ## 项目简介
// 一个基于多线程 Reactor 模型的高性能 HTTP/1.1 服务器，支持静态文件、长连接、优雅退出。
//
// ## 特性
// - 主从 Reactor + 线程池架构
// - 支持 HTTP/1.1 长连接（Keep-Alive）
// - 支持 GET / HEAD / POST / OPTIONS
// - 静态文件发送（sendfile 零拷贝）
// - 最小堆定时器处理超时连接
// - 异步日志
// - 优雅退出
//
// ## 架构图
// ![架构](docs/architecture.png)
//
// ## 编译运行
// ```bash
// mkdir build && cd build
// cmake ..
// make -j
// ./http-server
// ```
//
// ## 压测结果
// | 并发 | QPS | 平均延迟 | P99 |
// |---|---|---|---|
// | 100 | XXXX | Xms | Xms |
//
// ## TODO
// - [ ] WebSocket 支持
// - [ ] HTTPS
// - [ ] CGI

// ---------- docs/pitfalls.md 模板 ----------
// # 踩坑记录
//
// 1. **epoll ET 模式漏读数据**：ET 模式下必须循环 read 直到返回 EAGAIN，否则会丢数据。
// 2. **TcpConnection 生命周期**：用 shared_ptr + enable_shared_from_this，避免回调时对象已析构。
// 3. **跨线程投递任务**：必须 eventfd 唤醒 EventLoop，否则 epoll_wait 不会立即返回。
// 4. **定时器回调中删除自己**：不能在回调中直接 delete this，要用 weak_ptr 或延迟删除。
// 5. **SIGPIPE 导致进程退出**：服务器必须 signal(SIGPIPE, SIG_IGN)。

// ---------- .gitignore ----------
// build/
// *.o
// *.log
// compile_commands.json
```

</details>

---