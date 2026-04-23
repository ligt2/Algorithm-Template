#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  【Dinic算法 - 最大流】
  
  ⭐⭐⭐⭐☆ 省赛高频
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "最大流"
     • "最小割"（最大流=最小割）
     • "网络容量"
     • "二分图最大匹配"（可用最大流解决）
     • "最大不相交路径数"
  
  ✅ 建模技巧：
     • 点有容量限制 → 拆点（入点→出点）
     • 无向边 → 双向边各加容量
     • 多源多汇 → 添加超级源和超级汇
  
  ═══════════════════════════════════════════════════════════
  【核心思想】
  
  1. BFS分层，确定到每个点的最短距离
  2. DFS多路增广，沿着分层图找增广路
  3. 重复直到无法从源点到达汇点
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 一般图：O(V² × E)
  • 二分图：O(E × √V)
  • 单位容量图：O(E × √V)
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 二分图最大匹配（转化为网络流）
  • 最小割问题
  • 最大不相交路径
  
=============================================================*/

struct Dinic {
    // 【边的结构】
    struct Edge {
        int to;      // 终点
        ll cap;      // 容量
        ll flow;     // 当前流量
    };
    
    int n;                      // 节点数
    vector<Edge> edges;         // 边列表
    vector<vector<int>> adj;    // adj[u] = 边的编号列表
    vector<int> depth;          // BFS分层的深度
    vector<int> cur;            // 当前弧优化
    
    // 【构造函数】n个节点（0-indexed）
    Dinic(int n) : n(n), adj(n), depth(n), cur(n) {}
    
    // 【添加有向边】u → v，容量cap
    // ⚠️ 如果是无向边，调用两次：addEdge(u,v,cap); addEdge(v,u,cap);
    void addEdge(int u, int v, ll cap) {
        // 正向边
        adj[u].push_back(edges.size());
        edges.push_back({v, cap, 0});
        // 反向边（用于退流）
        adj[v].push_back(edges.size());
        edges.push_back({u, 0, 0});
    }
    
    // 【BFS分层】返回是否能到达汇点
    bool bfs(int s, int t) {
        fill(depth.begin(), depth.end(), -1);
        depth[s] = 0;
        queue<int> q;
        q.push(s);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) {
                Edge& e = edges[id];
                // 还有剩余容量且未访问
                if (e.cap > e.flow && depth[e.to] == -1) {
                    depth[e.to] = depth[u] + 1;
                    q.push(e.to);
                }
            }
        }
        
        return depth[t] != -1;
    }
    
    // 【DFS增广】返回增广的流量
    // pushed: 从源点到u已经推送的流量
    ll dfs(int u, int t, ll pushed) {
        if (u == t || pushed == 0) return pushed;
        
        ll result = 0;
        // 【当前弧优化】从cur[u]开始，避免重复遍历无用边
        for (int& i = cur[u]; i < adj[u].size(); i++) {
            int id = adj[u][i];
            Edge& e = edges[id];
            
            // 只走分层图的下一层
            if (depth[e.to] != depth[u] + 1) continue;
            
            ll canPush = min(pushed - result, e.cap - e.flow);
            ll d = dfs(e.to, t, canPush);
            
            if (d > 0) {
                e.flow += d;                    // 正向边加流量
                edges[id ^ 1].flow -= d;        // 反向边减流量（id^1是配对边）
                result += d;
                if (result == pushed) break;    // 已推满
            }
        }
        
        return result;
    }
    
    // 【求最大流】从s到t
    ll maxflow(int s, int t) {
        ll flow = 0;
        
        // 不断BFS分层 + DFS增广
        while (bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);  // 重置当前弧
            while (ll d = dfs(s, t, LLONG_MAX)) {
                flow += d;
            }
        }
        
        return flow;
    }
    
    // 【获取最小割的边】（在最大流后调用）
    // 返回：割边的编号（只返回正向边）
    vector<int> minCut(int s) {
        // 从s出发，能到达的点属于S集合
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) {
                Edge& e = edges[id];
                if (e.cap > e.flow && !visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        
        // 找从S到T的满流边
        vector<int> result;
        for (int i = 0; i < edges.size(); i += 2) {  // 只遍历正向边
            int u = edges[i ^ 1].to;  // 正向边的起点
            int v = edges[i].to;       // 正向边的终点
            if (visited[u] && !visited[v] && edges[i].cap > 0) {
                result.push_back(i);
            }
        }
        return result;
    }
};

// ═══════════════════════════════════════════════════════════
// 【带下界的最大流 / 最小费用最大流 见扩展模板】
// ═══════════════════════════════════════════════════════════

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== Dinic最大流模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：基础最大流
    // 图结构：
    //     (10)     (10)
    //  0 ----→ 1 ----→ 3
    //  |       |       ↑
    //  |(10)   |(2)    |(10)
    //  ↓       ↓       |
    //  2 ----→ 4 ----→ 5
    //     (10)     (10)
    // 源点0，汇点3
    // 最大流 = 20
    // ─────────────────────────────────────────────────────
    {
        Dinic dinic(4);
        // 简化版：0→1(10), 0→2(10), 1→2(2), 1→3(4), 2→3(10)
        dinic.addEdge(0, 1, 10);
        dinic.addEdge(0, 2, 10);
        dinic.addEdge(1, 2, 2);
        dinic.addEdge(1, 3, 4);
        dinic.addEdge(2, 3, 10);
        
        ll maxFlow = dinic.maxflow(0, 3);
        assert(maxFlow == 14);
        cout << "[基础最大流] 测试通过，最大流=" << maxFlow << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：二分图匹配（用最大流）
    // 左边3个点(1,2,3)，右边3个点(4,5,6)
    // 边：1-4, 1-5, 2-5, 3-6
    // 添加源点0和汇点7
    // ─────────────────────────────────────────────────────
    {
        Dinic dinic(8);
        // 源点0连左边
        dinic.addEdge(0, 1, 1);
        dinic.addEdge(0, 2, 1);
        dinic.addEdge(0, 3, 1);
        // 左边连右边
        dinic.addEdge(1, 4, 1);
        dinic.addEdge(1, 5, 1);
        dinic.addEdge(2, 5, 1);
        dinic.addEdge(3, 6, 1);
        // 右边连汇点7
        dinic.addEdge(4, 7, 1);
        dinic.addEdge(5, 7, 1);
        dinic.addEdge(6, 7, 1);
        
        ll matching = dinic.maxflow(0, 7);
        assert(matching == 3);
        cout << "[二分图最大匹配] 测试通过，最大匹配=" << matching << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：最小割
    // ─────────────────────────────────────────────────────
    {
        Dinic dinic(4);
        dinic.addEdge(0, 1, 3);
        dinic.addEdge(0, 2, 2);
        dinic.addEdge(1, 3, 2);
        dinic.addEdge(2, 3, 3);
        
        ll maxFlow = dinic.maxflow(0, 3);
        assert(maxFlow == 5);  // 最小割 = 最大流 = 5
        cout << "[最小割] 测试通过，最小割=" << maxFlow << "\n";
    }
    
    cout << "\n===== 所有Dinic测试通过 =====\n";
    return 0;
}