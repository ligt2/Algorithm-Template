#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  【树上差分 + 基环树处理】
  
  ⭐⭐⭐ 省赛中等频率
  
  ═══════════════════════════════════════════════════════════
  【树上差分 - 什么时候用？】
  
  ✅ 信号词：
     • "给树上路径加权值"（多次操作后求每个点/边的值）
     • "树上区间修改"
     • 类似数组差分，但在树上
  
  ✅ 两种形式：
     • 点差分：修改路径上所有点
     • 边差分：修改路径上所有边
  
  ═══════════════════════════════════════════════════════════
  【基环树 - 什么时候用？】
  
  ✅ 信号词：
     • "n个点n条边"（恰好多一条边形成一个环）
     • "每个点恰好有一条出边"（内向基环树）
     • "功能图"
  
  ✅ 处理思路：
     1. 找到环
     2. 断开环上一条边，变成树处理
     3. 或者把环单独拎出来，环上挂着若干子树
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】树上点差分
// ═══════════════════════════════════════════════════════════
struct TreePointDiff {
    int n;
    vector<vector<int>> adj;
    vector<ll> diff;          // 差分数组
    vector<ll> val;           // 最终每个点的值
    vector<vector<int>> fa;   // LCA用
    vector<int> dep;
    int LOG;
    
    TreePointDiff(int n) : n(n), adj(n + 1), diff(n + 1, 0), 
                           val(n + 1, 0), dep(n + 1, 0) {
        LOG = __lg(n) + 1;
        fa.assign(n + 1, vector<int>(LOG, 0));
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void buildLCA(int root = 1) {
        dfsLCA(root, 0);
    }
    
    void dfsLCA(int u, int parent) {
        fa[u][0] = parent;
        dep[u] = dep[parent] + 1;
        for (int k = 1; k < LOG; k++) {
            fa[u][k] = fa[fa[u][k-1]][k-1];
        }
        for (int v : adj[u]) {
            if (v != parent) dfsLCA(v, u);
        }
    }
    
    int lca(int u, int v) {
        if (dep[u] < dep[v]) swap(u, v);
        int d = dep[u] - dep[v];
        for (int k = 0; k < LOG; k++) {
            if ((d >> k) & 1) u = fa[u][k];
        }
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; k--) {
            if (fa[u][k] != fa[v][k]) {
                u = fa[u][k];
                v = fa[v][k];
            }
        }
        return fa[u][0];
    }
    
    // 【路径修改】给 u 到 v 路径上所有点加 delta
    // diff[u] += delta, diff[v] += delta
    // diff[lca] -= delta, diff[fa[lca]] -= delta
    void pathAdd(int u, int v, ll delta) {
        int l = lca(u, v);
        diff[u] += delta;
        diff[v] += delta;
        diff[l] -= delta;
        if (fa[l][0]) diff[fa[l][0]] -= delta;
    }
    
    // 【统计结果】DFS累加差分值
    void collect(int root = 1) {
        dfsCollect(root, 0);
    }
    
    void dfsCollect(int u, int parent) {
        val[u] = diff[u];
        for (int v : adj[u]) {
            if (v != parent) {
                dfsCollect(v, u);
                val[u] += val[v];
            }
        }
    }
    
    ll getVal(int u) { return val[u]; }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】树上边差分
// ═══════════════════════════════════════════════════════════
struct TreeEdgeDiff {
    int n;
    vector<vector<pair<int, int>>> adj;  // adj[u] = {(v, edgeId), ...}
    vector<ll> diff;          // 点差分（映射到边）
    vector<ll> edgeVal;       // 每条边的值
    vector<vector<int>> fa;
    vector<int> dep;
    int LOG, edgeCnt;
    
    TreeEdgeDiff(int n) : n(n), adj(n + 1), diff(n + 1, 0), 
                          dep(n + 1, 0), edgeCnt(0) {
        LOG = __lg(n) + 1;
        fa.assign(n + 1, vector<int>(LOG, 0));
    }
    
