#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  【线性DP - LIS / LCS / 最大子段和 / 递推】
  
  ⭐⭐⭐⭐ 省赛高频
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ LIS（最长上升子序列）信号词：
     • "最长递增子序列"
     • "最少删除几个元素使得有序"
     • "导弹拦截"（经典题）
  
  ✅ LCS（最长公共子序列）信号词：
     • "两个字符串的最长公共子序列"
     • "编辑距离"（变体）
  
  ✅ 最大子段和信号词：
     • "连续子数组的最大和"
     • "最大子矩阵和"（二维扩展）
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • LIS：O(n²) 朴素版，O(nlogn) 二分优化版（推荐）
  • LCS：O(nm)
  • 最大子段和：O(n)
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 蓝桥杯：导弹拦截、合唱队形
  • LeetCode：300最长递增子序列、1143最长公共子序列、53最大子数组和
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】LIS - O(n²) 朴素版（理解用）
// ═══════════════════════════════════════════════════════════
struct LIS_N2 {
    vector<int> dp;  // dp[i] = 以a[i]结尾的LIS长度
    int result;
    
    // 参数：数组a
    // 返回：LIS长度
    int solve(vector<int>& a) {
        int n = a.size();
        if (n == 0) return 0;
        
        dp.assign(n, 1);  // 【初始化】每个元素自身就是长度1的LIS
        result = 1;
        
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                // 【转移】如果a[j] < a[i]，可以接在j后面
                if (a[j] < a[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            result = max(result, dp[i]);
        }
        return result;
    }
    
    // ⚠️ 修改点：
    // • 最长不下降：a[j] < a[i] 改为 a[j] <= a[i]
    // • 最长下降：a[j] < a[i] 改为 a[j] > a[i]
};

// ═══════════════════════════════════════════════════════════
// 【模板2】LIS - O(nlogn) 二分优化版（实战推荐）⭐
// ═══════════════════════════════════════════════════════════
struct LIS_NLogN {
    vector<int> tail;  // tail[i] = 长度为i+1的LIS的最小末尾元素
    
    // 【核心原理】
    // tail数组始终保持单调递增
    // 新元素x：
    //   - 如果x > tail.back()，直接追加（LIS变长）
    //   - 否则，用x替换第一个>=x的位置（保持最小末尾）
    
    int solve(vector<int>& a) {
        tail.clear();
        
        for (int x : a) {
            // lower_bound：找第一个 >= x 的位置
            auto it = lower_bound(tail.begin(), tail.end(), x);
            
            if (it == tail.end()) {
                tail.push_back(x);  // x比所有元素都大，追加
            } else {
                *it = x;  // 替换，保持最小末尾
            }
        }
        return tail.size();
    }
    
    // 【变体】最长不下降子序列（允许相等）
    int solveLNDS(vector<int>& a) {
        tail.clear();
        for (int x : a) {
            // upper_bound：找第一个 > x 的位置（允许相等）
            auto it = upper_bound(tail.begin(), tail.end(), x);
            if (it == tail.end()) {
                tail.push_back(x);
            } else {
                *it = x;
            }
        }
        return tail.size();
    }
    
