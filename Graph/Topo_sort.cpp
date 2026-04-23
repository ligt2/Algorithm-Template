#include <bits/stdc++.h>
using namespace std;

/*=============================================================
  【拓扑排序 (Topological Sort)】
  
  ⭐⭐⭐⭐ 省赛常用
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "依赖关系"、"先后顺序"
     • "DAG"（有向无环图）
     • "课程先修"、"任务依赖"
     • "检测环"
  
  ✅ 典型场景：
     • 课程安排（先修课程）
     • 任务调度
     • 编译依赖
     • 判断有向图是否有环
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 时间：O(V + E)
  • 空间：O(V + E)
  
  ═══════════════════════════════════════════════════════════
  【核心原理】
  
  Kahn算法（BFS）：
  1. 将所有入度为0的点入队
  2. 每次取出一个点，删除它的所有出边
  3. 新产生的入度为0的点入队
  4. 如果取出的点数 < n，说明有环
  
  ═══════════════════════════════════════════════════════════
  【常见变体/修改点】
  
  1. 字典序最小的拓扑序：用优先队列
  2. 判断拓扑序是否唯一：检查每次队列是否只有一个元素
  3. DAG上的最长路：在拓扑序上DP
  4. DAG上的最短路：在拓扑序上DP
  
=============================================================*/

struct TopoSort {
    int n;                        // 节点数量
    vector<vector<int>> adj;      // 邻接表
    vector<int> indeg;            // 入度
    vector<int> order;            // 拓扑序

    // ═══════════════════════════════════════════════════════
    // 【构造函数】
    // ═══════════════════════════════════════════════════════
    TopoSort(int n) : n(n), adj(n + 1), indeg(n + 1, 0) {}

    // ═══════════════════════════════════════════════════════
    // 【添加有向边】u → v
    // 含义：u必须在v之前
    // ═══════════════════════════════════════════════════════
    void add(int u, int v) {
        adj[u].push_back(v);
        indeg[v]++;  // v的入度+1
    }

    // ═══════════════════════════════════════════════════════
    // 【运行拓扑排序】
    // 返回值：是否成功（false表示有环）
    // ═══════════════════════════════════════════════════════
    bool run() {
        order.clear();
        queue<int> q;
        
        // 【步骤1】将所有入度为0的点入队
        for (int i = 1; i <= n; i++) {
            if (indeg[i] == 0) {
                q.push(i);
            }
        }
        
        // 【步骤2】BFS
        while (!q.empty()) {
            int u = q.front(); 
            q.pop();
            order.push_back(u);
            
            // 删除u的所有出边
            for (int v : adj[u]) {
                indeg[v]--;
                // 如果v的入度变为0，入队
                if (indeg[v] == 0) {
                    q.push(v);
                }
            }
        }
        
        // 【判断是否有环】
        return order.size() == n;
    }

    // ═══════════════════════════════════════════════════════
    // 【获取拓扑序】
    // ═══════════════════════════════════════════════════════
    vector<int>& getOrder() { 
        return order; 
    }
};

// ═══════════════════════════════════════════════════════════
// 【变体】字典序最小的拓扑排序
// ═══════════════════════════════════════════════════════════
struct TopoSortLex {
    int n;
    vector<vector<int>> adj;
    vector<int> indeg;
    vector<int> order;

    TopoSortLex(int n) : n(n), adj(n + 1), indeg(n + 1, 0) {}

    void add(int u, int v) {
        adj[u].push_back(v);
        indeg[v]++;
    }

    bool run() {
        order.clear();
        // 【关键】用小根堆代替普通队列
        priority_queue<int, vector<int>, greater<int>> pq;
        
        for (int i = 1; i <= n; i++) {
            if (indeg[i] == 0) pq.push(i);
        }
        
        while (!pq.empty()) {
            int u = pq.top(); pq.pop();
            order.push_back(u);
            
            for (int v : adj[u]) {
                if (--indeg[v] == 0) {
                    pq.push(v);
                }
            }
        }
        
        return order.size() == n;
    }

    vector<int>& getOrder() { return order; }
};

// ═══════════════════════════════════════════════════════════
// 【应用】DAG上的最长路 / 最短路
// ═══════════════════════════════════════════════════════════
struct DAGPath {
    int n;
    vector<vector<pair<int, int>>> adj;  // adj[u] = {(v, w), ...}
    vector<int> indeg;
    vector<long long> dist;
    
