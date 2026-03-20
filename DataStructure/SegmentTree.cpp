#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "区间修改+区间查询"、"lazy标记"
  ✅ 题目特征:
     • 区间加/赋值 + 区间求和/最值
     • 需要维护max/min/gcd（树状数组不行）
  
  应用场景:
  • 区间修改 + 区间查询（加/赋值 + 和/最值）
  • 动态RMQ（区间最值）
  • 扫描线（面积并）
  • 线段树上二分
  
  典型题:
  • "区间[l,r]所有数+v，查询区间和/最大值"
  • "区间赋值为x，查询区间最小值"
  
  ❌ 反例: 只需单点改+区间和 → 树状数组（更快）
=============================================================*/

struct SegTree {
    int n;
    vector<ll> sum, lz;

    SegTree(int n) : n(n), sum(4*n+4, 0), lz(4*n+4, 0) {}

    // 从 1-indexed 数组 a[1..n] 建树
    SegTree(vector<ll>& a) : SegTree((int)a.size() - 1) { build(a, 1, 1, n); }

    void build(vector<ll>& a, int o, int l, int r) {
        if (l == r) { sum[o] = a[l]; return; }
        int m = (l + r) / 2;
        build(a, o*2, l, m); build(a, o*2+1, m+1, r);
        sum[o] = sum[o*2] + sum[o*2+1];
    }

    void push_down(int o, int l, int r) {
        if (lz[o]) {
            int m = (l + r) / 2;
            sum[o*2] += lz[o]*(m-l+1);  sum[o*2+1] += lz[o]*(r-m);
            lz[o*2]  += lz[o];          lz[o*2+1]  += lz[o];
            lz[o] = 0;
        }
    }

    void update(int o, int l, int r, int ql, int qr, ll v) {
        if (ql <= l && r <= qr) { sum[o] += v*(r-l+1); lz[o] += v; return; }
        push_down(o, l, r);
        int m = (l + r) / 2;
        if (ql <= m) update(o*2, l, m, ql, qr, v);
        if (qr > m)  update(o*2+1, m+1, r, ql, qr, v);
        sum[o] = sum[o*2] + sum[o*2+1];
    }

    ll query(int o, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return sum[o];
        push_down(o, l, r);
        int m = (l + r) / 2;
        ll res = 0;
        if (ql <= m) res += query(o*2, l, m, ql, qr);
        if (qr > m)  res += query(o*2+1, m+1, r, ql, qr);
        return res;
    }

    // ---- 对外接口 ----
    void update(int l, int r, ll v) { update(1, 1, n, l, r, v); }
    ll   query(int l, int r)        { return query(1, 1, n, l, r); }
};

int main() {
    // a[1..5] = {1, 2, 3, 4, 5}
    vector<ll> a = {0, 1, 2, 3, 4, 5};
    SegTree seg(a);

    assert(seg.query(1, 5) == 15);   // 全部和
    assert(seg.query(2, 4) == 9);    // 2+3+4

    seg.update(2, 4, 10);            // → [1,12,13,14,5]
    assert(seg.query(2, 4) == 39);   // 12+13+14
    assert(seg.query(1, 5) == 45);

    seg.update(3, 3, -5);            // 单点修改 a[3]=8
    assert(seg.query(3, 3) == 8);

    cout << "[SegTree] OK\n";
}