    // 【变体】最长下降子序列
    int solveLDS(vector<int>& a) {
        tail.clear();
        for (int x : a) {
            // 用greater比较，维护单调递减
            auto it = lower_bound(tail.begin(), tail.end(), x, greater<int>());
            if (it == tail.end()) {
                tail.push_back(x);
            } else {
                *it = x;
            }
        }
        return tail.size();
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】LCS - 最长公共子序列
// ═══════════════════════════════════════════════════════════
struct LCS {
    vector<vector<int>> dp;  // dp[i][j] = a[0..i-1]和b[0..j-1]的LCS长度
    
    // 参数：两个字符串a, b
    // 返回：LCS长度
    int solve(string& a, string& b) {
        int n = a.size(), m = b.size();
        
        // 【初始化】dp[0][...] = dp[...][0] = 0（空串的LCS为0）
        dp.assign(n + 1, vector<int>(m + 1, 0));
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (a[i-1] == b[j-1]) {
                    // 【转移1】字符相同，LCS+1
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    // 【转移2】字符不同，取两种跳过方式的最大值
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        return dp[n][m];
    }
    
    // 【获取LCS字符串】（回溯）
    string getLCS(string& a, string& b) {
        int i = a.size(), j = b.size();
        string result;
        
        while (i > 0 && j > 0) {
            if (a[i-1] == b[j-1]) {
                result += a[i-1];
                i--; j--;
            } else if (dp[i-1][j] > dp[i][j-1]) {
                i--;
            } else {
                j--;
            }
        }
        reverse(result.begin(), result.end());
        return result;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板4】LCS - 空间优化版（省空间）
// ═══════════════════════════════════════════════════════════
struct LCS_Optimized {
    int solve(string& a, string& b) {
        int n = a.size(), m = b.size();
        vector<int> dp(m + 1, 0);
        
        for (int i = 1; i <= n; i++) {
            int prev = 0;  // 保存dp[i-1][j-1]
            for (int j = 1; j <= m; j++) {
                int temp = dp[j];  // 保存当前值（下一轮的prev）
                if (a[i-1] == b[j-1]) {
                    dp[j] = prev + 1;
                } else {
                    dp[j] = max(dp[j], dp[j-1]);
                }
                prev = temp;
            }
        }
        return dp[m];
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板5】最大子段和 - Kadane算法
// ═══════════════════════════════════════════════════════════
struct MaxSubarray {
    ll maxSum;
    int left, right;  // 最大子段的区间 [left, right]
    
    // 【核心思想】
    // cur = 以当前位置结尾的最大子段和
    // 如果cur < 0，不如重新开始
    
    ll solve(vector<int>& a) {
        int n = a.size();
        if (n == 0) return 0;
        
        ll cur = 0;
        maxSum = LLONG_MIN;
        int tempLeft = 0;
        
        for (int i = 0; i < n; i++) {
            if (cur < 0) {
                cur = a[i];      // 重新开始
                tempLeft = i;
            } else {
                cur += a[i];     // 继续累加
            }
            
            if (cur > maxSum) {
                maxSum = cur;
                left = tempLeft;
                right = i;
            }
        }
        return maxSum;
    }
    
    // 【简化版】只求最大和
    ll solveSimple(vector<int>& a) {
        ll cur = 0, ans = LLONG_MIN;
        for (int x : a) {
            cur = max((ll)x, cur + x);
            ans = max(ans, cur);
        }
        return ans;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板6】编辑距离
// ═══════════════════════════════════════════════════════════
struct EditDistance {
    // dp[i][j] = 将a[0..i-1]变成b[0..j-1]的最少操作数
    // 操作：插入、删除、替换
    
    int solve(string& a, string& b) {
        int n = a.size(), m = b.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        
        // 【初始化】
        for (int i = 0; i <= n; i++) dp[i][0] = i;  // 删除i个字符
        for (int j = 0; j <= m; j++) dp[0][j] = j;  // 插入j个字符
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (a[i-1] == b[j-1]) {
                    dp[i][j] = dp[i-1][j-1];  // 字符相同，无需操作
                } else {
                    dp[i][j] = min({
                        dp[i-1][j] + 1,    // 删除a[i-1]
                        dp[i][j-1] + 1,    // 插入b[j-1]
                        dp[i-1][j-1] + 1   // 替换
                    });
                }
            }
        }
        return dp[n][m];
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 线性DP模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：LIS O(n²)
    // 数组[10, 9, 2, 5, 3, 7, 101, 18]
    // LIS: [2, 3, 7, 101] 或 [2, 5, 7, 101] 等，长度4
    // ─────────────────────────────────────────────────────
    {
        vector<int> a = {10, 9, 2, 5, 3, 7, 101, 18};
        LIS_N2 solver;
        
        assert(solver.solve(a) == 4);
        cout << "[LIS-O(n²)] 测试通过，LIS长度=" << solver.solve(a) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：LIS O(nlogn)
    // ─────────────────────────────────────────────────────
    {
        vector<int> a = {10, 9, 2, 5, 3, 7, 101, 18};
        LIS_NLogN solver;
        
        assert(solver.solve(a) == 4);
        cout << "[LIS-O(nlogn)] 测试通过，LIS长度=" << solver.solve(a) << "\n";
        
        // 测试最长不下降
        vector<int> b = {1, 3, 3, 2, 4, 4, 5};
        assert(solver.solveLNDS(b) == 6);  // [1,3,3,4,4,5]
        cout << "[LNDS] 测试通过，长度=" << solver.solveLNDS(b) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：LCS
    // a="abcde", b="ace" → LCS="ace", 长度3
    // ─────────────────────────────────────────────────────
    {
        string a = "abcde", b = "ace";
        LCS solver;
        
        assert(solver.solve(a, b) == 3);
        assert(solver.getLCS(a, b) == "ace");
        cout << "[LCS] 测试通过，LCS=\"" << solver.getLCS(a, b) 
             << "\"，长度=" << solver.solve(a, b) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试4：最大子段和
    // 数组[-2, 1, -3, 4, -1, 2, 1, -5, 4]
    // 最大子段[4, -1, 2, 1]，和=6
    // ─────────────────────────────────────────────────────
    {
        vector<int> a = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        MaxSubarray solver;
        
        assert(solver.solve(a) == 6);
        cout << "[最大子段和] 测试通过，最大和=" << solver.maxSum 
             << "，区间[" << solver.left << "," << solver.right << "]\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试5：编辑距离
    // "horse" → "ros"：3步（删h, 替r→r, 删e）
    // ─────────────────────────────────────────────────────
    {
        string a = "horse", b = "ros";
        EditDistance solver;
        
        assert(solver.solve(a, b) == 3);
        cout << "[编辑距离] 测试通过，最少操作=" << solver.solve(a, b) << "\n";
    }
    
    cout << "\n===== 所有线性DP测试通过 =====\n";
    return 0;
}