#include "TaskManager.h"
#include <iostream>

// ============================================================
// TaskManager 功能测试
// 覆盖 add / showPending / complete / sortByPriority / remove 全部成员，
// 每步都打印未完成列表，便于对照各方法的实际效果。
// ============================================================

int main() {
    TaskManager tm;

    // ---- 1. 添加任务（id 由 nextId_ 从 1 自增）----
    std::cout << "=== 1. 添加任务 ===\n";
    tm.add("写周报", 2);        // id=1, 中优先级
    tm.add("修复登录 Bug", 3);  // id=2, 高优先级
    tm.add("整理桌面", 1);      // id=3, 低优先级
    tm.add("代码评审", 3);      // id=4, 高优先级
    std::cout << "已添加 4 个任务（id: 1~4）\n\n";

    // ---- 2. 显示未完成任务（期望：4 个全部列出）----
    std::cout << "=== 2. 未完成任务列表（期望：显示全部 4 个）===\n";
    tm.showPending();
    std::cout << "\n";

    // ---- 3. 完成任务（期望：id=1「写周报」标记 done 后从列表消失）----
    std::cout << "=== 3. 完成任务 id=1（写周报）===\n";
    tm.complete(1);
    tm.showPending();   // 期望：只剩 id=2,3,4
    std::cout << "\n";

    // ---- 4. 按优先级排序 ----
    // 当前 sortByPriority 用 (a.priority > b.priority)，是【降序】：
    // 期望 priority 大的排前面 → 2(p3)、4(p3) 在前，3(p1) 在最后。
    // 注意：相同优先级（id=2、id=4 都是 p3）的相对顺序不保证，std::sort 是非稳定排序。
    std::cout << "=== 4. 按优先级排序（当前实现：降序 高→低）===\n";
    tm.sortByPriority();
    tm.showPending();
    std::cout << "\n";

    // ---- 5. 删除任务（期望：id=3「整理桌面」被彻底移除）----
    std::cout << "=== 5. 删除任务 id=3（整理桌面）===\n";
    tm.remove(3);
    tm.showPending();   // 期望：只剩 id=2,4
    std::cout << "\n";

    std::cout << "=== 测试结束 ===\n";
    return 0;
}
