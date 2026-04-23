#include<bits/stdc++.h>
using ll = long long;
using ull = unsigned long long;
using u128 = unsigned __int128;
using i128 = __int128;
using namespace std;
const int INF = 0x3f3f3f3f;
/*=============================================================
  【二分图匹配 - 匈牙利算法 / Hopcroft-Karp】
  
  ⭐⭐⭐⭐ 省赛必会
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "二分图"、"匹配"
     • "两类对象配对"
     • "工人分配任务"
     • "相亲配对"
     • "最大独立集"（二分图最大独立集 = n - 最大匹配）
     • "最小点覆盖"（二分图最小点覆盖 = 最大匹配）
  
  ✅ 判断二分图：
     • 能否两种颜色染色（无奇环）
     • 明确的左右两部分（如男女、工人任务）
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 匈牙利算法：O(V × E)
  • Hopcroft-Karp：O(E × √V)（推荐）
  
  ═══════════════════════════════════════════════════════════
  【相关定理】
  
  • 最大匹配 = 最小点覆盖
  • 最大独立集 = n - 最大匹配
  • 最小边覆盖 = n - 最大匹配（无孤立点时）
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 棋盘覆盖
  • 任务分配
  • 最小点覆盖
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】匈牙利算法（简单易懂版）
// ═══════════════════════════════════════════════════════════
struct Hungarian {
    int n, m;                   // 左边n个点，右边m个点
    vector<vector<int>> adj;    // adj[u] = 左边u能匹配的右边点
    vector<int> matchL;         // matchL[u] = 左边u匹配的右边点
    vector<int> matchR;         // matchR[v] = 右边v匹配的左边点
    vector<bool> visR;          // 右边点在当前轮是否被访问
    
    // 【构造函数】左边n个点(1~n)，右边m个点(1~m)
    Hungarian(int n, int m) : n(n), m(m), adj(n + 1), 
                              matchL(n + 1, 0), matchR(m + 1, 0) {}
    
    // 【添加边】左边u可以匹配右边v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }
    
    // 【DFS寻找增广路】从左边u出发
    bool dfs(int u) {
        for (int v : adj[u]) {
            if (visR[v]) continue;
            visR[v] = true;
            
            // 【增广条件】
            // 1. 右边v未匹配
            // 2. 或v的当前匹配可以找到其他匹配
            if (matchR[v] == 0 || dfs(matchR[v])) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
        return false;
    }
    
    // 【求最大匹配】
    int maxMatch() {
        int result = 0;
        
        for (int u = 1; u <= n; u++) {
            visR.assign(m + 1, false);  // 每轮重置访问标记
            if (dfs(u)) result++;
        }
        
        return result;
    }
    
    // 【获取匹配结果】返回 {左边点, 右边点} 的配对
    vector<pair<int, int>> getMatching() {
        vector<pair<int, int>> result;
        for (int u = 1; u <= n; u++) {
            if (matchL[u] != 0) {
                result.push_back({u, matchL[u]});
            }
        }
        return result;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】Hopcroft-Karp算法（更快，推荐）
// ═══════════════════════════════════════════════════════════
struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> matchL, matchR;
    vector<int> distL;          // BFS分层距离
    
    HopcroftKarp(int n, int m) : n(n), m(m), adj(n + 1),
                                  matchL(n + 1, 0), matchR(m + 1, 0),
                                  distL(n + 1) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }
    
    // 【BFS分层】找到所有未匹配左点到未匹配右点的最短路
    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= n; u++) {
            if (matchL[u] == 0) {
                distL[u] = 0;
                q.push(u);
            } else {
                distL[u] = INT_MAX;
            }
        }
        
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                int next = matchR[v];  // v的当前匹配
                if (next == 0) {
                    found = true;  // 找到增广路终点
                } else if (distL[next] == INT_MAX) {
                    distL[next] = distL[u] + 1;
                    q.push(next);
                }
            }
        }
        return found;
    }
    
    // 【DFS增广】沿着分层图找增广路
    bool dfs(int u) {
        for (int v : adj[u]) {
            int next = matchR[v];
            if (next == 0 || (distL[next] == distL[u] + 1 && dfs(next))) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
        distL[u] = INT_MAX;  // 标记为已处理
        return false;
    }
    
    int maxMatch() {
        int result = 0;
        
        while (bfs()) {
            for (int u = 1; u <= n; u++) {
                if (matchL[u] == 0 && dfs(u)) {
                    result++;
                }
            }
        }
        
        return result;
    }
    
    vector<pair<int, int>> getMatching() {
        vector<pair<int, int>> result;
        for (int u = 1; u <= n; u++) {
            if (matchL[u] != 0) {
                result.push_back({u, matchL[u]});
            }
        }
        return result;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】二分图判定 + 染色
// ═══════════════════════════════════════════════════════════
struct BipartiteCheck {
    int n;
    vector<vector<int>> adj;
    vector<int> color;  // -1未访问，0和1表示两种颜色
    
    BipartiteCheck(int n) : n(n), adj(n + 1), color(n + 1, -1) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    bool bfs(int start) {
        queue<int> q;
        q.push(start);
        color[start] = 0;
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false;  // 存在奇环
                }
            }
        }
        return true;
    }
    
    // 【判断是否是二分图】
    bool isBipartite() {
        for (int i = 1; i <= n; i++) {
            if (color[i] == -1) {
                if (!bfs(i)) return false;
            }
        }
        return true;
    }
    
    // 【获取两部分的点】
    pair<vector<int>, vector<int>> getParts() {
        vector<int> left, right;
        for (int i = 1; i <= n; i++) {
            if (color[i] == 0) left.push_back(i);
            else if (color[i] == 1) right.push_back(i);
        }
        return {left, right};
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板4】带权二分图最大匹配（KM算法）
// ═══════════════════════════════════════════════════════════
struct KM {
    int n;
    vector<vector<ll>> w;       // w[i][j] = 左i到右j的权值
    vector<ll> la, lb;          // 顶标
    vector<int> matchL, matchR;
    vector<bool> visL, visR;
    vector<ll> slack;           // 松弛量
    const ll INF = 1e18;
    
    KM(int n) : n(n), w(n + 1, vector<ll>(n + 1, -INF)),
                la(n + 1, -INF), lb(n + 1, 0),
                matchL(n + 1, 0), matchR(n + 1, 0),
                visL(n + 1), visR(n + 1), slack(n + 1) {}
    
    void addEdge(int u, int v, ll weight) {
        w[u][v] = max(w[u][v], weight);
        la[u] = max(la[u], weight);
    }
    
    bool dfs(int u) {
        visL[u] = true;
        for (int v = 1; v <= n; v++) {
            if (visR[v]) continue;
            ll gap = la[u] + lb[v] - w[u][v];
            if (gap == 0) {
                visR[v] = true;
                if (matchR[v] == 0 || dfs(matchR[v])) {
                    matchL[u] = v;
                    matchR[v] = u;
                    return true;
                }
            } else {
                slack[v] = min(slack[v], gap);
            }
        }
        return false;
    }
    
    ll solve() {
        for (int u = 1; u <= n; u++) {
            fill(slack.begin(), slack.end(), INF);
            while (true) {
                fill(visL.begin(), visL.end(), false);
                fill(visR.begin(), visR.end(), false);
                if (dfs(u)) break;
                
                ll d = INF;
                for (int v = 1; v <= n; v++) {
                    if (!visR[v]) d = min(d, slack[v]);
                }
                
                for (int i = 1; i <= n; i++) {
                    if (visL[i]) la[i] -= d;
                    if (visR[i]) lb[i] += d;
                    else slack[i] -= d;
                }
            }
        }
        
        ll ans = 0;
        for (int u = 1; u <= n; u++) {
            if (matchL[u] && w[u][matchL[u]] > -INF) {
                ans += w[u][matchL[u]];
            }
        }
        return ans;
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 二分图匹配模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：匈牙利算法
    // 左边3人，右边3任务
    // 1可做任务1,2  2可做任务2  3可做任务3
    // 最大匹配：1-1, 2-2, 3-3 = 3
    // ─────────────────────────────────────────────────────
    {
        Hungarian hg(3, 3);
        hg.addEdge(1, 1);
        hg.addEdge(1, 2);
        hg.addEdge(2, 2);
        hg.addEdge(3, 3);
        
        int match = hg.maxMatch();
        assert(match == 3);
        
        cout << "[匈牙利] 测试通过，最大匹配=" << match << "\n";
        cout << "         匹配方案: ";
        for (auto [u, v] : hg.getMatching()) {
            cout << "(" << u << "," << v << ") ";
        }
        cout << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：Hopcroft-Karp
    // ─────────────────────────────────────────────────────
    {
        HopcroftKarp hk(3, 3);
        hk.addEdge(1, 1);
        hk.addEdge(1, 2);
        hk.addEdge(2, 2);
        hk.addEdge(3, 3);
        
        int match = hk.maxMatch();
        assert(match == 3);
        cout << "[Hopcroft-Karp] 测试通过，最大匹配=" << match << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：二分图判定
    // 图：1-2, 2-3, 3-4, 4-1（四边形，是二分图）
    // ─────────────────────────────────────────────────────
    {
        BipartiteCheck bc(4);
        bc.addEdge(1, 2);
        bc.addEdge(2, 3);
        bc.addEdge(3, 4);
        bc.addEdge(4, 1);
        
        assert(bc.isBipartite() == true);
        cout << "[二分图判定-偶环] 测试通过，是二分图\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试4：非二分图（三角形）
    // ─────────────────────────────────────────────────────
    {
        BipartiteCheck bc(3);
        bc.addEdge(1, 2);
        bc.addEdge(2, 3);
        bc.addEdge(3, 1);
        
        assert(bc.isBipartite() == false);
        cout << "[二分图判定-奇环] 测试通过，不是二分图\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试5：KM算法（带权最大匹配）
    // 3人3任务，权值矩阵：
    //     任务1 任务2 任务3
    // 人1   3     4     6
    // 人2   4     5     3
    // 人3   2     3     2
    // 最优：人1-任务3(6), 人2-任务2(5), 人3-任务1(2) = 13
    // ─────────────────────────────────────────────────────
    {
        KM km(3);
        km.addEdge(1, 1, 3); km.addEdge(1, 2, 4); km.addEdge(1, 3, 6);
        km.addEdge(2, 1, 4); km.addEdge(2, 2, 5); km.addEdge(2, 3, 3);
        km.addEdge(3, 1, 2); km.addEdge(3, 2, 3); km.addEdge(3, 3, 2);
        
        ll maxWeight = km.solve();
        assert(maxWeight == 13);
        cout << "[KM带权匹配] 测试通过，最大权值=" << maxWeight << "\n";
    }
    
    cout << "\n===== 所有二分图匹配测试通过 =====\n";
    return 0;
}