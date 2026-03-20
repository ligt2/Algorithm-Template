#include<bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;

/*=============================================================
  C++ STL 完整手册
  
  ==================== 容器复杂度总表 ====================
  
  │ 容器             │ 访问   │ 插入/删除  │ 查找   │ 空间  │
  │──────────────────│────────│───────────│────────│───────│
  │ vector           │ O(1)   │ 尾O(1)    │ O(n)   │ O(n)  │
  │ deque            │ O(1)   │ 头尾O(1)  │ O(n)   │ O(n)  │
  │ list             │ O(n)   │ O(1)*     │ O(n)   │ O(n)  │
  │ set/map          │ -      │ O(logn)   │ O(logn)│ O(n)  │
  │ multiset/multimap│ -      │ O(logn)   │ O(logn)│ O(n)  │
  │ unordered_set/map│ -      │ O(1)平均  │ O(1)   │ O(n)  │
  │ priority_queue   │ top O(1)│ O(logn)  │ -      │ O(n)  │
  │ stack/queue      │ top O(1)│ O(1)     │ -      │ O(n)  │
  
  * list插入删除O(1)需要已知迭代器位置
  
  ==================== 场景速查 ====================
  
  │ 需求                        │ 最佳选择              │
  │────────────────────────────│──────────────────────│
  │ 动态数组/邻接表              │ vector               │
  │ 双端增删                     │ deque                │
  │ 频繁中间插删                 │ list（罕用）          │
  │ 有序+去重                    │ set                  │
  │ 有序+可重复                  │ multiset             │
  │ 键值映射（有序）             │ map                  │
  │ 键值映射（快速）             │ unordered_map        │
  │ 计数器                       │ map<T,int> / 数组    │
  │ 堆/优先级                    │ priority_queue       │
  │ BFS队列                      │ queue                │
  │ DFS/括号匹配                 │ stack                │
  │ 滑动窗口最值                 │ deque（单调队列）     │
  │ 位压缩（n>64）               │ bitset               │
  
=============================================================*/

// =====================================================================
// 1. vector — 动态数组
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度                                                   │
├─────────────────────────────────────────────────────────────┤
│ 随机访问 v[i]        │ O(1)                                 │
│ 尾部插入 push_back   │ 均摊 O(1)                            │
│ 尾部删除 pop_back    │ O(1)                                 │
│ 中间插入/删除        │ O(n)                                 │
│ 查找 find            │ O(n)                                 │
│ 排序 sort            │ O(nlogn)                             │
└─────────────────────────────────────────────────────────────┘

应用场景:
  • 替代普通数组（可变长度，自动扩容）
  • 邻接表存图: vector<vector<pii>> g(n+1)
  • 记录结果/路径
  • DP数组（一维/二维）
  
典型题型:
  • 几乎所有题都用！
  • "存储图的邻接表"
  • "记录答案序列"
  
⚠️ 坑点:
  - size()返回size_t（无符号），i < v.size()-1 当size=0时溢出！
    正确写法: i + 1 < v.size() 或 (int)v.size()
  - vector<bool> 是特殊实现，不能取地址，建议用 vector<int>
*/
void test_vector() {
    // --- 初始化 ---
    vector<int> v1;                        // 空
    vector<int> v2(5);                     // 5个0
    vector<int> v3(5, -1);                 // 5个-1
    vector<int> v4 = {1, 2, 3, 4, 5};      // 列表初始化
    vector<int> v5(v4.begin(), v4.begin()+3); // 拷贝前3个
    
    // --- 常用操作 ---
    v1.push_back(10);       // 尾插
    v1.emplace_back(20);    // 尾插（更快，原地构造）
    v1.pop_back();          // 尾删
    int x = v1.back();      // 尾元素
    int y = v1.front();     // 首元素
    int z = v1[0];          // 下标访问（不检查越界）
    z = v1.at(0);           // 下标访问（检查越界）
    bool empty = v1.empty();
    int sz = v1.size();
    v1.clear();             // 清空
    v1.resize(10);          // 调整大小
    v1.reserve(100);        // 预分配（避免多次扩容）
    
    // --- 二维vector ---
    int n = 3, m = 4;
    vector<vector<int>> mat(n, vector<int>(m, 0)); // n×m全0矩阵
    
    // --- 遍历 ---
    for (int i = 0; i < (int)v4.size(); i++) { /* v4[i] */ }  // 下标
    for (int val : v4) { /* val */ }                          // 范围for
    for (auto it = v4.begin(); it != v4.end(); ++it) { /* *it */ } // 迭代器
    
    cout << "[vector] OK\n";
}

