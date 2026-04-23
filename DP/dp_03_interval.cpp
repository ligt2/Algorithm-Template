#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  【区间DP】
  
  ⭐⭐⭐ 省赛中等频率
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "合并"、"分割"
     • "区间"、"子串"
     • "回文"
     • "括号匹配"
     • "戳气球"
  
  ✅ 核心特征：
     • 大区间的最优解依赖于小区间
     • 需要枚举"分割点/合并点"
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 时间：O(n³)（三重循环：区间长度、起点、分割点）
  • 空间：O(n²)
  • 适用：n ≤ 500
  
  ═══════════════════════════════════════════════════════════
  【代码框架】
  
  for (int len = 2; len <= n; len++) {        // 枚举区间长度
      for (int i = 0; i + len - 1 < n; i++) { // 枚举起点
          int j = i + len - 1;                 // 计算终点
          for (int k = i; k < j; k++) {        // 枚举分割点
              dp[i][j] = min/max(dp[i][j], dp[i][k] + dp[k+1][j] + cost);
          }
      }
  }
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 石子合并（相邻合并）
  • 矩阵链乘法
  • 最长回文子序列
  • 戳气球
  • 最优三角剖分
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】石子合并 - 相邻合并，求最小/最大代价
// ═══════════════════════════════════════════════════════════
struct MergeStones {
    vector<vector<int>> dpMin;  // 最小代价
    vector<vector<int>> dpMax;  // 最大代价
    vector<int> prefix;          // 前缀和（快速计算区间和）
    
    // 【核心思想】
    // dp[i][j] = 合并区间[i,j]的最小代价
    // 转移：枚举分割点k，先合并[i,k]和[k+1,j]，再把两堆合并
    // 合并代价 = sum(i, j)
    
    pair<int, int> solve(vector<int>& a) {
        int n = a.size();
        if (n == 1) return {0, 0};
        
        // 【前缀和预处理】
        prefix.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + a[i];
        }
        
        // 【初始化】
        dpMin.assign(n, vector<int>(n, INT_MAX));
        dpMax.assign(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            dpMin[i][i] = 0;  // 单个石子，合并代价为0
            dpMax[i][i] = 0;
        }
        
