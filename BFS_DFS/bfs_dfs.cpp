#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  BFS/DFS 遍历与功能性算法 — ICPC完整模板
  
  ========== 做题决策树 ==========
  题目问什么?
  │
  ├─ "最短路/最少步数"
  │   ├─ 无权图        → ① BFS
  │   ├─ 边权∈{0,1}   → ③ 0-1 BFS  
  │   └─ 非负权        → Dijkstra
  │
  ├─ "连通/分块/岛屿"
  │   ├─ 静态图        → ⑤ 连通分量
  │   └─ 动态加边      → DSU
  │
  ├─ "有无环"
  │   ├─ 无向图        → ⑥ DFS parent检测
  │   └─ 有向图        → ⑦ 三色DFS
  │
  ├─ "拓扑序/依赖/DAG" → ⑧ Kahn's BFS
  ├─ "二分图/二染色"   → ⑨ BFS染色
  ├─ "删边/删点断连"   → ⑩ Tarjan割边割点
  ├─ "强连通/缩点"     → ⑪ Tarjan SCC
  └─ "一笔画"          → ⑫ 欧拉路径
  
  ========== 常见陷阱 ⚠️ ==========
  1. 图不连通 → 所有算法都要 for(1..n) 遍历每个起点
  2. 统一1-indexed（本模板）
  3. DFS递归 n>10^5 → 考虑手动栈
  4. 无向图邻接表 → 每条边存两次
  5. 有向/无向环检测方法完全不同!
=============================================================*/


// ==================== ① BFS 单源最短路 ====================
/*=============================================================
  ✅ 什么时候用:
     • 所有边权=1（或无权）的最短路
     • "最少步数"、"最短距离"
  
  返回: dist[], dist[v]=-1 表示不可达
  
  ✅ 路径还原: 加 par[] 数组
     par[v]=u 表示v由u扩展
     从终点沿par倒推还原路径
  
  复杂度: O(V+E)
=============================================================*/
vector<int> bfs(const vector<vector<int>>& adj, int src) {
    int n = adj.size();
    vector<int> dist(n, -1);
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
    return dist;
}

// 带路径还原版本
vector<int> bfsWithPath(const vector<vector<int>>& adj,
                         int src, int dst) {
    int n = adj.size();
    vector<int> dist(n, -1), par(n, -1);
    queue<int> q;
    dist[src] = 0; q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                par[v] = u;
                q.push(v);
            }
    }
    // 还原路径
    vector<int> path;
    if (dist[dst] == -1) return path; // 不可达
    for (int v = dst; v != -1; v = par[v])
        path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}


// ==================== ② 多源 BFS ====================
/*=============================================================
  ✅ 什么时候用:
     • 多个起点同时扩散
     • 每个点到最近源点的距离
     • "腐烂的橘子"、"最近出口"
  
  做法: 所有源点同时入队，dist全设0
  复杂度: O(V+E)
=============================================================*/
vector<int> multiBFS(const vector<vector<int>>& adj,
                     const vector<int>& sources) {
    int n = adj.size();
    vector<int> dist(n, -1);
    queue<int> q;
    for (int s : sources) {
        dist[s] = 0;
        q.push(s);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
    return dist;
}


// ==================== ③ 0-1 BFS ====================
/*=============================================================
  ✅ 什么时候用:
     • 边权只有 0 和 1
     • "翻转最少次数"（翻=1，不翻=0）
     • 比Dijkstra更快：O(V+E)
  
  做法: 双端队列 deque
        权0 → push_front（优先处理）
        权1 → push_back
  
  ⚠️ 同一节点可能被多次松弛
     不能用visited数组，要用dist比较
  
  adj[u] = {(v, w)}，w ∈ {0,1}
=============================================================*/
vector<int> bfs01(const vector<vector<pair<int,int>>>& adj,
                  int src) {
    int n = adj.size();
    vector<int> dist(n, INT_MAX);
    deque<int> dq;
    dist[src] = 0;
    dq.push_back(src);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else        dq.push_back(v);
            }
        }
    }
    return dist;
}


