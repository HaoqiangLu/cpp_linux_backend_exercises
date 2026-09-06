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
  - [ ] 自然使用 `find`、`substr`、`s +=` 等操作。
  - [ ] 理解 SSO（Small String Optimization）对性能的影响。

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
  - [ ] 熟练使用 `m[key]++`、`m.find()`、`m.at()`。
  - [ ] 能用范围 for + 结构化绑定（C++17）遍历键值对。

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
  - [ ] 不再用数组+手动去重解决集合类问题。
  - [ ] 理解 `count` 与 `find` 在语义和性能上的区别。

<details>
<summary>📦 练习框架代码</summary>

```cpp
// === 1.4 set/unordered_set 练习框架 ===
// 项目结构:
// 1-4-set/
// ├── blacklist.h
// ├── blacklist.cpp
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
  - [ ] 能正确书写 `[=]`、`[&]`、`[x]`、`[&x]` 等捕获列表并理解其生命周期风险。
  - [ ] 理解算法的复杂度保证。

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
  - [ ] 理解 `auto`、`auto&`、`const auto&`、`auto&&` 的推导差异。

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
  - [ ] 知道范围 for 不能用于需要修改容器结构（如删除元素）的场景。

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
  - [ ] 理解 Lambda 本质是仿函数对象，可作为参数传递或存储。
  - [ ] 避免在 Lambda 中悬挂引用。

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
  - [ ] 知道 `make_unique/make_shared` 优于直接 `new`。
  - [ ] 理解循环引用问题及 `weak_ptr` 解决方案。

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
  - [ ] 知道移动后源对象处于有效但未指定状态。
  - [ ] 能在性能敏感场景正确应用移动语义。

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
  - [ ] 理解封装不仅是隐藏数据，更是保护不变量。

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
  - [ ] 理解 RAII 是 C++ 资源管理的基石。
  - [ ] 知道 Rule of Three/Five 的适用场景。

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
  - [ ] 知道 protected 成员的可见性规则。

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
  - [ ] 始终对重写函数使用 `override` 关键字。
  - [ ] 理解虚函数表（vtable）的实现原理。

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
// ├── Logger.cpp
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
  - [ ] 理解纯虚析构函数的特殊写法。

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
//   ├── Shape.cpp
//   ├── circle.h / Circle.cpp
//   ├── Rectangle.h / Rectangle.cpp
//   ├── triangle.h / Triangle.cpp
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

- **练习目标**：掌握 Linux 文件操作；理解文件描述符；区分系统调用与标准 I/O。
- **练习任务**：
    1. 实现简化版 `cp`：复制文件。
    2. 实现简化版 `cat`：打印文件内容。
    3. 统计文件大小。
    4. 逐行读取文本文件。
    5. 日志写入文件，支持追加模式。
    6. 实现简易配置文件解析器（key=value 格式）。
- **巩固标准**：
  - [ ] 能用 `open/read/write/close` 完成基本文件操作。
  - [ ] 理解文件描述符的本质与继承规则。
  - [ ] 知道何时用系统调用、何时用 stdio。

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
#include <sys/types.h>

// 练习1: 简化版 cp（复制文件）
void copyFile(const std::string& src, const std::string& dst);

// 练习2: 简化版 cat（打印文件内容）
void catFile(const std::string& filename);

// 练习3: 统计文件大小
off_t fileSize(const std::string& filename);

// 练习4: 逐行读取文本文件
void readLines(const std::string& filename);

// 练习5: 日志写入（追加模式）
void writeLog(const std::string& filename, const std::string& msg);

// ---------- FileIoExercises.cpp ----------
#include "FileIoExercises.h"

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

void copyFile(const std::string& src, const std::string& dst) {
    // TODO: open 源文件(O_RDONLY)，open 目标文件(O_WRONLY|O_CREAT|O_TRUNC)
    // TODO: 循环 read/write，完成后 close
}

void catFile(const std::string& filename) {
    // TODO: open 文件，循环 read 到 buffer，打印到 stdout，close
}

off_t fileSize(const std::string& filename) {
    // TODO: 用 stat 或 lseek 获取文件大小
    return 0;
}

void readLines(const std::string& filename) {
    // TODO: std::ifstream 逐行读取并打印
}

void writeLog(const std::string& filename, const std::string& msg) {
    // TODO: open(filename.c_str(), O_WRONLY|O_CREAT|O_APPEND)，write，close
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
  - [ ] 掌握 exec 族函数的区别与用法。
  - [ ] 知道僵尸进程的产生原因与避免方法。

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

#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

void miniShell() {
    char cmd[1024];
    while (true) {
        std::cout << "myshell> ";
        // TODO: std::fgets 读取输入到 cmd
        // TODO: std::strtok 解析命令和参数
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
  - [ ] 理解多线程必须考虑同步，而非“写得快就对”。
  - [ ] 知道 `join/detach` 的区别与必要性。
  - [ ] 能识别常见的数据竞争场景。

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
    // TODO: 创建多个 std::thread，各自调用 printNumbers
    // TODO: join 所有线程
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
  - [ ] 理解锁粒度对性能的影响。
  - [ ] 知道条件变量必须配合 while 循环检查谓词（防虚假唤醒）。

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
        done = true;
        cv.notify_all();
    };

    auto consumer = [&]() {
        while (true) {
            // TODO: std::unique_lock + cv.wait(buffer 非空 || done) + pop + 处理
            if (done && buffer.empty()) break;
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
  - [ ] 理解三次握手/四次挥手在 API 层面的体现。
  - [ ] 知道粘包问题及简单解决方案。

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
  - [ ] 掌握 ET 模式下非阻塞 IO + 循环读写的必要性。
  - [ ] 能搭建可支撑万级并发的服务器框架。

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