    DAGPath(int n) : n(n), adj(n + 1), indeg(n + 1, 0), dist(n + 1) {}
    
    void add(int u, int v, int w) {
        adj[u].push_back({v, w});
        indeg[v]++;
    }
    
    // 【DAG最长路】从s出发
    long long longestPath(int s) {
        fill(dist.begin(), dist.end(), LLONG_MIN);
        dist[s] = 0;
        
        // 拓扑排序
        queue<int> q;
        vector<int> deg = indeg;
        for (int i = 1; i <= n; i++) {
            if (deg[i] == 0) q.push(i);
        }
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            
            // 更新邻居
            for (auto [v, w] : adj[u]) {
                if (dist[u] != LLONG_MIN) {
                    dist[v] = max(dist[v], dist[u] + w);
                }
                if (--deg[v] == 0) {
                    q.push(v);
                }
            }
        }
        
        long long ans = LLONG_MIN;
        for (int i = 1; i <= n; i++) {
            ans = max(ans, dist[i]);
        }
        return ans;
    }
    
    // 【DAG最短路】从s出发
    long long shortestPath(int s, int t) {
        fill(dist.begin(), dist.end(), LLONG_MAX);
        dist[s] = 0;
        
        queue<int> q;
        vector<int> deg = indeg;
        for (int i = 1; i <= n; i++) {
            if (deg[i] == 0) q.push(i);
        }
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            
            for (auto [v, w] : adj[u]) {
                if (dist[u] != LLONG_MAX) {
                    dist[v] = min(dist[v], dist[u] + w);
                }
                if (--deg[v] == 0) {
                    q.push(v);
                }
            }
        }
        
        return dist[t];
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 拓扑排序模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：基础拓扑排序
    // 依赖关系：1→2, 1→3, 2→4, 3→4
    //         1
    //        / \
    //       2   3
    //        \ /
    //         4
    // 合法拓扑序：[1,2,3,4] 或 [1,3,2,4]
    // ─────────────────────────────────────────────────────
    {
        TopoSort ts(4);
        ts.add(1, 2);
        ts.add(1, 3);
        ts.add(2, 4);
        ts.add(3, 4);
        
        bool ok = ts.run();
        assert(ok == true);
        assert(ts.getOrder().size() == 4);
        assert(ts.getOrder()[0] == 1);
        assert(ts.getOrder()[3] == 4);
        
        cout << "[基础拓扑排序] 测试通过\n";
        cout << "  拓扑序: ";
        for (int v : ts.getOrder()) cout << v << " ";
        cout << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：检测环
    // 依赖关系：1→2, 2→3, 3→1（有环）
    // ─────────────────────────────────────────────────────
    {
        TopoSort ts(3);
        ts.add(1, 2);
        ts.add(2, 3);
        ts.add(3, 1);
        
        bool ok = ts.run();
        assert(ok == false);
        
        cout << "[检测环] 测试通过，存在环\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：字典序最小的拓扑排序
    // ─────────────────────────────────────────────────────
    {
        TopoSortLex ts(4);
        ts.add(1, 2);
        ts.add(1, 3);
        ts.add(2, 4);
        ts.add(3, 4);
        
        bool ok = ts.run();
        assert(ok == true);
        // 字典序最小：1, 2, 3, 4
        assert(ts.getOrder() == vector<int>({1, 2, 3, 4}));
        
        cout << "[字典序最小] 测试通过\n";
        cout << "  拓扑序: ";
        for (int v : ts.getOrder()) cout << v << " ";
        cout << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试4：DAG最长路
    // ─────────────────────────────────────────────────────
    {
        DAGPath dag(4);
        dag.add(1, 2, 3);
        dag.add(1, 3, 2);
        dag.add(2, 4, 4);
        dag.add(3, 4, 5);
        
        long long ans = dag.longestPath(1);
        // 最长路：1→3→4 = 2+5=7 或 1→2→4 = 3+4=7
        assert(ans == 7);
        
        cout << "[DAG最长路] 测试通过，最长路=" << ans << "\n";
    }
    
    cout << "\n===== 拓扑排序测试通过 =====\n";
    return 0;
}