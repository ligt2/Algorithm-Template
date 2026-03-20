#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "连通性"、"合并集合"、"动态连通"
  ✅ 题目特征:
     • 判断两点是否连通
     • 动态加边
  
  应用场景:
  • 判断连通性
  • Kruskal最小生成树
  • 最小生成树边数统计
  • 连通块大小查询
  
  典型题:
  • "加边后判断两点是否连通"
  • "朋友圈个数"
  
  ❌ 反例: 需要删边 → 线段树分治
=============================================================*/
// 并查集 DSU（基础中的基础，Kruskal/连通性都要用）
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
    bool same(int x, int y) { return find(x) == find(y); }
    int size(int x) { return sz[find(x)]; }
};


int main() {
    // 合并(1,2)(2,3)，判1和3连通、1和4不连通
    DSU dsu(5);
    dsu.merge(1, 2); 
    dsu.merge(2, 3);
    
    assert(dsu.same(1, 3) && !dsu.same(1, 4));
    assert(dsu.size(1) == 3);
    cout << "[DSU] 测试通过\n";
    return 0;
}