    int addEdge(int u, int v) {
        int id = edgeCnt++;
        adj[u].push_back({v, id});
        adj[v].push_back({u, id});
        edgeVal.push_back(0);
        return id;
    }
    
    void buildLCA(int root = 1) {
        dfsLCA(root, 0);
    }
    
    void dfsLCA(int u, int parent) {
        fa[u][0] = parent;
        dep[u] = dep[parent] + 1;
        for (int k = 1; k < LOG; k++) {
            fa[u][k] = fa[fa[u][k-1]][k-1];
        }
        for (auto [v, _] : adj[u]) {
            if (v != parent) dfsLCA(v, u);
        }
    }
    
    int lca(int u, int v) {
        if (dep[u] < dep[v]) swap(u, v);
        int d = dep[u] - dep[v];
        for (int k = 0; k < LOG; k++) {
            if ((d >> k) & 1) u = fa[u][k];
        }
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; k--) {
            if (fa[u][k] != fa[v][k]) {
                u = fa[u][k];
                v = fa[v][k];
            }
        }
        return fa[u][0];
    }
    
    // 【路径修改】给 u 到 v 路径上所有边加 delta
    // diff[u] += delta, diff[v] += delta
    // diff[lca] -= 2 * delta
    void pathAdd(int u, int v, ll delta) {
        int l = lca(u, v);
        diff[u] += delta;
        diff[v] += delta;
        diff[l] -= 2 * delta;
    }
    
    // 【统计结果】将点差分映射到边
    void collect(int root = 1) {
        dfsCollect(root, 0, -1);
    }
    
    void dfsCollect(int u, int parent, int parentEdge) {
        ll sum = diff[u];
        for (auto [v, edgeId] : adj[u]) {
            if (v != parent) {
                dfsCollect(v, u, edgeId);
                sum += diff[v];  // 先收集子树的差分值
            }
        }
        diff[u] = sum;  // 更新当前点差分
        if (parentEdge != -1) {
            edgeVal[parentEdge] = sum;
        }
    }
    
    ll getEdgeVal(int edgeId) { return edgeVal[edgeId]; }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】基环树 - 找环 + 处理
// ═══════════════════════════════════════════════════════════
struct PseudoTree {
    int n;
    vector<vector<int>> adj;
    vector<int> deg;          // 入度
    vector<bool> onCycle;     // 是否在环上
    vector<int> cycle;        // 环上的节点（按顺序）
    
    PseudoTree(int n) : n(n), adj(n + 1), deg(n + 1, 0), onCycle(n + 1, false) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        deg[u]++;
        deg[v]++;
    }
    
    // 【拓扑排序找环】删除所有度为1的点，剩下的就是环
    void findCycle() {
        queue<int> q;
        vector<int> d = deg;
        
        // 将度为1的点入队
        for (int i = 1; i <= n; i++) {
            if (d[i] == 1) q.push(i);
        }
        
        // 不断删除度为1的点
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (--d[v] == 1) {
                    q.push(v);
                }
            }
        }
        
        // 度 >= 2 的点在环上
        for (int i = 1; i <= n; i++) {
            if (d[i] >= 2) {
                onCycle[i] = true;
            }
        }
        
        // 按顺序提取环
        extractCycle();
    }
    
    void extractCycle() {
        cycle.clear();
        int start = -1;
        for (int i = 1; i <= n; i++) {
            if (onCycle[i]) { start = i; break; }
        }
        if (start == -1) return;
        
        int cur = start, prev = -1;
        do {
            cycle.push_back(cur);
            for (int v : adj[cur]) {
                if (onCycle[v] && v != prev) {
                    prev = cur;
                    cur = v;
                    break;
                }
            }
        } while (cur != start);
    }
    
    // 【获取环】
    vector<int>& getCycle() { return cycle; }
    
    // 【判断是否在环上】
    bool isOnCycle(int u) { return onCycle[u]; }
};