// ==================== ④ 网格 BFS ====================
/*=============================================================
  ✅ 什么时候用:
     • 二维网格最短路/洪水填充/岛屿计数
     • "迷宫"、"地图"、"矩阵最短路径"
  
  ⚠️ 常见坑:
     • 方向数组写错或漏方向
     • 忘记边界检查
     • 起点本身是障碍
  
  '#'=障碍，'.'=可通行（按题意修改）
  返回: dist[r][c]=-1 表示不可达
=============================================================*/
vector<vector<int>> gridBFS(const vector<string>& grid,
                             int sr, int sc) {
    int R = grid.size(), C = grid[0].size();
    vector<vector<int>> dist(R, vector<int>(C, -1));
    if (grid[sr][sc] == '#') return dist;
    
    // 四方向（需要八方向则继续添加）
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    queue<pair<int,int>> q;
    dist[sr][sc] = 0;
    q.push({sr, sc});
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx >= 0 && nx < R &&
                ny >= 0 && ny < C &&
                grid[nx][ny] != '#' &&
                dist[nx][ny] == -1) {
                dist[nx][ny] = dist[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
    return dist;
}


// ==================== ⑤ 连通分量（无向图）====================
/*=============================================================
  ✅ 什么时候用:
     • "有几个连通块"
     • "哪些点在同一块"
     • 静态图一次性求解
  
  返回: {comp[], 分量个数}
        comp[u] = u所在分量编号（从0开始）
  
  ❌ 动态加边 → DSU
  复杂度: O(V+E)
=============================================================*/
pair<vector<int>, int> getComponents(
        const vector<vector<int>>& adj, int n) {
    vector<int> comp(n + 1, -1);
    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        if (comp[i] != -1) continue;
        queue<int> q;
        q.push(i); comp[i] = cnt;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u])
                if (comp[v] == -1) {
                    comp[v] = cnt;
                    q.push(v);
                }
        }
        cnt++;
    }
    return {comp, cnt};
}


// ==================== ⑥ 环检测——无向图 ====================
/*=============================================================
  ✅ 什么时候用:
     • "无向图是否有环"
     • "是否是树"（连通+无环 ⟺ 连通+E=V-1）
  
  方法: DFS，遇到已访问且非父节点的邻居 → 有环
  
  ⚠️ skipped标记说明:
     简单图中父节点只出现一次，只跳过一次
     多重边时两条相同边本身构成环
     → 只跳一条才能正确检测多重边的环
  
  复杂度: O(V+E)
=============================================================*/
bool hasCycleUndirected(const vector<vector<int>>& adj, int n) {
    vector<int> vis(n + 1, 0);
    bool found = false;
    
    function<void(int, int)> dfs = [&](int u, int fa) {
        vis[u] = 1;
        bool skipped = false; // 只跳过一条回父边
        for (int v : adj[u]) {
            if (found) return;
            if (v == fa && !skipped) {
                skipped = true;
                continue;
            }
            if (vis[v]) { found = true; return; }
            dfs(v, u);
        }
    };
    
    for (int i = 1; i <= n && !found; i++)
        if (!vis[i]) dfs(i, 0); // 0作为哨兵（1-indexed）
    
    return found;
}


// ==================== ⑦ 环检测——有向图（三色DFS）====================
/*=============================================================
  ✅ 什么时候用:
     • "有向图是否有环"
     • "能否拓扑排序"（无环 ⟺ 存在拓扑序）
  
  三色含义:
    白(0) = 未访问
    灰(1) = 在当前DFS路径上（递归栈中）← 关键
    黑(2) = 已完全处理
  
  遇到灰色 → 发现后向边 → 有环！
  遇到黑色 → 安全，跳过
  
  ⚠️ 无向图不能用三色法！
     无向图的"回边"就是父边，会误判
  
  复杂度: O(V+E)
=============================================================*/
bool hasCycleDirected(const vector<vector<int>>& adj, int n) {
    vector<int> color(n + 1, 0);
    bool found = false;
    
    function<void(int)> dfs = [&](int u) {
        color[u] = 1; // 染灰
        for (int v : adj[u]) {
            if (found) return;
            if (color[v] == 1) { found = true; return; } // 遇灰=有环
            if (color[v] == 0) dfs(v);
            // 遇黑=已处理，跳过
        }
        color[u] = 2; // 染黑
    };
    
    for (int i = 1; i <= n && !found; i++)
        if (color[i] == 0) dfs(i);
    
    return found;
}


