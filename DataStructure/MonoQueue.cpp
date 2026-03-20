#include<bits/stdc++.h>
using ll = long long;
using ull = unsigned long long;
using u128 = unsigned __int128;
using i128 = __int128;
using namespace std;
const int INF = 0x3f3f3f3f;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "滑动窗口最大值/最小值"
  ✅ 题目特征:
     • 固定/动态窗口内求最值
     • 窗口大小k，数组长度n≥10⁵
  
  应用场景:
  • 滑动窗口最大值/最小值（单调递减/递增队列）
  • 单调队列优化DP
  
  典型题:
  • "滑动窗口最大值"
  • "每个长度为k的子数组的最小值"
  
  ❌ 反例: 窗口内求和 → 前缀和
=============================================================*/
struct MonoQueue {
    static const int MAXN = 1e6 + 10; 
    int q[MAXN];  
    int h, t;
    
    vector<int> ma(const vector<int>& nums, int k) {
        vector<int> r; h = 0; t = -1;
        for (int i = 0; i < nums.size(); i++) {
            while (h <= t && nums[q[t]] <= nums[i]) t--;
            q[++t] = i;
            if (q[h] <= i - k) h++;
            if (i >= k - 1) r.push_back(nums[q[h]]);
        }
        return r;
    }

    vector<int> mi(const vector<int>& nums, int k) {
        vector<int> r; h = 0; t = -1;
        for (int i = 0; i < nums.size(); i++) {
            while (h <= t && nums[q[t]] >= nums[i]) t--;  
            q[++t] = i;
            if (q[h] <= i - k) h++;
            if (i >= k - 1) r.push_back(nums[q[h]]);
        }
        return r;
    }

    vector<vector<int>> ma_2d(const vector<vector<int>>& mat, int a, int b) {
        int n = mat.size(); if(!n) return {};
        int m = mat[0].size();
        vector<vector<int>> row(n, vector<int>(m));
        for (int i = 0; i < n; i++) {
            vector<int> w = ma(mat[i], b);
            for (int j = 0; j < w.size(); j++) {
                row[i][j + b - 1] = w[j];
            }
        }
        vector<vector<int>> r(n, vector<int>(m));
        for (int j = b - 1; j < m; j++) { 
            vector<int> col; 
            for (int i = 0; i < n; i++) col.push_back(row[i][j]);
            
            vector<int> w = ma(col, a);
            for (int i = 0; i < w.size(); i++) {
                r[i + a - 1][j] = w[i];
            }
        }
        return r;
    }

    vector<vector<int>> mi_2d(const vector<vector<int>>& mat, int a, int b) {
        int n = mat.size(); if(!n) return {};
        int m = mat[0].size();
        
        vector<vector<int>> row(n, vector<int>(m));
        for (int i = 0; i < n; i++) {
            vector<int> w = mi(mat[i], b); // 注意调用 mi
            for (int j = 0; j < w.size(); j++) {
                row[i][j + b - 1] = w[j];
            }
        }
        vector<vector<int>> r(n, vector<int>(m));
        for (int j = b - 1; j < m; j++) {
            vector<int> col; 
            for (int i = 0; i < n; i++) col.push_back(row[i][j]);
            vector<int> w = mi(col, a); // 注意调用 mi
            for (int i = 0; i < w.size(); i++) {
                r[i + a - 1][j] = w[i];
            }
        }
        return r;
    }
} mq;

void test() {
    // 数组: [1, 3, -1, -3, 5, 3, 6, 7], k=3
    // 窗口: [1,3,-1], [3,-1,-3], [-1,-3,5], [-3,5,3], [5,3,6], [3,6,7]
    // 最大值: 3, 3, 5, 5, 6, 7
    vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    vector<int> res1 = mq.ma(nums, 3);
    assert(res1[0] == 3);
    assert(res1[5] == 7);
    cout << "1D Test Passed!" << endl;

    // 3x3 矩阵, 找 2x2 最大值
    // 1 2 3
    // 4 5 6
    // 7 8 9
    vector<vector<int>> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    vector<vector<int>> res2 = mq.ma_2d(mat, 2, 2);
    assert(res2[1][1] == 5); // 左上那个 2x2
    assert(res2[1][2] == 6);
    assert(res2[2][1] == 8);
    assert(res2[2][2] == 9);
    cout << "2D Test Passed!" << endl;
}

signed main() {
	cin.tie(nullptr)->sync_with_stdio(false);	
	test(); 
}