// ═══════════════════════════════════════════════════════════
// 【模板4】内向基环树（每个点恰好一条出边）
// ═══════════════════════════════════════════════════════════
struct FunctionalGraph {
    int n;
    vector<int> to;           // to[u] = u的后继
    vector<bool> onCycle;
    vector<int> cycleId;      // 属于哪个环（0表示不在环上）
    vector<vector<int>> cycles;  // 所有环
    
    FunctionalGraph(int n) : n(n), to(n + 1), onCycle(n + 1, false), 
                              cycleId(n + 1, 0) {}
    
    void setEdge(int u, int v) {
        to[u] = v;
    }
    
    // 【找所有环】
    void findCycles() {
        vector<int> vis(n + 1, 0);  // 0未访问，1访问中，2已完成
        
        for (int i = 1; i <= n; i++) {
            if (vis[i]) continue;
            
            // 找到从i开始的路径
            vector<int> path;
            int u = i;
            while (vis[u] == 0) {
                vis[u] = 1;
                path.push_back(u);
                u = to[u];
            }
            
            // 如果u是访问中的状态，说明找到一个新环
            if (vis[u] == 1) {
                vector<int> cyc;
                int cid = cycles.size() + 1;
                bool found = false;
                for (int v : path) {
                    if (v == u) found = true;
                    if (found) {
                        cyc.push_back(v);
                        onCycle[v] = true;
                        cycleId[v] = cid;
                    }
                }
                cycles.push_back(cyc);
            }
            
            // 标记为已完成
            for (int v : path) vis[v] = 2;
        }
    }
    
    int getCycleCount() { return cycles.size(); }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 树上差分/基环树模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：树上点差分
    //        1
    //       / \
    //      2   3
    //     / \
    //    4   5
    // 操作：路径4→3每个点+1
    // 结果：4:1, 2:1, 1:1, 3:1, 5:0
    // ─────────────────────────────────────────────────────
    {
        TreePointDiff tree(5);
        tree.addEdge(1, 2);
        tree.addEdge(1, 3);
        tree.addEdge(2, 4);
        tree.addEdge(2, 5);
        tree.buildLCA(1);
        
        tree.pathAdd(4, 3, 1);  // 路径4-2-1-3
        tree.collect(1);
        
        assert(tree.getVal(4) == 1);
        assert(tree.getVal(2) == 1);
        assert(tree.getVal(1) == 1);
        assert(tree.getVal(3) == 1);
        assert(tree.getVal(5) == 0);
        
        cout << "[树上点差分] 测试通过\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：基环树找环
    // 图：1-2, 2-3, 3-4, 4-2（环：2-3-4）
    // ─────────────────────────────────────────────────────
    {
        PseudoTree pt(4);
        pt.addEdge(1, 2);
        pt.addEdge(2, 3);
        pt.addEdge(3, 4);
        pt.addEdge(4, 2);
        pt.findCycle();
        
        assert(!pt.isOnCycle(1));
        assert(pt.isOnCycle(2));
        assert(pt.isOnCycle(3));
        assert(pt.isOnCycle(4));
        
        cout << "[基环树找环] 测试通过，环: ";
        for (int v : pt.getCycle()) cout << v << " ";
        cout << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：内向基环树
    // to: 1→2, 2→3, 3→1, 4→3
    // 环：1→2→3→1
    // ─────────────────────────────────────────────────────
    {
        FunctionalGraph fg(4);
        fg.setEdge(1, 2);
        fg.setEdge(2, 3);
        fg.setEdge(3, 1);
        fg.setEdge(4, 3);
        fg.findCycles();
        
        assert(fg.getCycleCount() == 1);
        assert(fg.onCycle[1] && fg.onCycle[2] && fg.onCycle[3]);
        assert(!fg.onCycle[4]);
        
        cout << "[内向基环树] 测试通过，环数量=" << fg.getCycleCount() << "\n";
    }
    
    cout << "\n===== 所有树上差分/基环树测试通过 =====\n";
    return 0;
}