// ==================== ⑨ 二分图判定（BFS染色）====================
/*=============================================================
  ✅ 什么时候用:
     • "能否分成两组使同组无边"
     • 二分图匹配的前置判断
     • "是否有奇环"（有奇环 ⟺ 非二分图）
  
  方法: BFS给节点交替染0/1两色
        相邻节点同色 → 非二分图
  
  返回: {是否二分图, color[]}
        color[u]=0或1表示所在组别
  
  复杂度: O(V+E)
=============================================================*/
pair<bool, vector<int>> bipartiteCheck(
        const vector<vector<int>>& adj, int n) {
    vector<int> color(n + 1, -1);
    bool ok = true;
    
    for (int i = 1; i <= n && ok; i++) {
        if (color[i] != -1) continue;
        queue<int> q;
        color[i] = 0; q.push(i);
        while (!q.empty() && ok) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u]; // 染相反色
                    q.push(v);
                } else if (color[v] == color[u]) {
                    ok = false; // 同色相邻，非二分图
                }
            }
        }
    }
    return {ok, color};
}



// ==================== 测试 main() ====================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cout << "========== ① BFS 单源 ==========\n";
    {
        // 图: 1-2-3-4-5 (链)
        vector<vector<int>> adj(6);
        auto addE = [&](int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        };
        addE(1,2); addE(2,3); addE(3,4); addE(4,5);
        auto d = bfs(adj, 1);
        // 期望: d[1]=0,d[2]=1,d[3]=2,d[4]=3,d[5]=4
        assert(d[1]==0 && d[3]==2 && d[5]==4);
        cout << "从1出发: ";
        for(int i=1;i<=5;i++) cout<<"d["<<i<<"]="<<d[i]<<" ";
        cout << "\n[BFS] ✅ 通过\n\n";
    }

    cout << "========== ② 多源 BFS ==========\n";
    {
        // 图: 1-2-3-4-5，源点={1,5}
        vector<vector<int>> adj(6);
        auto addE = [&](int u, int v) {
            adj[u].push_back(v); adj[v].push_back(u);
        };
        addE(1,2); addE(2,3); addE(3,4); addE(4,5);
        auto d = multiBFS(adj, {1, 5});
        // 期望: d[1]=0,d[2]=1,d[3]=2,d[4]=1,d[5]=0
        assert(d[1]==0 && d[3]==2 && d[5]==0 && d[4]==1);
        cout << "源点{1,5}: ";
        for(int i=1;i<=5;i++) cout<<"d["<<i<<"]="<<d[i]<<" ";
        cout << "\n[多源BFS] ✅ 通过\n\n";
    }

    cout << "========== ③ 0-1 BFS ==========\n";
    {
        // 1→2(w=1), 1→3(w=0), 3→2(w=0)
        // 从1到2: 直接走1→2花费1，走1→3→2花费0
        vector<vector<pair<int,int>>> adj(4);
        adj[1].push_back({2,1});
        adj[1].push_back({3,0});
        adj[3].push_back({2,0});
        auto d = bfs01(adj, 1);
        // 期望: d[2]=0（经过3）,d[3]=0
        assert(d[2]==0 && d[3]==0);
        cout << "d[2]=" << d[2] << " d[3]=" << d[3] << "\n";
        cout << "[0-1 BFS] ✅ 通过\n\n";
    }

    cout << "========== ④ 网格 BFS ==========\n";
    {
        vector<string> grid = {
            ".....",
            ".###.",
            ".....",
        };
        auto d = gridBFS(grid, 0, 0);
        // (0,0)到(2,4): 需要绕开障碍
        cout << "d[0][0]=" << d[0][0]
             << " d[2][4]=" << d[2][4] << "\n";
        assert(d[0][0] == 0);
        assert(d[2][4] > 0); // 可达
        cout << "[网格BFS] ✅ 通过\n\n";
    }

    cout << "========== ⑤ 连通分量 ==========\n";
    {
        // 两个分量: {1,2,3} 和 {4,5}
        vector<vector<int>> adj(6);
        auto addE = [&](int u, int v) {
            adj[u].push_back(v); adj[v].push_back(u);
        };
        addE(1,2); addE(2,3); addE(4,5);
        auto [comp, cnt] = getComponents(adj, 5);
        assert(cnt == 2);
        assert(comp[1]==comp[2] && comp[2]==comp[3]);
        assert(comp[4]==comp[5]);
        assert(comp[1]!=comp[4]);
        cout << "分量数=" << cnt << "\n";
        cout << "[连通分量] ✅ 通过\n\n";
    }

    cout << "========== ⑥ 环检测（无向）==========\n";
    {
        // 无环：树 1-2-3-4
        {
            vector<vector<int>> adj(5);
            auto addE = [&](int u, int v) {
                adj[u].push_back(v); adj[v].push_back(u);
            };
            addE(1,2); addE(2,3); addE(3,4);
            assert(!hasCycleUndirected(adj, 4));
            cout << "树形图（无环）: 无环 ✅\n";
        }
        // 有环：三角形 1-2-3-1
        {
            vector<vector<int>> adj(4);
            auto addE = [&](int u, int v) {
                adj[u].push_back(v); adj[v].push_back(u);
            };
            addE(1,2); addE(2,3); addE(3,1);
            assert(hasCycleUndirected(adj, 3));
            cout << "三角形（有环）: 有环 ✅\n";
        }
        cout << "[无向环检测] ✅ 通过\n\n";
    }

    cout << "========== ⑦ 环检测（有向）==========\n";
    {
        // 无环：DAG 1→2→3
        {
            vector<vector<int>> adj(4);
            adj[1].push_back(2); adj[2].push_back(3);
            assert(!hasCycleDirected(adj, 3));
            cout << "DAG 1→2→3（无环）: 无环 ✅\n";
        }
        // 有环：1→2→3→1
        {
            vector<vector<int>> adj(4);
            adj[1].push_back(2);
            adj[2].push_back(3);
            adj[3].push_back(1);
            assert(hasCycleDirected(adj, 3));
            cout << "1→2→3→1（有环）: 有环 ✅\n";
        }
        cout << "[有向环检测] ✅ 通过\n\n";
    }


    cout << "========== ⑨ 二分图判定 ==========\n";
    {
        // 二分图：路径 1-2-3-4
        {
            vector<vector<int>> adj(5);
            auto addE = [&](int u, int v) {
                adj[u].push_back(v); adj[v].push_back(u);
            };
            addE(1,2); addE(2,3); addE(3,4);
            auto [ok, color] = bipartiteCheck(adj, 4);
            assert(ok);
            cout << "路径图（是二分图）: ✅\n";
        }
        // 非二分图：奇环三角形
        {
            vector<vector<int>> adj(4);
            auto addE = [&](int u, int v) {
                adj[u].push_back(v); adj[v].push_back(u);
            };
            addE(1,2); addE(2,3); addE(3,1);
            auto [ok, color] = bipartiteCheck(adj, 3);
            assert(!ok);
            cout << "三角形（非二分图）: ✅\n";
        }
        cout << "[二分图判定] ✅ 通过\n\n";
    }



    cout << "========== 全部测试通过 ✅ ==========\n";
    return 0;
}