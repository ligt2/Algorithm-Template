#include<bits/stdc++.h>
using namespace std;
/*=============================================================
  什么时候用？
  明确信号词: "第一个/最后一个满足条件"、"有序"、"单调性"
  题目特征: 
     • 有序数组/答案有单调性
     • 数据范围 n≥10⁵（遍历会TLE）
  
  应用场景:
  • lower_bound/upper_bound（STL二分）
  • 答案二分（最大化最小值/最小化最大值）
  • 实数二分（误差ε）
  
  典型题:
  • "第一个 ≥x 的位置"
  • "分配问题：最大化最小分配值"
  • "切割问题：最小化最大长度"
  
  ❌ 反例: 无序数组查找 → 哈希表
=============================================================*/
// 找第一个满足条件的位置
template<typename T, typename F>
T bs_first(T l, T r, F check) {
    T ans = r + 1;
    while (l <= r) {
        T mid = l + (r - l) / 2;
        if (check(mid)) { ans = mid; r = mid - 1; }
        else l = mid + 1;
    }
    return ans;
}

// 找最后一个满足条件的位置
template<typename T, typename F>
T bs_last(T l, T r, F check) {
    T ans = l - 1;
    while (l <= r) {
        T mid = l + (r - l) / 2;
        if (check(mid)) { ans = mid; l = mid + 1; }
        else r = mid - 1;
    }
    return ans;
}

int main() {
    vector<int> v = {1, 2, 2, 2, 4, 6};
    int n = v.size();
    
    // 找第一个 >= 2 的下标 → 1
    cout << bs_first(0, n-1, [&](int i){ return v[i] >= 2; }) << "\n";
    //找最后一个 < 2 的下标  → 0
    cout << bs_last(0, n-1, [&](int i){ return v[i] < 2; })   << "\n";
	cout << "---\n";


  // ============ lower_bound / upper_bound ============
    // 前提：序列已排序（非降序）
    // lower_bound(begin, end, x) → 第一个 >= x 的迭代器
    // upper_bound(begin, end, x) → 第一个 >  x 的迭代器
    // 返回 end 表示不存在，使用前务必判断好 || 转下标：it - v.begin()    取值：*it

    // 场景1: 第一个 >= 4 的位置
    auto lb = lower_bound(v.begin(), v.end(), 4);
    cout << "first >= 4: idx=" << (lb - v.begin()) << "\n"; // 4

    // 场景2: 第一个 > 2 的位置
    auto ub = upper_bound(v.begin(), v.end(), 2);
    cout << "first > 2:  idx=" << (ub - v.begin()) << "\n"; // 4

    // 场景3: x 出现次数 = upper_bound - lower_bound
    int cnt = upper_bound(v.begin(), v.end(), 2) - lower_bound(v.begin(), v.end(), 2);
    cout << "count of 2: " << cnt << "\n"; // 3

    // 场景4: 最后一个 <= x → upper_bound往前退一步
    auto it = upper_bound(v.begin(), v.end(), 3);
    if (it != v.begin()) cout << "last <= 3: " << *prev(it) << "\n"; // 2

    // 场景5: 最后一个 < x → lower_bound往前退一步
    it = lower_bound(v.begin(), v.end(), 2);
    if (it != v.begin()) cout << "last < 2:  " << *prev(it) << "\n"; // 1

    // 场景6: 判断 x 是否存在
    lb = lower_bound(v.begin(), v.end(), 4);
    bool found = (lb != v.end() && *lb == 4);
    cout << "4 exists: " << found << "\n"; // 1

 
}