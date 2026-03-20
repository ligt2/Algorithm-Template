#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1e18;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "最短路"、"非负权边"
  ✅ 题目特征:
     • 边权 ≥0（重要！）
     • 单源最短路
  
  应用场景:
  • 非负权单源最短路（O((n+m)logn)）
  • 边权非负的最短路问题
  
  典型题:
  • "从起点s到所有点的最短距离"
  • "网格图最短路（边权≥0）"
  
  ❌ 反例: 有负权边 → SPFA / Bellman-Ford
=============================================================*/
struct Dijkstra {
    int n;
    vector<vector<pair<int, int>>> g;
    vector<ll> dist;

    Dijkstra(int n) : n(n), g(n + 1), dist(n + 1, INF) {}

    void add(int u, int v, int w) {
        g[u].emplace_back(v, w);
        // g[v].emplace_back(u, w); // 无向图打开此行
    }

    void run(int s) {
        fill(dist.begin(), dist.end(), INF);
        dist[s] = 0;
        priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto &[v, w] : g[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    ll query(int u) { return dist[u]; }
};

/*
测试图（有向）:
    1 --(2)--> 2 --(3)--> 4
    |        ^        ^
    (5)      |(1)     | (1)
    |        |        |
    v        |        |
    3 -------^-------^

    从节点1出发:
    1→2: 2
    1→3: 5,  但 1→2→...不能回3（有向）, 所以 dist[3]=5
    1→4: 1→3→4 = 5+1=6,  1→2→4 = 2+3=5, 取5

    预期输出: 0 2 5 5
*/

// Dijkstra dij(n);          // 1. 建图（节点编号 1~n）
// dij.add(u, v, w);         // 2. 加边（无向图在结构体内取消注释）
// dij.run(s);               // 3. 从 s 跑最短路
// ll ans = dij.query(t);    // 查询 s→t 的最短距离，INF 表示不可达
int main() {
    Dijkstra dij(4);
    dij.add(1, 2, 2);
    dij.add(1, 3, 5);
    dij.add(2, 4, 3);
    dij.add(3, 2, 1);
    dij.add(3, 4, 1);
    dij.run(1);
    // 预期: 0 2 5 5
    for (int i = 1; i <= 4; i++)
        cout << (dij.query(i) == INF ? -1 : dij.query(i)) << " \n"[i == 4];

    /*
    // ===== 正式提交用这段 =====
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, s;
    cin >> n >> m >> s;
    Dijkstra dij(n);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        dij.add(u, v, w);
    }
    dij.run(s);
    for (int i = 1; i <= n; i++)
        cout << (dij.query(i) == INF ? -1 : dij.query(i)) << " \n"[i == n];
    */

    return 0;
}