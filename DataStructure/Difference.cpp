#include<bits/stdc++.h>
using ll = long long;
using ull = unsigned long long;
using u128 = unsigned __int128;
using i128 = __int128;
using namespace std;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "区间加减"、"区间修改+单点查询"
  ✅ 题目特征: 
     • 多次区间修改，最后查询结果
     • m 次区间操作 + n 个点查询
  
  应用场景:
  • 区间加常数（公交车上下车）
  • 二维差分（矩阵区域加减）
  • 离线处理区间修改
  
  典型题:
  • "m次操作：[l,r] 所有数+v，问最终每个位置的值"
  • "日程安排：多个区间覆盖，问重叠次数"
  
  ❌ 反例: 需要实时查询区间和 → 树状数组/线段树
=============================================================*/

template <typename T>
struct Diff1D {
    int n;
    vector<T> d; 
    // 初始化：0 ~ n-1
    Diff1D(int _n) : n(_n), d(_n + 1, 0) {}
    void add(int l, int r, T v) {
        d[max(0, l)] += v;
        d[min(n, r + 1)] -= v;
    }
    vector<T> get() {
        vector<T> res(n);
        T cur = 0;
        for (int i = 0; i < n; i++) {
            cur += d[i];
            res[i] = cur;
        }
        return res;
    }
};


template <typename T>
struct Diff2D {
    int n, m;
    vector<vector<T>> d; // 差分矩阵 
    Diff2D(int _n, int _m) : n(_n), m(_m), d(_n + 1, vector<T>(_m + 1, 0)) {}
    // (x1, y1) ~ (x2, y2)区间+v 
    void add(int x1, int y1, int x2, int y2, T v) {
        d[max(0, x1)][max(0, y1)] += v;
        d[min(n, x2 + 1)][max(0, y1)] -= v;
        d[max(0, x1)][min(m, y2 + 1)] -= v;
        d[min(n, x2 + 1)][min(m, y2 + 1)] += v;
    }

    // 还原：返回n*m 的结果矩阵
    vector<vector<T>> get() {
        vector<vector<T>> res(n, vector<T>(m));  //注意最后再进行一次还原 
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i > 0) d[i][j] += d[i - 1][j];
                if (j > 0) d[i][j] += d[i][j - 1];
                if (i > 0 && j > 0) d[i][j] -= d[i - 1][j - 1];
                res[i][j] = d[i][j];
            }
        }
        return res;
    }
};

int main() {

    Diff1D<int> d1(5);
    d1.add(1, 3, 2);
    d1.add(2, 4, 3);
    auto res1 = d1.get();
    // 预期: 0 2 5 5 3
    cout << "1D Result: ";
    for(int x : res1) cout << x << " "; 
    cout << "\n";       

    Diff2D<int> d2(4, 4);
    d2.add(1, 1, 2, 2, 1);
    auto res2 = d2.get();
    // 预期: 中间 2x2 区域是 1，其余是 0
    cout << "2D Result:\n";
    for(auto row : res2) {
        for(int x : row) cout << x << " ";
        cout << "\n";
    }

    return 0;
}