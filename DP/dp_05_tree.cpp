#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  【树形DP】
  
  ⭐⭐ 省赛中低频率，但难度较高
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "树上选择"
     • "树的直径"、"树的中心"
     • "选择节点使得相邻节点不同时选"
     • "树上背包"
  
  ✅ 核心特征：
     • 问题定义在树结构上
     • 子树的答案可以合并得到父节点的答案
  
  ═══════════════════════════════════════════════════════════
  【代码框架】
  
  void dfs(int u, int parent) {
      // 初始化dp[u]
      for (int v : children[u]) {
          if (v == parent) continue;
          dfs(v, u);
          // 用dp[v]更新dp[u]
      }
  }
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 树的直径
  • 树的最大独立集
  • 树上最长路径
  • 选课问题（树上背包）
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】树的直径（边权）
// ═══════════════════════════════════════════════════════════
struct TreeDiameter {
    vector<vector<pair<int, int>>> adj;  // adj[u] = {(v, w), ...}
    int n;
    int diameter;
    
    TreeDiameter(int n) : n(n), adj(n + 1), diameter(0) {}
    
    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    
    // 【DFS】返回从u出发向下的最长路径
    int dfs(int u, int parent) {
        int max1 = 0, max2 = 0;  // 最长和次长
        
        for (auto [v, w] : adj[u]) {
            if (v == parent) continue;
            
            int d = dfs(v, u) + w;
            
            // 更新最长和次长
            if (d > max1) {
                max2 = max1;
                max1 = d;
            } else if (d > max2) {
                max2 = d;
            }
        }
        
        // 【关键】经过u的最长路径 = max1 + max2
        diameter = max(diameter, max1 + max2);
        
        return max1;  // 返回向下的最长路径
    }
    