// =====================================================================
// 2. string — 字符串
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度                                                   │
├─────────────────────────────────────────────────────────────┤
│ 随机访问 s[i]        │ O(1)                                 │
│ 拼接 s += t          │ O(|t|)                               │
│ 子串 substr(pos,len) │ O(len)                               │
│ 查找 find(t)         │ O(n*m) 最坏                          │
│ 比较 s == t          │ O(min(n,m))                          │
│ 转数字 stoi/stoll    │ O(n)                                 │
└─────────────────────────────────────────────────────────────┘

应用场景:
  • 字符串处理（比char[]更安全）
  • 支持 + 拼接、比较运算符
  • 高精度数字存储（配合模拟）
  
典型题型:
  • "字符串匹配"（暴力/KMP）
  • "字符串转数字"
  • "字典序比较"
*/
void test_string() {
    // --- 初始化 ---
    string s1;
    string s2 = "hello";
    string s3(5, 'a');        // "aaaaa"
    string s4 = s2 + " world"; // 拼接
    
    // --- 常用操作 ---
    s1 += "abc";              // 追加
    char c = s1[0];           // 访问
    int len = s1.size();      // 长度（同 length()）
    s1.clear();
    s1.empty();
    
    // --- 子串 ---
    string sub = s2.substr(1, 3);  // "ell" (pos=1, len=3)
    string sub2 = s2.substr(2);    // "llo" (pos=2到末尾)
    
    // --- 查找 ---
    size_t pos = s2.find("ll");    // 2 (首次出现位置)
    if (pos == string::npos) { /* 未找到 */ }
    pos = s2.rfind("l");           // 3 (最后出现位置)
    
    // --- 转换 ---
    int num = stoi("123");         // string → int
    ll bignum = stoll("123456789012345");
    double d = stod("3.14");
    string str = to_string(123);   // int → string
    
    // --- 字符串分割（手写）---
    auto split = [](const string& s, char sep) {
        vector<string> res;
        string cur;
        for (char c : s) {
            if (c == sep) { if (!cur.empty()) res.push_back(cur); cur.clear(); }
            else cur += c;
        }
        if (!cur.empty()) res.push_back(cur);
        return res;
    };
    
    // --- 字符串读取 ---
    // cin >> s;         // 读到空格/换行停止
    // getline(cin, s);  // 读整行
    
    cout << "[string] OK\n";
}

// =====================================================================
// 3. set / multiset — 有序集合
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度（红黑树实现）                                       │
├─────────────────────────────────────────────────────────────┤
│ 插入 insert          │ O(logn)                              │
│ 删除 erase           │ O(logn)                              │
│ 查找 find/count      │ O(logn)                              │
│ 二分 lower/upper_bound │ O(logn)                            │
│ 最小 *begin()        │ O(1)                                 │
│ 最大 *rbegin()       │ O(1)                                 │
│ 遍历                 │ O(n)                                 │
└─────────────────────────────────────────────────────────────┘

set vs multiset:
  set:      自动去重，每个元素最多一个
  multiset: 允许重复元素

应用场景:
  • 自动排序+去重（set）
  • 动态维护有序序列
  • 快速查找最小/最大值
  • 动态中位数（两个set/multiset）
  
