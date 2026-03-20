#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "动态修改+区间查询"、"逆序对"
  ✅ 题目特征:
     • 单点修改 + 区间求和（最常用）
     • 求逆序对（离散化+树状数组）
  
  应用场景:
  • 单点修改 + 区间求和
  • 区间修改 + 单点查询（差分）
  • 逆序对统计
  • 动态第k小（权值树状数组）
  
  典型题:
  • "支持修改a[i]，查询sum[l..r]"
  • "求数组的逆序对个数"
  
  ❌ 反例: 需要区间max/min/gcd → 线段树
=============================================================*/

// 单点修改 + 区间查询
struct BIT {
    int n;
    vector<ll> tr;
    BIT(int n) : n(n), tr(n + 1, 0) {}
    void add(int i, ll v) { for (; i <= n; i += i & -i) tr[i] += v; }
    ll ask(int i) { ll s = 0; for (; i > 0; i -= i & -i) s += tr[i]; return s; }
    ll ask(int l, int r) { return ask(r) - ask(l - 1); } // [l, r]
};

// 区间修改 + 单点查询（差分）
struct BIT_diff {
    BIT b;
    BIT_diff(int n) : b(n) {}
    void add(int l, int r, ll v) { b.add(l, v); b.add(r + 1, -v); } // [l,r] += v
    ll ask(int i) { return b.ask(i); }
};

int main() {
    // --- BIT: 单点修改 + 区间求和 ---
    {
        BIT bit(5);
        int a[] = {0, 1, 3, 5, 7, 9}; // 1-indexed
        for (int i = 1; i <= 5; i++) bit.add(i, a[i]);

        assert(bit.ask(1, 3) == 9);   // 1+3+5
        assert(bit.ask(2, 5) == 24);  // 3+5+7+9
        bit.add(3, 10);               // a[3]: 5→15
        assert(bit.ask(1, 3) == 19);  // 1+3+15
        cout << "[BIT] OK\n";
    }
    // --- BIT_diff: 区间修改 + 单点查询 ---
    {
        BIT_diff bit(5);              // 初始全0
        bit.add(2, 4, 3);            // [0, 3, 3, 3, 0]
        bit.add(3, 5, 2);            // [0, 3, 5, 5, 2]
        assert(bit.ask(1)==0 && bit.ask(2)==3 && bit.ask(3)==5);
        assert(bit.ask(4)==5 && bit.ask(5)==2);
        cout << "[BIT_diff] OK\n";
    }
}