    int solve() {
        diameter = 0;
        dfs(1, -1);
        return diameter;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】树的最大独立集（没有边权的版本）
// ═══════════════════════════════════════════════════════════
struct MaxIndependentSet {
    // 选择若干节点，使得任意两个选中的节点不相邻，求最大点数
    
    vector<vector<int>> adj;
    vector<int> dp0;  // dp0[u] = 不选u时，u子树的最大独立集
    vector<int> dp1;  // dp1[u] = 选u时，u子树的最大独立集
    int n;
    
    MaxIndependentSet(int n) : n(n), adj(n + 1), dp0(n + 1, 0), dp1(n + 1, 0) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void dfs(int u, int parent) {
        dp0[u] = 0;
        dp1[u] = 1;  // 选u，至少有u自己
        
        for (int v : adj[u]) {
            if (v == parent) continue;
            
            dfs(v, u);
            
            // 【转移】
            dp0[u] += max(dp0[v], dp1[v]);  // 不选u，子节点随意
            dp1[u] += dp0[v];                // 选u，子节点不能选
        }
    }
    
    int solve() {
        dfs(1, -1);
        return max(dp0[1], dp1[1]);
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】树的最大权独立集（带点权）
// ═══════════════════════════════════════════════════════════
struct MaxWeightIndependentSet {
    vector<vector<int>> adj;
    vector<ll> val;   // 节点权值
    vector<ll> dp0;   // 不选u
    vector<ll> dp1;   // 选u
    int n;
    
    MaxWeightIndependentSet(int n, vector<ll>& values) 
        : n(n), adj(n + 1), val(values), dp0(n + 1, 0), dp1(n + 1, 0) {
        val.insert(val.begin(), 0);  // 1-indexed
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void dfs(int u, int parent) {
        dp0[u] = 0;
        dp1[u] = val[u];
        
        for (int v : adj[u]) {
            if (v == parent) continue;
            dfs(v, u);
            dp0[u] += max(dp0[v], dp1[v]);
            dp1[u] += dp0[v];
        }
    }
    
    ll solve() {
        dfs(1, -1);
        return max(dp0[1], dp1[1]);
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板4】树的重心
// ═══════════════════════════════════════════════════════════
struct TreeCentroid {
    vector<vector<int>> adj;
    vector<int> sz;    // sz[u] = u子树的大小
    int n;
    int centroid;
    int minMaxPart;    // 删除重心后，最大连通块的大小
    
    TreeCentroid(int n) : n(n), adj(n + 1), sz(n + 1, 0) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void dfs(int u, int parent) {
        sz[u] = 1;
        int maxPart = 0;  // 删除u后最大连通块大小
        
        for (int v : adj[u]) {
            if (v == parent) continue;
            dfs(v, u);
            sz[u] += sz[v];
            maxPart = max(maxPart, sz[v]);
        }
        
        // 【关键】还要考虑"上方"的连通块
        maxPart = max(maxPart, n - sz[u]);
        
        if (maxPart < minMaxPart) {
            minMaxPart = maxPart;
            centroid = u;
        }
    }
    
    int solve() {
        minMaxPart = INT_MAX;
        dfs(1, -1);
        return centroid;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板5】树上背包（选课问题）
// ═══════════════════════════════════════════════════════════
struct TreeKnapsack {
    // 每门课有学分，选某课必须先选先修课（形成树）
    // 最多选m门课，求最大学分
    
    vector<vector<int>> children;
    vector<int> score;        // 每门课的学分
    vector<vector<ll>> dp;    // dp[u][j] = u子树中选j门课的最大学分
    int n, m;
    
    TreeKnapsack(int n, int m, vector<int>& s) 
        : n(n), m(m), children(n + 1), score(s), dp(n + 1, vector<ll>(m + 2, 0)) {
        score.insert(score.begin(), 0);  // 1-indexed
    }
    
    void addEdge(int parent, int child) {
        children[parent].push_back(child);
    }
    
    void dfs(int u) {
        dp[u][1] = score[u];  // 只选u自己
        
        for (int v : children[u]) {
            dfs(v);
            
            // 【树上背包合并】倒序枚举
            for (int j = m + 1; j >= 1; j--) {
                for (int k = 0; k < j; k++) {
                    // 从u子树选j门，其中v子树选k门
                    dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[v][k]);
                }
            }
        }
    }
    
    ll solve() {
        // 添加虚拟根节点0，所有无先修课的课程挂在0下
        dfs(0);
        return dp[0][m + 1];  // +1因为包含虚拟根
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 树形DP模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：树的直径
    //     1
    //    /|\
    //   2 3 4
    //   |
    //   5
    // 边权都为1，直径 = 5→2→1→3 或 5→2→1→4 = 3
    // ─────────────────────────────────────────────────────
    {
        TreeDiameter tree(5);
        tree.addEdge(1, 2, 1);
        tree.addEdge(1, 3, 1);
        tree.addEdge(1, 4, 1);
        tree.addEdge(2, 5, 1);
        
        assert(tree.solve() == 3);
        cout << "[树的直径] 测试通过，直径=" << tree.solve() << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：最大独立集
    //     1
    //    / \
    //   2   3
    //  / \
    // 4   5
    // 最大独立集: {1,4,5} 或 {3,4,5} 或 {2,3} → 大小=3
    // ─────────────────────────────────────────────────────
    {
        MaxIndependentSet tree(5);
        tree.addEdge(1, 2);
        tree.addEdge(1, 3);
        tree.addEdge(2, 4);
        tree.addEdge(2, 5);
        
        assert(tree.solve() == 3);
        cout << "[最大独立集] 测试通过，大小=" << tree.solve() << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：树的重心
    //     1
    //    /|\
    //   2 3 4
    //   |
    //   5
    // 删除1后，最大连通块为2（节点2,5），其他删法更差
    // 重心是1
    // ─────────────────────────────────────────────────────
    {
        TreeCentroid tree(5);
        tree.addEdge(1, 2);
        tree.addEdge(1, 3);
        tree.addEdge(1, 4);
        tree.addEdge(2, 5);
        
        assert(tree.solve() == 1);
        cout << "[树的重心] 测试通过，重心=" << tree.solve() << "\n";
    }
    
    cout << "\n===== 所有树形DP测试通过 =====\n";
    return 0;
}