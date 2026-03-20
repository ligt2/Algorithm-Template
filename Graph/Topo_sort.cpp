#include<bits/stdc++.h>
using namespace std;
/*=============================================================
  什么时候用？
  
  ✅ 明确信号词: "依赖关系"、"课程安排"、"DAG"
  ✅ 题目特征:
     • 有向无环图（DAG）
     • 需要排序满足依赖
  
  应用场景:
  • 拓扑排序（BFS/DFS）
  • 判断是否有环
  • 依赖关系排序
  
  典型题:
  • "课程安排（先修课）"
  • "任务调度（依赖关系）"
  
  ❌ 反例: 有环图 → 判环/强连通分量
=============================================================*/
// 拓扑排序（DAG判定 + 排序）O(n+m)
struct TopoSort {
    int n;
    vector<vector<int>> g;
    vector<int> ind, order;
    TopoSort(int n) : n(n), g(n + 1), ind(n + 1, 0) {}
    void add(int u, int v) { g[u].push_back(v); ind[v]++; }
    // 返回 true 表示是 DAG，order 为拓扑序
    bool run() {
        queue<int> q;
        for (int i = 1; i <= n; i++) if (!ind[i]) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : g[u]) if (--ind[v] == 0) q.push(v);
        }
        return (int)order.size() == n; // false → 有环
    }
};


int main() {
    // DAG：1→2, 1→3, 2→4, 3→4 → 拓扑序长度=4
    TopoSort ts(4);
    ts.add(1,2); ts.add(1,3); ts.add(2,4); ts.add(3,4);
    bool is_dag = ts.run();
    
    assert(is_dag && ts.order.size() == 4);
    cout << "[TopoSort] 测试通过，拓扑序：";
    for (int x : ts.order) cout << x << " ";
    cout << "\n";
    return 0;
}