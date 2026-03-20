#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1e18;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "任意两点最短路"、"传递闭包"
  ✅ 题目特征:
     • n≤500（O(n³)复杂度）
     • 需要所有点对最短路
  
  应用场景:
  • 全源最短路（n≤500）
  • 传递闭包
  • 判断图的连通性
  
  典型题:
  • "任意两点间最短距离"
  • "经过中转点的最短路"
  
  ❌ 反例: n>500 → 多次Dijkstra
=============================================================*/
// Floyd（全源最短路）O(n³) 适用 n≤500
struct Floyd {
    int n;
    vector<vector<ll>> d;
    Floyd(int n) : n(n), d(n + 1, vector<ll>(n + 1, INF)) {
        for (int i = 0; i <= n; i++) d[i][i] = 0;
    }
    void add(int u, int v, int w) {
        d[u][v] = min(d[u][v], (ll)w);
        // d[v][u] = min(d[v][u], (ll)w); // 无向图取消注释
    }
    void run() {
        for (int k = 1; k <= n; k++)
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    }
    ll query(int u, int v) { return d[u][v]; }
};


int main() {
    // 3个点：1→2(1), 2→3(2), 1→3(10)
    Floyd fl(3);
    fl.add(1,2,1); fl.add(2,3,2); fl.add(1,3,10);
    fl.run();
    
    assert(fl.query(1,3)==3 && fl.query(1,2)==1);
    cout << "[Floyd] 测试通过，d(1,3)=" << fl.query(1,3) << "\n";
    return 0;
}