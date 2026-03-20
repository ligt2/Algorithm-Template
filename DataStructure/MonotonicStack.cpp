#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  单调栈 (Monotonic Stack)
  
  应用场景:
  1. 下一个/上一个 更大/更小元素
  2. 柱状图最大矩形 (LC84)
  3. 接雨水 (LC42)
  4. 子数组最小值之和 (LC907)
  5. 股票价格跨度 / 每日温度
  
  四种变体速查:
  ┌──────────────┬──────────┬──────────┬──────────┬─────────┐
  │   需求        │ 遍历方向 │ 弹出条件  │ 栈单调性  │ 默认值  │
  ├──────────────┼──────────┼──────────┼──────────┼─────────┤
  │ nextGreater  │ ← 右到左 │ a[top]<=x│ 递减栈   │  n      │
  │ nextSmaller  │ ← 右到左 │ a[top]>=x│ 递增栈   │  n      │
  │ prevGreater  │ → 左到右 │ a[top]<=x│ 递减栈   │ -1      │
  │ prevSmaller  │ → 左到右 │ a[top]>=x│ 递增栈   │ -1      │
  └──────────────┴──────────┴──────────┴──────────┴─────────┘
  
  严格 vs 非严格:
  - 找 >  : 弹出 <=       - 找 >= : 弹出 <
  - 找 <  : 弹出 >=       - 找 <= : 弹出 >
  
  复杂度: O(n)  每个元素最多入栈出栈各一次
=============================================================*/

// 右边第一个 > a[i] 的下标，不存在返回 n
vector<int> nextGreater(vector<int>& a) {
    int n = a.size(); vector<int> res(n, n); stack<int> stk;
    for (int i = n-1; i >= 0; i--) {
        while (!stk.empty() && a[stk.top()] <= a[i]) stk.pop();
        if (!stk.empty()) res[i] = stk.top();
        stk.push(i);
    } return res;
}

// 右边第一个 < a[i] 的下标，不存在返回 n
vector<int> nextSmaller(vector<int>& a) {
    int n = a.size(); vector<int> res(n, n); stack<int> stk;
    for (int i = n-1; i >= 0; i--) {
        while (!stk.empty() && a[stk.top()] >= a[i]) stk.pop();
        if (!stk.empty()) res[i] = stk.top();
        stk.push(i);
    } return res;
}

// 左边第一个 > a[i] 的下标，不存在返回 -1
vector<int> prevGreater(vector<int>& a) {
    int n = a.size(); vector<int> res(n, -1); stack<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && a[stk.top()] <= a[i]) stk.pop();
        if (!stk.empty()) res[i] = stk.top();
        stk.push(i);
    } return res;
}

// 左边第一个 < a[i] 的下标，不存在返回 -1
vector<int> prevSmaller(vector<int>& a) {
    int n = a.size(); vector<int> res(n, -1); stack<int> stk;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && a[stk.top()] >= a[i]) stk.pop();
        if (!stk.empty()) res[i] = stk.top();
        stk.push(i);
    } return res;
}

// 经典应用：柱状图最大矩形
ll largestRectangle(vector<int>& h) {
    auto L = prevSmaller(h), R = nextSmaller(h);
    ll ans = 0;
    for (int i = 0; i < (int)h.size(); i++)
        ans = max(ans, (ll)h[i] * (R[i] - L[i] - 1));
    return ans;
}

int main() {
    // --- 四种变体 ---
    {
        vector<int> a = {2, 1, 3, 2, 4};
        //                0  1  2  3  4
        assert(nextGreater(a) == (vector<int>{2, 2, 4, 4, 5}));
        assert(nextSmaller(a) == (vector<int>{1, 5, 3, 5, 5}));
        assert(prevGreater(a) == (vector<int>{-1, 0, -1, 2, -1}));
        assert(prevSmaller(a) == (vector<int>{-1, -1, 1, 1, 3}));
        cout << "[MonoStack] OK\n";
    }
    // --- 柱状图最大矩形 ---
    {
        vector<int> h = {2, 1, 5, 6, 2, 3};
        // 最大矩形: 高5 × 宽2(下标2~3) = 10
        assert(largestRectangle(h) == 10);
        cout << "[Histogram] OK: " << largestRectangle(h) << "\n";
    }
}