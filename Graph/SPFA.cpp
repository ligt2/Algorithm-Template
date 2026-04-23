#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1e18;

/*=============================================================
  【SPFA算法 - 单源最短路（可处理负权边）】
  
  ⭐⭐⭐⭐ 省赛常用
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "有负权边的最短路"
     • "判断负环"
     • "差分约束系统"
  
  ✅ 使用场景：
     • 存在负权边（Dijkstra不能用）
     • 需要判断是否有负环
     • 差分约束问题
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 平均：O(kE)，k是常数，约为2
  • 最坏：O(VE)，在特殊构造的图上
  • ⚠️ 比赛中可能被卡，优先用Dijkstra
  
  ═══════════════════════════════════════════════════════════
  【与Dijkstra对比】
  
  ┌──────────────────────────────────────────────────────────┐
  │  条件              选择                                  │
  ├──────────────────────────────────────────────────────────┤
  │  非负权边          Dijkstra（更快更稳定）                │
  │  有负权边          SPFA（必须）                          │
  │  判负环            SPFA（必须）                          │
  └──────────────────────────────────────────────────────────┘
  
  ═══════════════════════════════════════════════════════════
  【核心原理】
  
  Bellman-Ford的队列优化版本：
  - 维护一个队列，存储"距离变小"的节点
  - 只有距离变小的节点才能更新其他节点
  - 判负环：某节点入队次数超过n次
  
  ═══════════════════════════════════════════════════════════
  【常见变体/修改点】
  
  1. 最长路：将min改max，初始化改为负无穷
  2. 差分约束：根据约束条件建图
  3. 判断从某点出发能否到达负环
  
=============================================================*/

struct SPFA {
    int n;                                // 节点数量
    vector<vector<pair<int, int>>> g;     // 邻接表
    vector<ll> dist;                      // 最短距离
    vector<int> cnt;                      // 入队次数（判负环）
    vector<bool> inq;                     // 是否在队列中
    bool has_neg_cycle = false;           // 是否存在负环

    // ═══════════════════════════════════════════════════════
    // 【构造函数】
    // ═══════════════════════════════════════════════════════
    SPFA(int n) : n(n), g(n + 1), dist(n + 1, INF), 
                  cnt(n + 1, 0), inq(n + 1, false) {}

    // ═══════════════════════════════════════════════════════
    // 【添加边】
    // 
    // 【修改点】
    // • 无向图：取消注释反向边
    // ═══════════════════════════════════════════════════════
    void add(int u, int v, int w) {
        g[u].emplace_back(v, w);
        // g[v].emplace_back(u, w);  // 【无向图】取消此注释
    }

    // ═══════════════════════════════════════════════════════
    // 【运行SPFA】
    // 参数 s: 源点
    // ═══════════════════════════════════════════════════════
    void run(int s) {
        // 【初始化】
        fill(dist.begin(), dist.end(), INF);
        fill(cnt.begin(), cnt.end(), 0);
        fill(inq.begin(), inq.end(), false);
        has_neg_cycle = false;
        
        dist[s] = 0;
        inq[s] = true;
        cnt[s] = 1;
        
        queue<int> q;
        q.push(s);
        
        while (!q.empty()) {
            int u = q.front(); 
            q.pop();
            inq[u] = false;  // 出队，标记不在队列中
            
            // 【松弛操作】
            for (auto &[v, w] : g[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    
                    // 如果v不在队列中，入队
                    if (!inq[v]) {
                        inq[v] = true;
                        cnt[v]++;
                        
                        // 【判负环】入队次数超过n次
                        if (cnt[v] > n) { 
                            has_neg_cycle = true; 
                            return; 
                        }
                        
                        q.push(v);
                    }
                }
            }
        }
    }

    // ═══════════════════════════════════════════════════════
    // 【查询距离】
    // ═══════════════════════════════════════════════════════
    ll query(int u) { 
        return dist[u]; 
    }

    // ═══════════════════════════════════════════════════════
    // 【判断是否存在负环】
    // ═══════════════════════════════════════════════════════
    bool hasNegativeCycle() { 
        return has_neg_cycle; 
    }
};

// ═══════════════════════════════════════════════════════════
// 【变体】判断整个图是否有负环（从虚拟源点出发）
// ═══════════════════════════════════════════════════════════
struct SPFANegCycleDetect {
    int n;
    vector<vector<pair<int, int>>> g;
    
