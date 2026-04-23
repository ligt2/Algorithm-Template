#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  【LCA - 最近公共祖先】
  
  ⭐⭐⭐☆ 省赛中等频率
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "树上两点的最近公共祖先"
     • "树上两点距离"（dist(u,v) = dep[u] + dep[v] - 2*dep[lca]）
     • "树上路径"
     • "树链"
  
  ═══════════════════════════════════════════════════════════
  【算法选择】
  
  • 倍增法：预处理O(nlogn)，查询O(logn)，代码简单（推荐）
  • Tarjan离线：O(n+q)，需要离线所有查询
  • 树链剖分：预处理O(n)，查询O(logn)，还能支持路径修改
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 预处理：O(n log n)
  • 单次查询：O(log n)
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 树上两点距离
  • 树上路径第k个节点
  • 树上路径权值和
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】LCA - 倍增法
// ═══════════════════════════════════════════════════════════
struct LCA {
    int n, LOG;
    vector<vector<int>> adj;      // 邻接表
    vector<vector<int>> fa;       // fa[u][k] = u的第2^k个祖先
    vector<int> dep;              // 深度
    
    // 【构造函数】n个节点（1-indexed）
    LCA(int n) : n(n), adj(n + 1), dep(n + 1, 0) {
        LOG = __lg(n) + 1;
        fa.assign(n + 1, vector<int>(LOG, 0));
    }
    
    // 【添加边】
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // 【预处理】从根节点root开始DFS
    void build(int root = 1) {
        dfs(root, 0);
    }
    
    void dfs(int u, int parent) {
        fa[u][0] = parent;
        dep[u] = dep[parent] + 1;
        
        // 【倍增预处理】fa[u][k] = fa[fa[u][k-1]][k-1]
        // 即：u的第2^k祖先 = u的第2^(k-1)祖先的第2^(k-1)祖先
        for (int k = 1; k < LOG; k++) {
            fa[u][k] = fa[fa[u][k - 1]][k - 1];
        }
        
        for (int v : adj[u]) {
            if (v != parent) {
                dfs(v, u);
            }
        }
    }
    
    // 【查询LCA】
    int query(int u, int v) {
        // 【步骤1】让u和v在同一深度
        if (dep[u] < dep[v]) swap(u, v);
        
        int diff = dep[u] - dep[v];
        for (int k = 0; k < LOG; k++) {
            if ((diff >> k) & 1) {
                u = fa[u][k];
            }
        }
        
        // 【步骤2】如果已经相遇，v就是LCA
        if (u == v) return u;
        
        // 【步骤3】一起往上跳，直到父节点相同
        for (int k = LOG - 1; k >= 0; k--) {
            if (fa[u][k] != fa[v][k]) {
                u = fa[u][k];
                v = fa[v][k];
            }
        }
        
        return fa[u][0];
    }
    
    // 【查询两点距离】
    int dist(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[query(u, v)];
    }
    
    // 【查询u是否是v的祖先】
    bool isAncestor(int u, int v) {
        return query(u, v) == u;
    }
    
    // 【查询u到v路径上第k个节点】(1-indexed，k=1是u本身)
    int kthOnPath(int u, int v, int k) {
        int lca = query(u, v);
        int lenToLCA = dep[u] - dep[lca];
        
        if (k <= lenToLCA + 1) {
            // 在u到lca的路径上
            return kthAncestor(u, k - 1);
        } else {
            // 在lca到v的路径上
            int remaining = dep[u] + dep[v] - 2 * dep[lca] + 1 - k;
            return kthAncestor(v, remaining);
        }
    }
    