典型题型:
  • "维护有序集合，支持插入/删除/查最值"
  • "第k小元素"
  • "滑动窗口中位数"（对顶堆/multiset）
  
⚠️ 坑点:
  - multiset.erase(x) 删除所有x！
  - 只删一个：ms.erase(ms.find(x))
*/
void test_set() {
    // --- set基本操作 ---
    set<int> s;
    s.insert(3);
    s.insert(1);
    s.insert(4);
    s.insert(1);              // 重复，不插入
    // s = {1, 3, 4}
    
    s.count(3);               // 1（存在返回1，否则0）
    s.find(3);                // 返回迭代器（不存在返回end()）
    s.erase(3);               // 删除值
    s.erase(s.begin());       // 删除迭代器
    
    int minVal = *s.begin();  // 最小值
    int maxVal = *s.rbegin(); // 最大值
    
    // --- 二分查找 ---
    s = {1, 3, 5, 7, 9};
    auto it = s.lower_bound(4);  // 第一个≥4 → 5
    it = s.upper_bound(5);       // 第一个>5 → 7
    
    // 前驱（<x的最大值）
    it = s.lower_bound(5);
    if (it != s.begin()) { --it; int prev = *it; } // 3
    
    // 后继（>x的最小值）
    it = s.upper_bound(5);
    if (it != s.end()) { int next = *it; } // 7
    
    // --- multiset ---
    multiset<int> ms;
    ms.insert(1);
    ms.insert(1);
    ms.insert(2);
    // ms = {1, 1, 2}
    
    ms.count(1);              // 2
    ms.erase(1);              // ⚠️ 删除所有1！
    ms.erase(ms.find(2));     // 只删一个2
    
    cout << "[set/multiset] OK\n";
}

// =====================================================================
// 4. map / unordered_map — 键值映射
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度                                                   │
├─────────────────────────────────────────────────────────────┤
│              │ map (红黑树)  │ unordered_map (哈希表)       │
│ 插入         │ O(logn)       │ O(1) 平均 / O(n) 最坏        │
│ 删除         │ O(logn)       │ O(1) 平均                    │
│ 查找         │ O(logn)       │ O(1) 平均                    │
│ 遍历         │ O(n) 有序     │ O(n) 无序                    │
│ lower_bound  │ O(logn)       │ ❌ 不支持                    │
└─────────────────────────────────────────────────────────────┘

何时用map vs unordered_map:
  map:           需要有序/需要二分/key不可哈希
  unordered_map: 只需查找（更快），但可能被卡

应用场景:
  • 计数器 map<T, int> cnt
  • 离散化（值→编号）
  • 记忆化搜索
  • 建立映射关系
  
典型题型:
  • "统计每个元素出现次数"
  • "两数之和"（哈希表O(n)）
  • "字符串映射到id"
  
⚠️ 坑点:
  - mp[key] 若key不存在会自动创建（值为0）！
  - 判断存在用 mp.count(key) 或 mp.find(key) != mp.end()
*/
void test_map() {
    // --- 基本操作 ---
    map<string, int> mp;
    mp["Alice"] = 90;
    mp["Bob"] = 85;
    mp["Alice"] = 95;         // 覆盖
    
    mp.count("Alice");        // 1（存在）
    mp.count("Carol");        // 0（不存在）
    
    // ⚠️ mp["Carol"] 会创建 Carol→0
    if (mp.count("Carol")) {} // 正确判断
    
    mp.erase("Bob");
    
    // --- 遍历（有序）---
    for (auto& [k, v] : mp) {
        // k: key, v: value
    }
    
    // --- 计数器 ---
    map<int, int> cnt;
    vector<int> nums = {1, 2, 1, 3, 1, 2};
    for (int x : nums) cnt[x]++;
    // cnt = {1:3, 2:2, 3:1}
    
    // --- unordered_map（更快）---
    unordered_map<int, int> ump;
    ump[100] = 200;
    // 用法与map相同，但无序，且不支持lower_bound
    
    // --- 防止哈希表被卡（自定义哈希）---
    struct SafeHash {
        size_t operator()(ll x) const {
            static const size_t FIXED = chrono::steady_clock::now().time_since_epoch().count();
            return x ^ (FIXED >> 1);
        }
    };
    unordered_map<ll, int, SafeHash> safe_mp;
    
    cout << "[map] OK\n";
}

