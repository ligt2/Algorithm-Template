#include <bits/stdc++.h>
using namespace std;

/*=============================================================
  【欧拉回路 / 欧拉路径 / 2-SAT】
  
  ⭐⭐☆ 省赛低频率
  
  ═══════════════════════════════════════════════════════════
  【欧拉路径/回路 - 什么时候用？】
  
  ✅ 信号词：
     • "一笔画"
     • "经过每条边恰好一次"
     • "不重复走完所有边"
  
  ✅ 存在条件：
     • 欧拉回路（无向）：所有点度数为偶数
     • 欧拉回路（有向）：所有点入度=出度
     • 欧拉路径（无向）：恰好0或2个奇度点
     • 欧拉路径（有向）：恰好一个出度-入度=1，一个入度-出度=1
  
  ═══════════════════════════════════════════════════════════
  【2-SAT - 什么时候用？】
  
  ✅ 信号词：
     • "布尔变量满足约束"
     • "二选一"、"要么...要么..."
     • "x和y至少一个为真"
     • "x为真则y必须为假"
  
  ✅ 建图方式：
     • x ∨ y：¬x → y, ¬y → x
     • x ⊕ y：x → ¬y, y → ¬x, ¬x → y, ¬y → x
     • x = y：x → y, y → x, ¬x → ¬y, ¬y → ¬x
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】欧拉回路/路径（无向图）
// ═══════════════════════════════════════════════════════════
struct EulerUndirected {
    int n;
    vector<vector<pair<int, int>>> adj;  // adj[u] = {(v, edgeId), ...}
    vector<bool> used;          // 边是否已使用
    vector<int> path;           // 欧拉路径
    int edgeCnt;
    
    EulerUndirected(int n) : n(n), adj(n + 1), edgeCnt(0) {}
    
    void addEdge(int u, int v) {
        int id = edgeCnt++;
        adj[u].push_back({v, id});
        adj[v].push_back({u, id});
        used.push_back(false);
    }
    
    // 【Hierholzer算法】
    void dfs(int u) {
        while (!adj[u].empty()) {
            auto [v, id] = adj[u].back();
            adj[u].pop_back();
            
            if (used[id]) continue;
            used[id] = true;
            
            dfs(v);
        }
        path.push_back(u);
    }
    
    // 【检查是否存在欧拉回路】
    bool hasEulerCircuit() {
        for (int i = 1; i <= n; i++) {
            if (adj[i].size() % 2 != 0) return false;
        }
        return true;
    }
    
    // 【检查是否存在欧拉路径】返回起点（0表示不存在）
    int hasEulerPath() {
        int odd = 0, start = 1;
        for (int i = 1; i <= n; i++) {
            if (adj[i].size() % 2 != 0) {
                odd++;
                start = i;
            }
        }
        if (odd == 0 || odd == 2) return start;
        return 0;
    }
    
    // 【求欧拉路径/回路】
    vector<int> solve(int start = 0) {
        if (start == 0) start = hasEulerPath();
        if (start == 0) return {};  // 不存在
        
        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());
        return path;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】欧拉回路/路径（有向图）
// ═══════════════════════════════════════════════════════════
struct EulerDirected {
    int n;
    vector<vector<int>> adj;
    vector<int> path;
    vector<int> inDeg, outDeg;
    
    EulerDirected(int n) : n(n), adj(n + 1), inDeg(n + 1, 0), outDeg(n + 1, 0) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        outDeg[u]++;
        inDeg[v]++;
    }
    
    void dfs(int u) {
        while (!adj[u].empty()) {
            int v = adj[u].back();
            adj[u].pop_back();
            dfs(v);
        }
        path.push_back(u);
    }
    
    // 【检查欧拉回路】
    bool hasEulerCircuit() {
        for (int i = 1; i <= n; i++) {
            if (inDeg[i] != outDeg[i]) return false;
        }
        return true;
    }
    
    // 【检查欧拉路径】返回{起点, 终点}，{0,0}表示不存在
    pair<int, int> hasEulerPath() {
        int start = 0, end = 0;
        int diffPlus = 0, diffMinus = 0;
        
        for (int i = 1; i <= n; i++) {
            int diff = outDeg[i] - inDeg[i];
            if (diff == 1) { start = i; diffPlus++; }
            else if (diff == -1) { end = i; diffMinus++; }
            else if (diff != 0) return {0, 0};
        }
        
        if (diffPlus == 0 && diffMinus == 0) {
            // 欧拉回路，任意起点
            for (int i = 1; i <= n; i++) {
                if (outDeg[i] > 0) return {i, i};
            }
        }
        if (diffPlus == 1 && diffMinus == 1) return {start, end};
        return {0, 0};
    }
    
    vector<int> solve(int start = 0) {
        if (start == 0) {
            auto [s, e] = hasEulerPath();
            start = s;
        }
        if (start == 0) return {};
        
        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());
        return path;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】2-SAT
// ═══════════════════════════════════════════════════════════
struct TwoSAT {
    int n;                      // 变量个数
    vector<vector<int>> adj;    // 蕴含图
    vector<int> dfn, low;
    vector<int> sccId;
    vector<bool> inStack;
    stack<int> stk;
    int timestamp, sccCnt;
    
    // 【编号规则】
    // 变量i的"真"状态：编号 2*i
    // 变量i的"假"状态：编号 2*i+1
    
    TwoSAT(int n) : n(n), adj(2 * n + 2), dfn(2 * n + 2, 0), 
                    low(2 * n + 2, 0), sccId(2 * n + 2, 0),
                    inStack(2 * n + 2, false), timestamp(0), sccCnt(0) {}
    
    // 【内部编号】
    int trueNode(int i) { return 2 * i; }
    int falseNode(int i) { return 2 * i + 1; }
    int negNode(int node) { return node ^ 1; }
    
    // 【添加蕴含边】a → b
    void addImplication(int a, int b) {
        adj[a].push_back(b);
    }
    
    // 【添加约束】至少一个为真：x ∨ y
    // valX: true表示x为真，false表示x为假
    void addOr(int x, bool valX, int y, bool valY) {
        int a = valX ? trueNode(x) : falseNode(x);
        int b = valY ? trueNode(y) : falseNode(y);
        // ¬a → b, ¬b → a
        addImplication(negNode(a), b);
        addImplication(negNode(b), a);
    }
    
    // 【添加约束】恰好一个为真：x ⊕ y
    void addXor(int x, bool valX, int y, bool valY) {
        addOr(x, valX, y, valY);         // 至少一个
        addOr(x, !valX, y, !valY);       // 至少一个为假
    }
    
    // 【添加约束】两个相同：x = y
    void addEqual(int x, bool valX, int y, bool valY) {
        addOr(x, valX, y, !valY);        // x → y
        addOr(x, !valX, y, valY);        // ¬x → ¬y
    }
    
    // 【添加约束】x必须为某个值
    void addForce(int x, bool val) {
        addOr(x, val, x, val);
    }
    
    // 【Tarjan求SCC】
    void tarjan(int u) {
        dfn[u] = low[u] = ++timestamp;
        stk.push(u);
        inStack[u] = true;
        
        for (int v : adj[u]) {
            if (dfn[v] == 0) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (inStack[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        
        if (dfn[u] == low[u]) {
            sccCnt++;
            while (true) {
                int x = stk.top(); stk.pop();
                inStack[x] = false;
                sccId[x] = sccCnt;
                if (x == u) break;
            }
        }
    }
    
    // 【求解】返回是否有解
    bool solve() {
        for (int i = 0; i <= 2 * n + 1; i++) {
            if (dfn[i] == 0) tarjan(i);
        }
        
        // 检查：变量i的真和假不能在同一个SCC
        for (int i = 1; i <= n; i++) {
            if (sccId[trueNode(i)] == sccId[falseNode(i)]) {
                return false;
            }
        }
        return true;
    }
    
    // 【获取解】变量i的取值
    bool getValue(int i) {
        // SCC编号越大，拓扑序越小
        // 选择拓扑序更大的那个
        return sccId[trueNode(i)] > sccId[falseNode(i)];
    }
    
    // 【获取所有变量的取值】
    vector<bool> getSolution() {
        vector<bool> result(n + 1);
        for (int i = 1; i <= n; i++) {
            result[i] = getValue(i);
        }
        return result;
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 欧拉回路/2-SAT模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：无向图欧拉回路
    // 图：1-2, 2-3, 3-1（三角形，所有点度数为2）
    // ─────────────────────────────────────────────────────
    {
        EulerUndirected eu(3);
        eu.addEdge(1, 2);
        eu.addEdge(2, 3);
        eu.addEdge(3, 1);
        
        assert(eu.hasEulerCircuit() == true);
        auto path = eu.solve(1);
        assert(path.size() == 4 && path.front() == path.back());
        
        cout << "[无向图欧拉回路] 测试通过，路径: ";
        for (int v : path) cout << v << " ";
        cout << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：有向图欧拉路径
    // 图：1→2, 2→3, 3→1, 1→3
    // ─────────────────────────────────────────────────────
    {
        EulerDirected ed(3);
        ed.addEdge(1, 2);
        ed.addEdge(2, 3);
        ed.addEdge(3, 1);
        ed.addEdge(1, 3);
        
        auto [start, end] = ed.hasEulerPath();
        assert(start == 1 && end == 3);
        
        auto path = ed.solve();
        cout << "[有向图欧拉路径] 测试通过，路径: ";
        for (int v : path) cout << v << " ";
        cout << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：2-SAT
    // 3个变量x1, x2, x3
    // 约束：x1 ∨ x2, ¬x1 ∨ x3, x2 ∨ x3
    // 解：x1=true, x2=true, x3=true（或其他满足约束的解）
    // ─────────────────────────────────────────────────────
    {
        TwoSAT sat(3);
        sat.addOr(1, true, 2, true);   // x1 ∨ x2
        sat.addOr(1, false, 3, true);  // ¬x1 ∨ x3
        sat.addOr(2, true, 3, true);   // x2 ∨ x3
        
        assert(sat.solve() == true);
        auto sol = sat.getSolution();
        
        cout << "[2-SAT] 测试通过，有解\n";
        cout << "        x1=" << sol[1] << ", x2=" << sol[2] << ", x3=" << sol[3] << "\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试4：2-SAT无解
    // 约束：x1, ¬x1（x1必须同时为真和假）
    // ─────────────────────────────────────────────────────
    {
        TwoSAT sat(1);
        sat.addForce(1, true);
        sat.addForce(1, false);
        
        assert(sat.solve() == false);
        cout << "[2-SAT无解] 测试通过，无解\n";
    }
    
    cout << "\n===== 所有欧拉回路/2-SAT测试通过 =====\n";
    return 0;
}