    // 【查询u的第k个祖先】
    int kthAncestor(int u, int k) {
        for (int i = 0; i < LOG && u; i++) {
            if ((k >> i) & 1) {
                u = fa[u][i];
            }
        }
        return u;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】带边权的LCA（用于求路径权值和）
// ═══════════════════════════════════════════════════════════
struct LCA_Weighted {
    int n, LOG;
    vector<vector<pair<int, ll>>> adj;  // adj[u] = {(v, w), ...}
    vector<vector<int>> fa;
    vector<int> dep;
    vector<ll> dist;             // 到根的距离
    
    LCA_Weighted(int n) : n(n), adj(n + 1), dep(n + 1, 0), dist(n + 1, 0) {
        LOG = __lg(n) + 1;
        fa.assign(n + 1, vector<int>(LOG, 0));
    }
    
    void addEdge(int u, int v, ll w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    
    void build(int root = 1) {
        dfs(root, 0, 0);
    }
    
    void dfs(int u, int parent, ll d) {
        fa[u][0] = parent;
        dep[u] = dep[parent] + 1;
        dist[u] = d;
        
        for (int k = 1; k < LOG; k++) {
            fa[u][k] = fa[fa[u][k - 1]][k - 1];
        }
        
        for (auto [v, w] : adj[u]) {
            if (v != parent) {
                dfs(v, u, d + w);
            }
        }
    }
    
    int query(int u, int v) {
        if (dep[u] < dep[v]) swap(u, v);
        int diff = dep[u] - dep[v];
        for (int k = 0; k < LOG; k++) {
            if ((diff >> k) & 1) u = fa[u][k];
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
    
    // 【路径权值和】
    ll pathSum(int u, int v) {
        int lca = query(u, v);
        return dist[u] + dist[v] - 2 * dist[lca];
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== LCA模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：基本LCA查询
    // 树结构：
    //        1
    //       / \
    //      2   3
    //     / \
    //    4   5
    // ─────────────────────────────────────────────────────
    {
        LCA lca(5);
        lca.addEdge(1, 2);
        lca.addEdge(1, 3);
        lca.addEdge(2, 4);
        lca.addEdge(2, 5);
        lca.build(1);
        
        assert(lca.query(4, 5) == 2);
        assert(lca.query(4, 3) == 1);
        assert(lca.query(4, 2) == 2);
        
        cout << "[LCA查询] 测试通过\n";
        cout << "         LCA(4,5)=" << lca.query(4, 5) << "\n";
        cout << "         LCA(4,3)=" << lca.query(4, 3) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：两点距离
    // ─────────────────────────────────────────────────────
    {
        LCA lca(5);
        lca.addEdge(1, 2);
        lca.addEdge(1, 3);
        lca.addEdge(2, 4);
        lca.addEdge(2, 5);
        lca.build(1);
        
        assert(lca.dist(4, 5) == 2);
        assert(lca.dist(4, 3) == 3);
        
        cout << "[两点距离] 测试通过，dist(4,5)=" << lca.dist(4, 5) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：带边权LCA
    //        1
    //      2/ \3
    //      2   3
    //    1/ \4
    //    4   5
    // dist(4,5) = 1+4 = 5
    // dist(4,3) = 1+2+3 = 6
    // ─────────────────────────────────────────────────────
    {
        LCA_Weighted lca(5);
        lca.addEdge(1, 2, 2);
        lca.addEdge(1, 3, 3);
        lca.addEdge(2, 4, 1);
        lca.addEdge(2, 5, 4);
        lca.build(1);
        
        assert(lca.pathSum(4, 5) == 5);
        assert(lca.pathSum(4, 3) == 6);
        
        cout << "[带权LCA] 测试通过，pathSum(4,5)=" << lca.pathSum(4, 5) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试4：路径第k个节点
    // ─────────────────────────────────────────────────────
    {
        LCA lca(5);
        lca.addEdge(1, 2);
        lca.addEdge(1, 3);
        lca.addEdge(2, 4);
        lca.addEdge(2, 5);
        lca.build(1);
        
        // 路径4→5：4→2→5
        assert(lca.kthOnPath(4, 5, 1) == 4);
        assert(lca.kthOnPath(4, 5, 2) == 2);
        assert(lca.kthOnPath(4, 5, 3) == 5);
        
        cout << "[路径第k点] 测试通过\n";
    }
    
    cout << "\n===== 所有LCA测试通过 =====\n";
    return 0;
}