// =====================================================================
// 5. stack — 栈
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度                                                   │
├─────────────────────────────────────────────────────────────┤
│ 入栈 push            │ O(1)                                 │
│ 出栈 pop             │ O(1)                                 │
│ 访问栈顶 top         │ O(1)                                 │
│ 判空 empty           │ O(1)                                 │
│ 大小 size            │ O(1)                                 │
└─────────────────────────────────────────────────────────────┘

应用场景:
  • 括号匹配
  • 表达式求值（中缀转后缀）
  • DFS非递归
  • 单调栈
  
典型题型:
  • "有效的括号"
  • "计算器"（表达式求值）
  • "下一个更大元素"
*/
void test_stack() {
    stack<int> stk;
    stk.push(1);
    stk.push(2);
    stk.push(3);
    
    int top = stk.top();      // 3（不弹出）
    stk.pop();                // 弹出3
    top = stk.top();          // 2
    
    bool empty = stk.empty();
    int sz = stk.size();
    
    // ⚠️ 没有clear()，清空方法：
    while (!stk.empty()) stk.pop();
    // 或 stk = stack<int>();
    
    cout << "[stack] OK\n";
}

// =====================================================================
// 6. queue — 队列
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度                                                   │
├─────────────────────────────────────────────────────────────┤
│ 入队 push            │ O(1)                                 │
│ 出队 pop             │ O(1)                                 │
│ 访问队首 front       │ O(1)                                 │
│ 访问队尾 back        │ O(1)                                 │
└─────────────────────────────────────────────────────────────┘

应用场景:
  • BFS广度优先搜索
  • 层序遍历
  • 模拟排队
  
典型题型:
  • "二叉树层序遍历"
  • "最短路径（无权图）"
  • "腐烂的橘子"
*/
void test_queue() {
    queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    
    int front = q.front();    // 1
    int back = q.back();      // 3
    q.pop();                  // 弹出1
    front = q.front();        // 2
    
    cout << "[queue] OK\n";
}

// =====================================================================
// 7. deque — 双端队列
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度                                                   │
├─────────────────────────────────────────────────────────────┤
│ 头部插入 push_front  │ O(1)                                 │
│ 尾部插入 push_back   │ O(1)                                 │
│ 头部删除 pop_front   │ O(1)                                 │
│ 尾部删除 pop_back    │ O(1)                                 │
│ 随机访问 []          │ O(1) （比vector慢）                   │
│ 中间插入/删除        │ O(n)                                 │
└─────────────────────────────────────────────────────────────┘

应用场景:
  • 单调队列（滑动窗口最值）
  • 需要双端操作的队列
  • 0-1 BFS
  
典型题型:
  • LC239 "滑动窗口最大值"
  • "双端队列模拟"
*/
void test_deque() {
    deque<int> dq;
    dq.push_back(2);          // [2]
    dq.push_front(1);         // [1, 2]
    dq.push_back(3);          // [1, 2, 3]
    
    int front = dq.front();   // 1
    int back = dq.back();     // 3
    int mid = dq[1];          // 2（支持随机访问）
    
    dq.pop_front();           // [2, 3]
    dq.pop_back();            // [2]
    
    cout << "[deque] OK\n";
}