    SPFANegCycleDetect(int n) : n(n), g(n + 1) {}
    
    void add(int u, int v, int w) {
        g[u].emplace_back(v, w);
    }
    
    // 判断整个图是否存在负环
    bool hasNegativeCycle() {
        vector<ll> dist(n + 1, 0);  // 初始化为0，相当于添加虚拟源点
        vector<int> cnt(n + 1, 0);
        vector<bool> inq(n + 1, false);
        
        queue<int> q;
        for (int i = 1; i <= n; i++) {
            q.push(i);
            inq[i] = true;
        }
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = false;
            
            for (auto &[v, w] : g[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    if (!inq[v]) {
                        inq[v] = true;
                        if (++cnt[v] > n) return true;
                        q.push(v);
                    }
                }
            }
        }
        return false;
    }
};

// ═══════════════════════════════════════════════════════════
// 【应用】差分约束系统
// ═══════════════════════════════════════════════════════════
struct DifferenceConstraints {
    // 求解 x[j] - x[i] <= c 形式的约束
    // 转化为图：从i到j连一条权为c的边
    // 求最短路即为满足约束的解
    
    int n;
    SPFA spfa;
    
    DifferenceConstraints(int n) : n(n), spfa(n + 1) {
        // 添加虚拟源点0，到所有点的距离为0
        for (int i = 1; i <= n; i++) {
            spfa.add(0, i, 0);
        }
    }
    
    // 添加约束 x[j] - x[i] <= c
    void addConstraint(int i, int j, int c) {
        spfa.add(i, j, c);
    }
    
    // 求解，返回是否有解
    // 如果有解，sol[i]即为x[i]的一个解
    bool solve(vector<ll>& sol) {
        spfa.run(0);
        if (spfa.hasNegativeCycle()) return false;
        
        sol.resize(n + 1);
        for (int i = 1; i <= n; i++) {
            sol[i] = spfa.query(i);
        }
        return true;
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== SPFA模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：有负权边的最短路
    // 图：1→2(1), 2→3(-5), 1→3(10)
    // 最短路 1→3: 1→2→3 = 1+(-5) = -4
    // ─────────────────────────────────────────────────────
    {
        SPFA spfa(3);
        spfa.add(1, 2, 1);
        spfa.add(2, 3, -5);  // 负权边
        spfa.add(1, 3, 10);
        spfa.run(1);
        
        assert(spfa.query(3) == -4);
        assert(!spfa.hasNegativeCycle());
        
        cout << "[负权边最短路] 测试通过\n";
        cout << "  dist(1→3) = " << spfa.query(3) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：检测负环
    // 图：1→2(1), 2→3(1), 3→1(-5)
    // 形成负环：1→2→3→1，权值和 = 1+1+(-5) = -3 < 0
    // ─────────────────────────────────────────────────────
    {
        SPFA spfa(3);
        spfa.add(1, 2, 1);
        spfa.add(2, 3, 1);
        spfa.add(3, 1, -5);  // 形成负环
        spfa.run(1);
        
        assert(spfa.hasNegativeCycle() == true);
        cout << "[负环检测] 测试通过，存在负环\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：差分约束
    // 约束：x2 - x1 <= 3, x3 - x2 <= 2, x1 - x3 <= -1
    // 即：x2 <= x1 + 3, x3 <= x2 + 2, x1 <= x3 - 1
    // ─────────────────────────────────────────────────────
    {
        DifferenceConstraints dc(3);
        dc.addConstraint(1, 2, 3);   // x2 - x1 <= 3
        dc.addConstraint(2, 3, 2);   // x3 - x2 <= 2
        dc.addConstraint(3, 1, -1);  // x1 - x3 <= -1
        
        vector<ll> sol;
        bool ok = dc.solve(sol);
        
        assert(ok == true);
        // 验证约束
        assert(sol[2] - sol[1] <= 3);
        assert(sol[3] - sol[2] <= 2);
        assert(sol[1] - sol[3] <= -1);
        
        cout << "[差分约束] 测试通过\n";
        cout << "  x1=" << sol[1] << ", x2=" << sol[2] << ", x3=" << sol[3] << "\n";
    }
    
    cout << "\n===== SPFA测试通过 =====\n";
    return 0;
}