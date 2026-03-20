#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1e18;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "负权边"、"负环检测"
  ✅ 题目特征:
     • 边权可以为负
     • 需要判断负环
  
  应用场景:
  • 负权边最短路
  • 负环检测（入队次数>n）
  
  典型题:
  • "可能有负权边的最短路"
  • "判断图中是否有负环"
  
  ❌ 反例: 边权非负 → Dijkstra（更快）
=============================================================*/
// SPFA（可处理负权，可判负环）O(nm)最坏
struct SPFA {
    int n;
    vector<vector<pair<int,int>>> g;
    vector<ll> dist;
    vector<int> cnt;    // 入队次数，判负环
    vector<bool> inq;
    bool has_neg_cycle = false;

    SPFA(int n) : n(n), g(n + 1), dist(n + 1, INF), cnt(n + 1, 0), inq(n + 1, false) {}
    void add(int u, int v, int w) {
        g[u].emplace_back(v, w);
        // g[v].emplace_back(u, w); // 无向图取消注释
    }
    void run(int s) {
        fill(dist.begin(), dist.end(), INF);
        fill(cnt.begin(), cnt.end(), 0);
        fill(inq.begin(), inq.end(), false);
        dist[s] = 0; inq[s] = true; cnt[s] = 1;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop(); inq[u] = false;
            for (auto &[v, w] : g[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    if (!inq[v]) {
                        inq[v] = true;
                        if (++cnt[v] > n) { has_neg_cycle = true; return; }
                        q.push(v);
                    }
                }
            }
        }
    }
    ll query(int u) { return dist[u]; }
};


int main() {
    // 含负权边：1→2(1), 2→3(-3), 1→3(2) 起点1
    SPFA spfa(3);
    spfa.add(1,2,1); spfa.add(2,3,-3); spfa.add(1,3,2);
    spfa.run(1);
    
    assert(!spfa.has_neg_cycle);
    assert(spfa.query(1)==0 && spfa.query(2)==1 && spfa.query(3)==-2);
    cout << "[SPFA] 测试通过，结果：0 1 -2\n";
    return 0;
}