// =====================================================================
// 8. priority_queue — 优先队列（堆）
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度（二叉堆实现）                                       │
├─────────────────────────────────────────────────────────────┤
│ 入堆 push            │ O(logn)                              │
│ 出堆 pop             │ O(logn)                              │
│ 访问堆顶 top         │ O(1)                                 │
│ 建堆（heapify）       │ O(n)                                │
└─────────────────────────────────────────────────────────────┘

应用场景:
  • Dijkstra最短路
  • 贪心算法（每次取最大/最小）
  • Top K 问题
  • 合并K个有序链表
  • 对顶堆（动态中位数）
  
典型题型:
  • "最短路径"
  • "合并K个升序链表"
  • "数据流的中位数"
  • "会议室安排"
  
⚠️ 注意:
  - 默认大根堆！
  - 没有clear()
  - 不能遍历/修改内部元素
*/
void test_priority_queue() {
    // --- 大根堆（默认）---
    priority_queue<int> maxHeap;
    maxHeap.push(3);
    maxHeap.push(1);
    maxHeap.push(4);
    int top = maxHeap.top();  // 4（最大）
    maxHeap.pop();
    
    // --- 小根堆 ---
    priority_queue<int, vector<int>, greater<int>> minHeap;
    minHeap.push(3);
    minHeap.push(1);
    minHeap.push(4);
    top = minHeap.top();      // 1（最小）
    
    // --- pair自动比较（先first后second）---
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({2, 100});
    pq.push({1, 200});
    pq.push({2, 50});
    auto [d, u] = pq.top();   // {1, 200}
    
    // --- 自定义比较（lambda）---
    auto cmp = [](pii a, pii b) { return a.second > b.second; }; // 按second小根堆
    priority_queue<pii, vector<pii>, decltype(cmp)> pq2(cmp);
    
    // --- 清空 ---
    while (!maxHeap.empty()) maxHeap.pop();
    // 或 maxHeap = priority_queue<int>();
    
    cout << "[priority_queue] OK\n";
}

// =====================================================================
// 9. 常用算法
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 算法                 │ 复杂度      │ 用途                   │
├─────────────────────────────────────────────────────────────┤
│ sort                 │ O(nlogn)   │ 排序                   │
│ stable_sort          │ O(nlogn)   │ 稳定排序               │
│ nth_element          │ O(n)       │ 第k小（不完全排序）      │
│ partial_sort         │ O(nlogk)   │ 前k小排序              │
│ reverse              │ O(n)       │ 反转                   │
│ unique               │ O(n)       │ 去重（需先排序）        │
│ lower_bound          │ O(logn)    │ 第一个≥x               │
│ upper_bound          │ O(logn)    │ 第一个>x               │
│ binary_search        │ O(logn)    │ 是否存在               │
│ next_permutation     │ O(n)       │ 下一个排列             │
│ prev_permutation     │ O(n)       │ 上一个排列             │
│ max_element          │ O(n)       │ 最大值迭代器           │
│ min_element          │ O(n)       │ 最小值迭代器           │
│ accumulate           │ O(n)       │ 求和/累积              │
│ count                │ O(n)       │ 统计出现次数           │
│ fill                 │ O(n)       │ 填充                   │
│ copy                 │ O(n)       │ 复制                   │
│ merge                │ O(n+m)     │ 合并有序序列           │
│ set_union            │ O(n+m)     │ 集合并集               │
│ set_intersection     │ O(n+m)     │ 集合交集               │
└─────────────────────────────────────────────────────────────┘
*/
void test_algorithm() {
    vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // --- 排序 ---
    sort(v.begin(), v.end());                      // 升序
    sort(v.begin(), v.end(), greater<int>());      // 降序
    sort(v.begin(), v.end(), [](int a, int b) {    // 自定义
        return abs(a) < abs(b);                    // 按绝对值升序
    });
    
    // --- 去重（必须先排序！）---
    v = {3, 1, 4, 1, 5, 9, 2, 6};
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    // v = {1, 2, 3, 4, 5, 6, 9}
    
    // --- 二分查找 ---
    bool found = binary_search(v.begin(), v.end(), 4);  // true
    auto it = lower_bound(v.begin(), v.end(), 4);       // 指向4
    int idx = it - v.begin();                           // 下标
    it = upper_bound(v.begin(), v.end(), 4);            // 指向5
    
    // --- 全排列 ---
    vector<int> p = {1, 2, 3};
    do {
        // 处理当前排列
    } while (next_permutation(p.begin(), p.end()));
    // 共 3! = 6 次
    
    // --- 求和 ---
    v = {1, 2, 3, 4, 5};
    ll sum = accumulate(v.begin(), v.end(), 0LL);  // 15
    // ⚠️ 第三个参数决定返回类型！用0LL防溢出
    
    // --- 最值 ---
    int maxVal = *max_element(v.begin(), v.end());
    int minVal = *min_element(v.begin(), v.end());
    auto [minIt, maxIt] = minmax_element(v.begin(), v.end());
    
    // --- 填充 ---
    fill(v.begin(), v.end(), 0);
    
    // --- 第k小（不完全排序）---
    v = {3, 1, 4, 1, 5, 9, 2, 6};
    nth_element(v.begin(), v.begin() + 3, v.end());
    // v[3] 是第4小的元素（0-indexed）
    
    cout << "[algorithm] OK\n";
}

