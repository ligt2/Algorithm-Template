#include<bits/stdc++.h>
using ll = long long;
using namespace std;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "区间和"、"子数组和"
  ✅ 题目特征:
     • 多次查询区间和，数组不变
     • 子数组和问题
  
  应用场景:
  • 一维前缀和（区间和）
  • 二维前缀和（矩阵区域和）
  • 前缀和+哈希（子数组和为k）
  • 前缀异或（区间异或）
  
  典型题:
  • "查询sum[l..r]，数组固定"
  • "和为k的子数组个数"（前缀和+哈希）
  
  ❌ 反例: 动态修改 → 树状数组/线段树
=============================================================*/

template <typename T>
struct PreSum1D {
    vector<T> pre;
    int n;

    PreSum1D(const vector<T>& v) {
        n = v.size();
        if (n == 0) return;
        pre.assign(n, 0);
        
        pre[0] = v[0];
        for (int i = 1; i < n; ++i) {
            pre[i] = pre[i - 1] + v[i];
        }
    }

    // 查询闭区间 [l, r]
    T query(int l, int r) {
        l = max(0, l); r = min(n - 1, r); // 边界保护（手速版可删）
        if (l > r) return 0;
        
        if (l == 0) return pre[r];
        return pre[r] - pre[l - 1];
    }
};

// --- 2. 二维前缀和 (0-based) ---
// 特点：pre[0][0] = v[0][0]，查询时需要判四个边界
template <typename T>
struct PreSum2D {
    vector<vector<T>> pre;
    int n, m;

    PreSum2D(const vector<vector<T>>& grid) {
        if (grid.empty()) return;
        n = grid.size();
        m = grid[0].size();
        pre.assign(n, vector<T>(m, 0));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                pre[i][j] = grid[i][j];
                if (i > 0) pre[i][j] += pre[i - 1][j];
                if (j > 0) pre[i][j] += pre[i][j - 1];
                if (i > 0 && j > 0) pre[i][j] -= pre[i - 1][j - 1];
            }
        }
    }

    // 查询子矩形 (x1, y1) ~ (x2, y2)
    T query(int x1, int y1, int x2, int y2) {
        x1 = max(0, x1); y1 = max(0, y1); // 边界保护
        x2 = min(n - 1, x2); y2 = min(m - 1, y2);
        if (x1 > x2 || y1 > y2) return 0;

        T res = pre[x2][y2];
        if (x1 > 0) res -= pre[x1 - 1][y2];
        if (y1 > 0) res -= pre[x2][y1 - 1];
        if (x1 > 0 && y1 > 0) res += pre[x1 - 1][y1 - 1];
        return res;
    }
};


int main() {
    // === 1D 测试 ===
    vector<ll> arr = {1, 2, 3, 4, 5}; // 原始数组
    PreSum1D<ll> ps1(arr);
    
    // 查询 [1, 3] => 2 + 3 + 4 = 9
    cout << "1D Query [1, 3]: " << ps1.query(1, 3) << "\n"; 
    // 查询越界 [3, 100] => 4 + 5 = 9 (自动截断)
    cout << "1D Query [3, 100]: " << ps1.query(3, 100) << "\n";
    // === 2D 测试 ===
    vector<vector<ll>> matrix = {
        {1, 1, 1},
        {1, 2, 1},
        {1, 1, 1}
    }; // 3x3 矩阵，中间是2，其余是1
    PreSum2D<ll> ps2(matrix);
    // 查询全图 [0,0] ~ [2,2] => sum = 10
    cout << "2D Query Full: " << ps2.query(0, 0, 2, 2) << "\n";
    // 查询右下角 2x2: [1,1] ~ [2,2] 
    // 2 1
    // 1 1  => sum = 5
    cout << "2D Query Sub: " << ps2.query(1, 1, 2, 2) << "\n";
    return 0;
}