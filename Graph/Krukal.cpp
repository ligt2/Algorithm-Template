#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "最小生成树"、"连通所有点"
  ✅ 题目特征:
     • 无向图
     • 边数m较大
  
  应用场景:
  • 最小生成树（O(mlogm)）
  • 判断图是否连通
  
  典型题:
  • "连通所有城市的最小代价"
  • "修路最小花费"
  
  ❌ 反例: 需要边的具体连接方式 → Prim
=============================================================*/
// 并查集依赖（Kruskal必需）
struct DSU {
    vector<int> fa, sz;
    DSU(int n) : fa(n + 1), sz(n + 1, 1) { iota(fa.begin(), fa.end(), 0); }
    int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
    bool merge(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        fa[y] = x; sz[x] += sz[y];
        return true;
    }
};

// Kruskal 最小生成树 O(mlogm)
struct Kruskal {
    int n;
    struct Edge { int u, v, w; bool operator<(const Edge &o) const { return w < o.w; } };
    vector<Edge> edges;
    Kruskal(int n) : n(n) {}
    void add(int u, int v, int w) { edges.push_back({u, v, w}); }
    // 返回 {MST权值和, 边数}，边数<n-1说明不连通
    pair<ll, int> run() {
        sort(edges.begin(), edges.end());
        DSU dsu(n);
        ll sum = 0; int cnt = 0;
        for (auto &[u, v, w] : edges) {
            if (dsu.merge(u, v)) {
                sum += w; cnt++;
                if (cnt == n - 1) break;
            }
        }
        return {sum, cnt};
    }
};


int main() {
    // 4点无向：(1,2,1)(1,3,4)(2,3,2)(3,4,3) → MST=1+2+3=6
    Kruskal kr(4);
    kr.add(1,2,1); kr.add(1,3,4); 
    kr.add(2,3,2); kr.add(3,4,3);
    auto [sum, cnt] = kr.run();
    
    assert(sum == 6 && cnt == 3);
    cout << "[Kruskal] 测试通过，MST权值和=" << sum << "\n";
    return 0;
}