// =====================================================================
// 10. 数值算法
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 函数                 │ 头文件      │ 说明                   │
├─────────────────────────────────────────────────────────────┤
│ __gcd(a, b)          │ 内置        │ GCC最大公约数          │
│ gcd(a, b)            │ <numeric>   │ C++17 GCD             │
│ lcm(a, b)            │ <numeric>   │ C++17 LCM             │
│ abs(x)               │ <cstdlib>   │ 绝对值                 │
│ pow(x, y)            │ <cmath>     │ x^y (浮点)             │
│ sqrt(x)              │ <cmath>     │ 平方根                 │
│ ceil(x) / floor(x)   │ <cmath>     │ 上取整/下取整          │
└─────────────────────────────────────────────────────────────┘
*/
void test_numeric() {
    // GCD / LCM
    int g = __gcd(12, 18);    // 6
    // int l = lcm(4, 6);     // 12 (C++17)
    
    // 手写lcm（防溢出）
    auto my_lcm = [](ll a, ll b) { return a / __gcd(a, b) * b; };
    
    // 快速幂（手写，用于取模）
    auto qpow = [](ll a, ll b, ll mod) {
        ll res = 1;
        a %= mod;
        while (b > 0) {
            if (b & 1) res = res * a % mod;
            a = a * a % mod;
            b >>= 1;
        }
        return res;
    };
    
    cout << "[numeric] OK\n";
}

// =====================================================================
// 11. pair / tuple — 组合类型
// =====================================================================
/*
应用场景:
  pair:  坐标(x,y)、边(u,v)、距离+节点{dist,node}
  tuple: 三维坐标、多关键字排序
  
⚠️ pair/tuple 自动按字典序比较（先first/第一个）
*/
void test_pair_tuple() {
    // --- pair ---
    pair<int, int> p1 = {1, 2};
    pair<int, int> p2 = make_pair(3, 4);
    int x = p1.first, y = p1.second;
    
    // C++17 结构化绑定
    auto [a, b] = p1;
    
    // 比较（先first后second）
    vector<pii> v = {{1,3}, {1,2}, {2,1}};
    sort(v.begin(), v.end());
    // v = {{1,2}, {1,3}, {2,1}}
    
    // --- tuple ---
    tuple<int, int, int> t = {1, 2, 3};
    int t0 = get<0>(t);       // 1
    int t1 = get<1>(t);       // 2
    
    // 解包
    int ta, tb, tc;
    tie(ta, tb, tc) = t;
    
    // C++17 结构化绑定
    auto [tx, ty, tz] = t;
    
    cout << "[pair/tuple] OK\n";
}

