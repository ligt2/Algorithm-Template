#include <bits/stdc++.h>
using namespace std;

/*=============================================================
  【Tarjan算法 - 强连通分量(SCC) / 割点 / 割边(桥)】
  
  ⭐⭐⭐⭐⭐ 省赛必考，优先级最高
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 强连通分量(SCC)信号词：
     • "有向图中互相可达的点缩成一个"
     • "DAG缩点"
     • "有向图中的环"
     • 需要将有向图转化为DAG后再处理
  
  ✅ 割点信号词：
     • "删除某个点后图不连通"
     • "关键节点"、"关节点"
     • "网络中的关键服务器"
  
  ✅ 割边(桥)信号词：
     • "删除某条边后图不连通"
     • "关键边"、"桥"
     • "网络中的关键链路"
  
  ═══════════════════════════════════════════════════════════
  【核心概念】
  
  • dfn[u]: u的DFS序（第几个被访问）
  • low[u]: u能到达的最小dfn值
  
  SCC判定：dfn[u] == low[u] → u是某个SCC的根
  割点判定：low[v] >= dfn[u] → u是割点（v是u的子节点）
  割边判定：low[v] > dfn[u] → (u,v)是割边
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 时间：O(V + E)
  • 空间：O(V + E)
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 蓝桥杯：受欢迎的牛、缩点后最长路
  • 洛谷：P3387 缩点、P3388 割点
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】强连通分量 (SCC) - 有向图
// ═══════════════════════════════════════════════════════════
struct SCC {
    int n;                          // 节点数
    vector<vector<int>> adj;        // 邻接表
    vector<int> dfn, low;           // DFS序和能到达的最小DFS序
    vector<int> sccId;              // 每个点属于哪个SCC
    vector<int> sccSize;            // 每个SCC的大小
    vector<bool> inStack;           // 是否在栈中
    stack<int> stk;                 // Tarjan用的栈
    int timestamp;                  // 时间戳
    int sccCnt;                     // SCC的数量
    
    // 【构造函数】n个节点（1-indexed）
    SCC(int n) : n(n), adj(n + 1), dfn(n + 1, 0), low(n + 1, 0),
                 sccId(n + 1, 0), inStack(n + 1, false), 
                 timestamp(0), sccCnt(0) {}
    
    // 【添加有向边】u → v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }
    
    // 【Tarjan DFS】
    void tarjan(int u) {
        // 【初始化】记录DFS序
        dfn[u] = low[u] = ++timestamp;
        stk.push(u);
        inStack[u] = true;
        
        // 【遍历邻居】
        for (int v : adj[u]) {
            if (dfn[v] == 0) {
                // v未访问过，递归
                tarjan(v);
                // 【回溯更新】子节点能到达的，父节点也能到达
                low[u] = min(low[u], low[v]);
            } else if (inStack[v]) {
                // v在栈中，说明v是u的祖先（存在环）
                // 【关键】只有在栈中的才能更新（避免跨SCC更新）
                low[u] = min(low[u], dfn[v]);
            }
        }
        
        // 【判断SCC根】
        if (dfn[u] == low[u]) {
            // u是当前SCC的根，弹出栈中所有属于这个SCC的点
            sccCnt++;
            int size = 0;
            while (true) {
                int x = stk.top(); stk.pop();
                inStack[x] = false;
                sccId[x] = sccCnt;
                size++;
                if (x == u) break;
            }
            sccSize.push_back(size);
        }
    }
    
    // 【运行Tarjan】
    void run() {
        sccSize.clear();
        sccSize.push_back(0);  // sccSize[0]占位
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                tarjan(i);
            }
        }
    }
    
    // 【获取SCC数量】
    int getSccCount() { return sccCnt; }
    
    // 【获取某点所属的SCC编号】
    int getSccId(int u) { return sccId[u]; }
    
    // 【获取某SCC的大小】
    int getSccSize(int id) { return sccSize[id]; }
    
    // 【建立缩点后的DAG】
    // 返回：缩点后的邻接表（SCC编号从1到sccCnt）
    vector<vector<int>> buildDAG() {
        vector<set<int>> dagSet(sccCnt + 1);
        vector<vector<int>> dag(sccCnt + 1);
        
        for (int u = 1; u <= n; u++) {
            for (int v : adj[u]) {
                int su = sccId[u], sv = sccId[v];
                if (su != sv && !dagSet[su].count(sv)) {
                    dagSet[su].insert(sv);
                    dag[su].push_back(sv);
                }
            }
        }
        return dag;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】割点 (Articulation Points) - 无向图
// ═══════════════════════════════════════════════════════════
struct CutVertex {
    int n;
    vector<vector<int>> adj;
    vector<int> dfn, low;
    vector<bool> isCut;      // 是否是割点
    int timestamp;
    int root;                // 当前DFS树的根
    
    CutVertex(int n) : n(n), adj(n + 1), dfn(n + 1, 0), 
                       low(n + 1, 0), isCut(n + 1, false), timestamp(0) {}
    
    // 【添加无向边】
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void tarjan(int u, int parent) {
        dfn[u] = low[u] = ++timestamp;
        int childCnt = 0;  // 子节点数量（用于判断根节点是否是割点）
        
        for (int v : adj[u]) {
            if (dfn[v] == 0) {
                childCnt++;
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                
                // 【割点判定】
                // 条件1：u不是根，且low[v] >= dfn[u]
                //        意味着v无法不经过u到达u的祖先
                // 条件2：u是根，且有>=2个子节点
                if (u != root && low[v] >= dfn[u]) {
                    isCut[u] = true;
                }
            } else if (v != parent) {
                // v已访问且不是父节点，说明存在回边
                low[u] = min(low[u], dfn[v]);
            }
        }
        
        // 【根节点特判】
        if (u == root && childCnt >= 2) {
            isCut[u] = true;
        }
    }
    
    void run() {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                root = i;
                tarjan(i, -1);
            }
        }
    }
    
    // 【获取所有割点】
    vector<int> getCutVertices() {
        vector<int> result;
        for (int i = 1; i <= n; i++) {
            if (isCut[i]) result.push_back(i);
        }
        return result;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】割边/桥 (Bridges) - 无向图
// ═══════════════════════════════════════════════════════════
struct Bridge {
    int n;
    vector<vector<pair<int, int>>> adj;  // adj[u] = {(v, edgeId), ...}
    vector<int> dfn, low;
    vector<bool> isBridge;   // 边是否是桥
    int timestamp;
    int edgeCnt;
    
    Bridge(int n) : n(n), adj(n + 1), dfn(n + 1, 0), 
                    low(n + 1, 0), timestamp(0), edgeCnt(0) {}
    
    // 【添加无向边】返回边的编号
    int addEdge(int u, int v) {
        int id = edgeCnt++;
        adj[u].push_back({v, id});
        adj[v].push_back({u, id});
        isBridge.push_back(false);
        return id;
    }
    
    void tarjan(int u, int parentEdge) {
        dfn[u] = low[u] = ++timestamp;
        
        for (auto [v, edgeId] : adj[u]) {
            if (dfn[v] == 0) {
                tarjan(v, edgeId);
                low[u] = min(low[u], low[v]);
                
                // 【桥判定】low[v] > dfn[u]
                // 意味着v无法不经过边(u,v)到达u或u的祖先
                if (low[v] > dfn[u]) {
                    isBridge[edgeId] = true;
                }
            } else if (edgeId != parentEdge) {
                // 通过其他边访问过v
                low[u] = min(low[u], dfn[v]);
            }
        }
    }
    
    void run() {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                tarjan(i, -1);
            }
        }
    }
    
    // 【获取所有桥的编号】
    vector<int> getBridges() {
        vector<int> result;
        for (int i = 0; i < edgeCnt; i++) {
            if (isBridge[i]) result.push_back(i);
        }
        return result;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板4】边双连通分量 (2-Edge-Connected Component)
// ═══════════════════════════════════════════════════════════
struct EdgeBCC {
    int n;
    vector<vector<pair<int, int>>> adj;
    vector<int> dfn, low;
    vector<int> bccId;       // 每个点属于哪个边双
    vector<bool> isBridge;
    stack<int> stk;
    int timestamp, edgeCnt, bccCnt;
    
    EdgeBCC(int n) : n(n), adj(n + 1), dfn(n + 1, 0), low(n + 1, 0),
                     bccId(n + 1, 0), timestamp(0), edgeCnt(0), bccCnt(0) {}
    
    int addEdge(int u, int v) {
        int id = edgeCnt++;
        adj[u].push_back({v, id});
        adj[v].push_back({u, id});
        isBridge.push_back(false);
        return id;
    }
    
    void tarjan(int u, int parentEdge) {
        dfn[u] = low[u] = ++timestamp;
        stk.push(u);
        
        for (auto [v, edgeId] : adj[u]) {
            if (dfn[v] == 0) {
                tarjan(v, edgeId);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    isBridge[edgeId] = true;
                }
            } else if (edgeId != parentEdge) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        
        // 边双的根：不通过桥能到达自己
        if (dfn[u] == low[u]) {
            bccCnt++;
            while (true) {
                int x = stk.top(); stk.pop();
                bccId[x] = bccCnt;
                if (x == u) break;
            }
        }
    }
    
    void run() {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                tarjan(i, -1);
            }
        }
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== Tarjan算法模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：强连通分量
    // 有向图：1→2→3→1（一个环）, 3→4
    // SCC: {1,2,3}, {4}
    // ─────────────────────────────────────────────────────
    {
        SCC scc(4);
        scc.addEdge(1, 2);
        scc.addEdge(2, 3);
        scc.addEdge(3, 1);
        scc.addEdge(3, 4);
        scc.run();
        
        assert(scc.getSccCount() == 2);
        assert(scc.getSccId(1) == scc.getSccId(2));
        assert(scc.getSccId(2) == scc.getSccId(3));
        assert(scc.getSccId(3) != scc.getSccId(4));
        
        cout << "[SCC] 测试通过，SCC数量=" << scc.getSccCount() << "\n";
        cout << "      节点1,2,3属于SCC " << scc.getSccId(1) 
             << "，节点4属于SCC " << scc.getSccId(4) << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：割点
    // 无向图：1-2-3-4, 2-5
    //         1---2---3---4
    //             |
    //             5
    // 割点：2, 3
    // ─────────────────────────────────────────────────────
    {
        CutVertex cv(5);
        cv.addEdge(1, 2);
        cv.addEdge(2, 3);
        cv.addEdge(3, 4);
        cv.addEdge(2, 5);
        cv.run();
        
        auto cuts = cv.getCutVertices();
        assert(cuts.size() == 2);
        assert(cv.isCut[2] && cv.isCut[3]);
        
        cout << "[割点] 测试通过，割点: ";
        for (int v : cuts) cout << v << " ";
        cout << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：割边/桥
    // 无向图：1-2-3, 2-4-5-2（4,5,2形成环）
    //         1---2---3
    //             |\
    //             4-5
    // 桥：(1,2), (2,3)
    // ─────────────────────────────────────────────────────
    {
        Bridge br(5);
        int e0 = br.addEdge(1, 2);
        int e1 = br.addEdge(2, 3);
        int e2 = br.addEdge(2, 4);
        int e3 = br.addEdge(4, 5);
        int e4 = br.addEdge(5, 2);
        br.run();
        
        auto bridges = br.getBridges();
        assert(bridges.size() == 2);
        assert(br.isBridge[e0] && br.isBridge[e1]);
        
        cout << "[桥] 测试通过，桥的数量=" << bridges.size() << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试4：缩点后建DAG
    // ─────────────────────────────────────────────────────
    {
        SCC scc(6);
        scc.addEdge(1, 2); scc.addEdge(2, 3); scc.addEdge(3, 1);  // SCC1: {1,2,3}
        scc.addEdge(4, 5); scc.addEdge(5, 6); scc.addEdge(6, 4);  // SCC2: {4,5,6}
        scc.addEdge(3, 4);  // SCC1 → SCC2
        scc.run();
        
        auto dag = scc.buildDAG();
        cout << "[缩点DAG] 测试通过，SCC数量=" << scc.getSccCount() << "\n";
    }
    
    cout << "\n===== 所有Tarjan测试通过 =====\n";
    return 0;
}