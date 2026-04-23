#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  【状压DP】
  
  ⭐⭐ 省赛中低频率，难度较高
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "排列"、"全排列"
     • "选择子集"
     • "旅行商问题 TSP"
     • "n ≤ 20"（关键特征！）
  
  ✅ 核心特征：
     • 需要记录"哪些元素被选了"
     • n很小（≤20），因为状态数是 2^n
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 状态数：O(2^n)
  • 总复杂度：通常 O(n² × 2^n) 或 O(n × 2^n)
  • 适用：n ≤ 20（2^20 ≈ 1e6）
  
  ═══════════════════════════════════════════════════════════
  【位运算技巧】
  
  • 判断第i位是否为1：(mask >> i) & 1 或 mask & (1 << i)
  • 将第i位设为1：mask | (1 << i)
  • 将第i位设为0：mask & ~(1 << i)
  • 统计1的个数：__builtin_popcount(mask)
  • 枚举子集：for (int sub = mask; sub; sub = (sub - 1) & mask)
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 旅行商问题 TSP
  • 最短哈密顿路径
  • 铺砖问题
  • 工作分配问题
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】旅行商问题 TSP
// ═══════════════════════════════════════════════════════════
struct TSP {
    // 从0出发，经过所有点恰好一次，回到0的最短路径
    
    int n;
    vector<vector<int>> dist;
    vector<vector<int>> dp;  // dp[mask][i] = 已访问集合为mask，当前在i的最短距离
    
    TSP(int n, vector<vector<int>>& d) : n(n), dist(d) {}
    
