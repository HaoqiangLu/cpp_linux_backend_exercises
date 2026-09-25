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