        // 【枚举区间长度】从小到大
        for (int len = 2; len <= n; len++) {
            // 【枚举起点】
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;  // 终点
                int cost = prefix[j + 1] - prefix[i];  // 区间和
                
                // 【枚举分割点】
                for (int k = i; k < j; k++) {
                    dpMin[i][j] = min(dpMin[i][j], dpMin[i][k] + dpMin[k+1][j] + cost);
                    dpMax[i][j] = max(dpMax[i][j], dpMax[i][k] + dpMax[k+1][j] + cost);
                }
            }
        }
        
        return {dpMin[0][n-1], dpMax[0][n-1]};
    }
    
    // 辅助：计算区间和
    int rangeSum(int i, int j) {
        return prefix[j + 1] - prefix[i];
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】环形石子合并（首尾相连）
// ═══════════════════════════════════════════════════════════
struct MergeStonesCircle {
    // 【技巧】破环为链：将数组复制一份接在后面
    // 例：[1,2,3] → [1,2,3,1,2,3]
    // 答案 = min(dp[0][n-1], dp[1][n], ..., dp[n-1][2n-2])
    
    pair<int, int> solve(vector<int>& a) {
        int n = a.size();
        if (n == 1) return {0, 0};
        
        // 【破环为链】
        vector<int> b(2 * n);
        for (int i = 0; i < n; i++) {
            b[i] = b[i + n] = a[i];
        }
        
        // 前缀和
        vector<int> prefix(2 * n + 1, 0);
        for (int i = 0; i < 2 * n; i++) {
            prefix[i + 1] = prefix[i] + b[i];
        }
        
        // 区间DP
        int m = 2 * n;
        vector<vector<int>> dpMin(m, vector<int>(m, INT_MAX));
        vector<vector<int>> dpMax(m, vector<int>(m, 0));
        for (int i = 0; i < m; i++) {
            dpMin[i][i] = 0;
            dpMax[i][i] = 0;
        }
        
        for (int len = 2; len <= n; len++) {  // 注意：长度最多n
            for (int i = 0; i + len - 1 < m; i++) {
                int j = i + len - 1;
                int cost = prefix[j + 1] - prefix[i];
                
                for (int k = i; k < j; k++) {
                    dpMin[i][j] = min(dpMin[i][j], dpMin[i][k] + dpMin[k+1][j] + cost);
                    dpMax[i][j] = max(dpMax[i][j], dpMax[i][k] + dpMax[k+1][j] + cost);
                }
            }
        }
        
        // 【取所有起点的最优值】
        int ansMin = INT_MAX, ansMax = 0;
        for (int i = 0; i < n; i++) {
            ansMin = min(ansMin, dpMin[i][i + n - 1]);
            ansMax = max(ansMax, dpMax[i][i + n - 1]);
        }
        
        return {ansMin, ansMax};
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】最长回文子序列
// ═══════════════════════════════════════════════════════════
struct LongestPalindromeSubseq {
    // dp[i][j] = s[i..j]的最长回文子序列长度
    
    int solve(string& s) {
        int n = s.size();
        if (n == 0) return 0;
        
        vector<vector<int>> dp(n, vector<int>(n, 0));
        
        // 【初始化】单个字符是长度1的回文
        for (int i = 0; i < n; i++) {
            dp[i][i] = 1;
        }
        
        // 【从短到长枚举】或【从后往前枚举i】
        for (int i = n - 1; i >= 0; i--) {
            for (int j = i + 1; j < n; j++) {
                if (s[i] == s[j]) {
                    // 两端字符相同，加入回文
                    dp[i][j] = dp[i+1][j-1] + 2;
                } else {
                    // 两端不同，取跳过一端的最大值
                    dp[i][j] = max(dp[i+1][j], dp[i][j-1]);
                }
            }
        }
        
        return dp[0][n-1];
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板4】最长回文子串（连续）
// ═══════════════════════════════════════════════════════════
struct LongestPalindromeSubstr {
    string solve(string& s) {
        int n = s.size();
        if (n == 0) return "";
        
        // dp[i][j] = s[i..j]是否为回文
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        int start = 0, maxLen = 1;
        
        // 初始化：单个字符
        for (int i = 0; i < n; i++) dp[i][i] = true;
        
        // 初始化：相邻两个字符
        for (int i = 0; i + 1 < n; i++) {
            if (s[i] == s[i+1]) {
                dp[i][i+1] = true;
                start = i;
                maxLen = 2;
            }
        }
        
        // 长度从3开始
        for (int len = 3; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                if (s[i] == s[j] && dp[i+1][j-1]) {
                    dp[i][j] = true;
                    start = i;
                    maxLen = len;
                }
            }
        }
        
        return s.substr(start, maxLen);
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板5】矩阵链乘法
// ═══════════════════════════════════════════════════════════
struct MatrixChainMultiplication {
    // 矩阵 A_i 的维度为 dims[i-1] × dims[i]
    // 求最少乘法次数
    
    ll solve(vector<int>& dims) {
        int n = dims.size() - 1;  // 矩阵个数
        if (n <= 1) return 0;
        
        // dp[i][j] = 计算 A_i × A_{i+1} × ... × A_j 的最少乘法次数
        vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, 0));
        
        for (int len = 2; len <= n; len++) {
            for (int i = 1; i + len - 1 <= n; i++) {
                int j = i + len - 1;
                dp[i][j] = LLONG_MAX;
                
                for (int k = i; k < j; k++) {
                    // 分成 [i,k] 和 [k+1,j] 两部分
                    // 合并代价 = dims[i-1] * dims[k] * dims[j]
                    ll cost = dp[i][k] + dp[k+1][j] + 
                              (ll)dims[i-1] * dims[k] * dims[j];
                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }
        
        return dp[1][n];
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 区间DP模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：石子合并
    // 石子：[1, 3, 5, 2]
    // 最优合并顺序：(1+3)+(5+2)=4+7=11, 然后4+7+11=22
    // 或者更优的方式...
    // ─────────────────────────────────────────────────────
    {
        vector<int> stones = {1, 3, 5, 2};
        MergeStones solver;
        auto [minCost, maxCost] = solver.solve(stones);
        
        cout << "[石子合并] 最小代价=" << minCost << ", 最大代价=" << maxCost << "\n";
        // 验证：人工计算最小代价应该是22
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：环形石子合并
    // ─────────────────────────────────────────────────────
    {
        vector<int> stones = {4, 5, 9, 4};
        MergeStonesCircle solver;
        auto [minCost, maxCost] = solver.solve(stones);
        
        cout << "[环形石子合并] 最小代价=" << minCost << ", 最大代价=" << maxCost << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：最长回文子序列
    // "bbbab" → "bbbb"，长度4
    // ─────────────────────────────────────────────────────
    {
        string s = "bbbab";
        LongestPalindromeSubseq solver;
        
        assert(solver.solve(s) == 4);
        cout << "[最长回文子序列] 测试通过，长度=" << solver.solve(s) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试4：最长回文子串
    // "babad" → "bab" 或 "aba"
    // ─────────────────────────────────────────────────────
    {
        string s = "babad";
        LongestPalindromeSubstr solver;
        string result = solver.solve(s);
        
        assert(result == "bab" || result == "aba");
        cout << "[最长回文子串] 测试通过，结果=\"" << result << "\"\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试5：矩阵链乘法
    // 矩阵维度：[10, 30, 5, 60]
    // 表示3个矩阵：A1(10×30), A2(30×5), A3(5×60)
    // 最优：(A1×A2)×A3 = 10×30×5 + 10×5×60 = 1500 + 3000 = 4500
    // ─────────────────────────────────────────────────────
    {
        vector<int> dims = {10, 30, 5, 60};
        MatrixChainMultiplication solver;
        
        assert(solver.solve(dims) == 4500);
        cout << "[矩阵链乘法] 测试通过，最少乘法次数=" << solver.solve(dims) << "\n";
    }
    
    cout << "\n===== 所有区间DP测试通过 =====\n";
    return 0;
}