    int solve() {
        int full = (1 << n) - 1;  // 所有点都访问的状态
        
        // 【初始化】
        dp.assign(1 << n, vector<int>(n, INT_MAX));
        dp[1][0] = 0;  // 从点0出发，初始状态只访问了点0
        
        // 【枚举所有状态】
        for (int mask = 1; mask <= full; mask++) {
            // 【枚举当前所在点】
            for (int u = 0; u < n; u++) {
                // u必须在mask中
                if (!(mask & (1 << u))) continue;
                if (dp[mask][u] == INT_MAX) continue;
                
                // 【枚举下一个点】
                for (int v = 0; v < n; v++) {
                    // v不能在mask中（未访问）
                    if (mask & (1 << v)) continue;
                    
                    int newMask = mask | (1 << v);
                    dp[newMask][v] = min(dp[newMask][v], dp[mask][u] + dist[u][v]);
                }
            }
        }
        
        // 【最后回到起点0】
        int ans = INT_MAX;
        for (int u = 0; u < n; u++) {
            if (dp[full][u] != INT_MAX) {
                ans = min(ans, dp[full][u] + dist[u][0]);
            }
        }
        
        return ans == INT_MAX ? -1 : ans;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】最短哈密顿路径（不需要回到起点）
// ═══════════════════════════════════════════════════════════
struct ShortestHamiltonianPath {
    int n;
    vector<vector<int>> dist;
    vector<vector<int>> dp;
    
    ShortestHamiltonianPath(int n, vector<vector<int>>& d) : n(n), dist(d) {}
    
    int solve() {
        int full = (1 << n) - 1;
        dp.assign(1 << n, vector<int>(n, INT_MAX));
        
        // 【可以从任意点出发】
        for (int i = 0; i < n; i++) {
            dp[1 << i][i] = 0;
        }
        
        for (int mask = 1; mask <= full; mask++) {
            for (int u = 0; u < n; u++) {
                if (!(mask & (1 << u))) continue;
                if (dp[mask][u] == INT_MAX) continue;
                
                for (int v = 0; v < n; v++) {
                    if (mask & (1 << v)) continue;
                    int newMask = mask | (1 << v);
                    dp[newMask][v] = min(dp[newMask][v], dp[mask][u] + dist[u][v]);
                }
            }
        }
        
        // 【在任意点结束】
        int ans = INT_MAX;
        for (int u = 0; u < n; u++) {
            ans = min(ans, dp[full][u]);
        }
        return ans == INT_MAX ? -1 : ans;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】工作分配问题
// ═══════════════════════════════════════════════════════════
struct JobAssignment {
    // n个人做n个工作，第i个人做第j个工作的代价是cost[i][j]
    // 每人做一个工作，每个工作被做一次，求最小总代价
    
    int n;
    vector<vector<int>> cost;
    vector<int> dp;  // dp[mask] = 前popcount(mask)个人做了mask这些工作的最小代价
    
    JobAssignment(int n, vector<vector<int>>& c) : n(n), cost(c) {}
    
    int solve() {
        dp.assign(1 << n, INT_MAX);
        dp[0] = 0;  // 没人做任何工作，代价0
        
        for (int mask = 0; mask < (1 << n); mask++) {
            if (dp[mask] == INT_MAX) continue;
            
            int person = __builtin_popcount(mask);  // 下一个要分配工作的人
            if (person >= n) continue;
            
            // 给第person个人分配一个未被占用的工作
            for (int job = 0; job < n; job++) {
                if (mask & (1 << job)) continue;  // 工作已被占用
                
                int newMask = mask | (1 << job);
                dp[newMask] = min(dp[newMask], dp[mask] + cost[person][job]);
            }
        }
        
        return dp[(1 << n) - 1];
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板4】子集枚举
// ═══════════════════════════════════════════════════════════
struct SubsetEnumeration {
    // 枚举一个集合的所有子集
    
    void enumerate(int mask) {
        cout << "枚举 " << mask << " 的所有子集: ";
        
        // 【技巧】从mask开始，每次减1再与mask取与
        for (int sub = mask; ; sub = (sub - 1) & mask) {
            cout << sub << " ";
            if (sub == 0) break;  // 空集也要处理
        }
        cout << "\n";
    }
    
    // 枚举大小为k的子集
    void enumerateKSubsets(int n, int k) {
        cout << "枚举 " << n << " 个元素中大小为 " << k << " 的子集: ";
        
        // 【Gosper's Hack】
        int mask = (1 << k) - 1;  // 最小的k位子集
        while (mask < (1 << n)) {
            cout << mask << " ";
            
            int c = mask & -mask;       // 最低位的1
            int r = mask + c;           // 把最低连续1进位
            mask = (((r ^ mask) >> 2) / c) | r;  // 补回低位的1
        }
        cout << "\n";
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 状压DP模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：TSP
    // 4个城市，距离矩阵：
    //     0  1  2  3
    // 0 [ 0 10 15 20]
    // 1 [10  0 35 25]
    // 2 [15 35  0 30]
    // 3 [20 25 30  0]
    // 最优路线：0→1→3→2→0 = 10+25+30+15 = 80
    // ─────────────────────────────────────────────────────
    {
        vector<vector<int>> dist = {
            {0, 10, 15, 20},
            {10, 0, 35, 25},
            {15, 35, 0, 30},
            {20, 25, 30, 0}
        };
        TSP solver(4, dist);
        
        assert(solver.solve() == 80);
        cout << "[TSP] 测试通过，最短路径=" << solver.solve() << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：工作分配
    // 3个人3个工作，代价矩阵：
    //     工作0 工作1 工作2
    // 人0 [  9    2    7  ]
    // 人1 [  6    4    3  ]
    // 人2 [  5    8    1  ]
    // 最优：人0→工作1(2), 人1→工作0(6), 人2→工作2(1) = 9
    // ─────────────────────────────────────────────────────
    {
        vector<vector<int>> cost = {
            {9, 2, 7},
            {6, 4, 3},
            {5, 8, 1}
        };
        JobAssignment solver(3, cost);
        
        assert(solver.solve() == 9);
        cout << "[工作分配] 测试通过，最小代价=" << solver.solve() << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：子集枚举
    // ─────────────────────────────────────────────────────
    {
        SubsetEnumeration se;
        se.enumerate(0b1011);  // 11的子集
        se.enumerateKSubsets(4, 2);  // 4个元素中选2个
    }
    
    cout << "\n===== 所有状压DP测试通过 =====\n";
    return 0;
}