// =====================================================================
// 12. bitset — 位集合
// =====================================================================
/*
┌─────────────────────────────────────────────────────────────┐
│ 时间复杂度（N为bitset大小）                                    │
├─────────────────────────────────────────────────────────────┤
│ 位操作 &|^~          │ O(N/64) ≈ O(N/w)                     │
│ count()              │ O(N/64)                              │
│ set/reset/flip       │ O(1) 单个 / O(N) 全部                │
│ test/[]              │ O(1)                                 │
└─────────────────────────────────────────────────────────────┘

应用场景:
  • 状态压缩（n>64时）
  • 集合运算优化
  • 可达性/传递闭包
  • 背包DP优化（bitset DP）
  
典型题型:
  • "状压DP"（状态数很大）
  • "集合交并差"
  • "01背包优化"
  
⚠️ bitset大小必须是编译期常量！
*/
void test_bitset() {
    bitset<100> bs1;
    bitset<100> bs2("1010");  // 从字符串初始化
    
    bs1.set();                // 全部置1
    bs1.reset();              // 全部置0
    bs1.flip();               // 全部翻转
    
    bs1.set(5);               // 第5位置1
    bs1.reset(5);             // 第5位置0
    bs1.flip(5);              // 第5位翻转
    
    bool b = bs1.test(5);     // 查询第5位
    b = bs1[5];               // 同上
    
    int cnt = bs1.count();    // 1的个数
    bool any = bs1.any();     // 是否有1
    bool none = bs1.none();   // 是否全0
    bool all = bs1.all();     // 是否全1
    
    // 位运算
    bitset<100> bs3 = bs1 & bs2;   // 与
    bs3 = bs1 | bs2;               // 或
    bs3 = bs1 ^ bs2;               // 异或
    bs3 = ~bs1;                    // 非
    
    // 移位
    bs3 = bs1 << 3;
    bs3 = bs1 >> 2;
    
    // 转换
    unsigned long val = bs2.to_ulong();
    string str = bs2.to_string();
    
    cout << "[bitset] OK\n";
}

// =====================================================================
// 13. 常用技巧与模板
// =====================================================================
void test_tricks() {
    // --- 快速读入 ---
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // --- 常用常量 ---
    const int INF = 0x3f3f3f3f;
    const ll LINF = 0x3f3f3f3f3f3f3f3fLL;
    const int MOD = 1e9 + 7;
    
    // --- 数组初始化 ---
    int arr[100];
    memset(arr, 0, sizeof arr);     // 全0
    memset(arr, -1, sizeof arr);    // 全-1
    memset(arr, 0x3f, sizeof arr);  // 全INF
    fill(arr, arr + 100, 5);        // 全5（万能）
    
    // --- 坐标压缩（离散化）---
    vector<int> v = {100, 5, 200, 5, 100};
    vector<int> tmp = v;
    sort(tmp.begin(), tmp.end());
    tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
    for (int& x : v) {
        x = lower_bound(tmp.begin(), tmp.end(), x) - tmp.begin();
    }
    // v = {1, 0, 2, 0, 1}  (值域压缩到0~2)
    
    // --- lambda递归（C++14）---
    auto dfs = [&](auto&& self, int u) -> void {
        // self(self, v);  // 递归调用
    };
    // dfs(dfs, 0);
    
    cout << "[tricks] OK\n";
}

// =====================================================================
//                            主函数
// =====================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    test_vector();
    test_string();
    test_set();
    test_map();
    test_stack();
    test_queue();
    test_deque();
    test_priority_queue();
    test_algorithm();
    test_numeric();
    test_pair_tuple();
    test_bitset();
    test_tricks();
    
    cout << "\n==========================================\n";
    cout << "       All STL Tests Passed!\n";
    cout << "==========================================\n";
    
    return 0;
}