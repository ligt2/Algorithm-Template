#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  博弈论 (Game Theory) 
  
  什么时候用？
  
  ✅ 明确信号词: "两人博弈"、"先手/后手"、"轮流取"、
                 "最优策略"、"谁赢"、"必胜/必败"
  ✅ 题目特征:
     • 两人轮流操作，不能操作者输（或取最后一个的输）
     • 完全信息、无随机因素
     • 两人都采用最优策略

  ========== 核心框架 ==========
  
  公平组合游戏 (ICG):
  • 合法操作集合只取决于局面，与哪个玩家无关
  • 终态（无法操作）判负 → Normal Play Convention
  • 终态（无法操作）判胜 → Misère Play Convention
  
  P-position (Previous): 上一个操作的人赢 = 当前玩家必败
  N-position (Next):     下一个操作的人赢 = 当前玩家必胜
  
  判定法则:
  • 所有后继都是 N-position → 当前是 P-position (必败)
  • 存在某个后继是 P-position → 当前是 N-position (必胜)
  
  ========== Sprague-Grundy 定理 (万能武器) ==========
  
  SG(x) = mex{ SG(y) | x可以一步到达y }
  mex(S) = 最小的不在S中的非负整数
  
  • SG(x) = 0  ⟺  必败态 (P-position)
  • SG(x) ≠ 0  ⟺  必胜态 (N-position)
  
  ★★★ 多个独立子游戏的组合:
  SG(G₁ + G₂ + ⋯ + Gₙ) = SG(G₁) ⊕ SG(G₂) ⊕ ⋯ ⊕ SG(Gₙ)
  
  → 这就是博弈论和XOR密不可分的原因！
  
  ========== 做题决策树 (比赛时按此流程判断) ==========
  
  1. 识别出这是博弈题
     ↓
  2. 能否拆成多个独立子游戏？
     ├─ 是 → 分别求每个子游戏的SG值，XOR合并
     └─ 否 → 当作单个游戏处理
     ↓
  3. 是否匹配经典模型？(见下方速查表)
     ├─ 是 → 套公式，O(1)判断
     └─ 否 → 打表找SG值规律 / 记忆化搜索求SG
     ↓
  4. SG值有周期性？
     ├─ 是 → 找到周期，O(1)回答
     └─ 否 → 直接计算

  ========== 经典模型速查表 ==========
  
  模型         | 规则                          | 先手必败条件
  -------------|-------------------------------|---------------------
  Nim          | n堆,每次1堆取≥1               | XOR全部 = 0
  Bash         | 1堆n,每次取1~m                 | n%(m+1) = 0
  Wythoff      | 2堆,单堆任取 或 双堆取同数      | a=⌊k·φ⌋, b=a+k
  Fibonacci    | 1堆,每次≤2×上次(首次<n)         | n是Fibonacci数
  Nim-k(Moore) | n堆,每次≤k堆各取≥1             | 每位bit和%(k+1)=0
  Anti-Nim     | Nim但取最后的输                 | 见下方
  阶梯博弈      | 石子在阶梯上往下移              | 奇数级XOR = 0
  
=============================================================*/


// ==================== 1. Nim 游戏 ====================
/*=============================================================
  ✅ 什么时候用:
     • "n堆石子，每次从一堆中取至少1个，取完者胜"
  
  定理: 先手必胜 ⟺ a₁ ⊕ a₂ ⊕ ⋯ ⊕ aₙ ≠ 0
  
  直觉: XOR=0时，无论先手怎么操作都会破坏平衡;
        XOR≠0时，先手总能找到一种操作使XOR变为0
=============================================================*/
bool nimWin(const vector<int>& piles) {
    int xorSum = 0;
    for (int x : piles) xorSum ^= x;
    return xorSum != 0;
}


// ==================== 2. Bash 游戏 ====================
/*=============================================================
  ✅ 什么时候用:
     • "1堆n个石子，每次取1~m个，取完者胜"
  
  定理: 先手必败 ⟺ n % (m+1) == 0
  SG(n) = n % (m+1)
  
  直觉: 先手取k个 → 后手取(m+1-k)个 → 每轮恰好减少m+1
=============================================================*/
bool bashWin(ll n, ll m) {
    return n % (m + 1) != 0;
}
// Bash的SG值（多个Bash子游戏需要XOR时使用）
int bashSG(ll n, ll m) {
    return n % (m + 1);
}


// ==================== 3. Wythoff 博弈 ====================
/*=============================================================
  ✅ 什么时候用:
     • "两堆石子，每次可以:
        (1) 从一堆取任意个(≥1)，或
        (2) 从两堆取相同个数(≥1)"
  
  定理: 先手必败 ⟺ (a, b) 是冷局
  
  冷局公式 (a ≤ b):
    k = b - a
    a = ⌊k · φ⌋，其中 φ = (1+√5)/2 ≈ 1.618033988...
  
  前几个冷局: (0,0) (1,2) (3,5) (4,7) (6,10) (8,13) ...
  
  ⚠️ 浮点精度: 大数据时需要检查 golden ± 1 的范围
=============================================================*/
bool wythoffLose(ll a, ll b) {
    if (a > b) swap(a, b);
    ll k = b - a;
    // φ = (1+√5)/2
    double phi = (1.0 + sqrt(5.0)) / 2.0;
    ll expected_a = (ll)(k * phi);
    // 精度修正
    for (ll g = expected_a - 2; g <= expected_a + 2; g++)
        if (g >= 0 && g == a && g + k == b) return true;
    return false;
}
bool wythoffWin(ll a, ll b) { return !wythoffLose(a, b); }


// ==================== 4. SG函数计算 (万能模板) ====================
/*=============================================================
  ✅ 什么时候用:
     • 经典模型都不匹配
     • 题目给了自定义取法规则
     • 多个独立子游戏需要合并判断
  
  步骤:
  1. 对单个子游戏 → 打表/记忆化搜索求SG值
  2. 多个子游戏 → XOR所有SG值
  3. XOR = 0 必败，≠ 0 必胜
  
  复杂度: O(maxn × |moves|)
  
  ⚠️ 实战技巧: 先打表观察SG值是否有周期
     很多题的SG值存在小周期 → 直接 SG(n) = SG(n % period)
=============================================================*/

// 方法A: 给定可取集合 moves = {1, 3, 4} 表示每次可取1/3/4个
vector<int> computeSG(int maxn, const vector<int>& moves) {
    vector<int> sg(maxn + 1, 0);
    for (int i = 1; i <= maxn; i++) {
        set<int> reachable;
        for (int m : moves)
            if (i >= m)
                reachable.insert(sg[i - m]);
        // 求 mex
        int mex = 0;
        while (reachable.count(mex)) mex++;
        sg[i] = mex;
    }
    return sg;
}

// 方法B: 后继状态由自定义函数生成（更通用）
// getSuccessors(state) 返回所有后继状态
// 记忆化搜索版本
map<int, int> sg_memo;
int sg_dfs(int state, function<vector<int>(int)> getSuccessors) {
    if (sg_memo.count(state)) return sg_memo[state];
    set<int> reachable;
    for (int nxt : getSuccessors(state))
        reachable.insert(sg_dfs(nxt, getSuccessors));
    int mex = 0;
    while (reachable.count(mex)) mex++;
    return sg_memo[state] = mex;
}

// 方法C: SG值打表找周期（实战最常用！）
// 步骤: 1. 先暴力算出前200~500个SG值
//        2. 肉眼/程序找周期
//        3. 用周期O(1)回答
int findPeriod(const vector<int>& sg, int startCheck = 1) {
    int n = sg.size();
    for (int p = 1; p <= n / 3; p++) {
        bool ok = true;
        for (int i = startCheck; i + p < n && ok; i++)
            if (sg[i] != sg[i + p]) ok = false;
        if (ok) return p;
    }
    return -1; // 未找到周期
}


// ==================== 5. Anti-Nim (Misère Nim) ====================
/*=============================================================
  ✅ 什么时候用:
     • "Nim规则，但取走最后一个石子的人输"
  
  定理:
  • 若所有堆 ≤ 1: 先手胜 ⟺ 堆数为偶数 (XOR = 0)
  • 若存在堆 > 1:  先手胜 ⟺ XOR ≠ 0
  
  合并: 先手胜 ⟺ (XOR≠0 且 max>1) 或 (XOR=0 且 max≤1)
  
  直觉: 与普通Nim相比，只有在"所有堆都≤1"这个边界条件上
        结论相反，其他情况完全一样
=============================================================*/
bool antiNimWin(const vector<int>& piles) {
    int xorSum = 0;
    bool allLeq1 = true;
    for (int x : piles) {
        xorSum ^= x;
        if (x > 1) allLeq1 = false;
    }
    if (allLeq1) return (xorSum == 0);  // 偶数个1 → 先手胜
    return (xorSum != 0);               // 与普通Nim相同
}


// ==================== 6. Nim-k (Moore's Nim) ====================
/*=============================================================
  ✅ 什么时候用:
     • "n堆石子，每次最多从k堆中各取至少1个"
     • k=1 退化为普通Nim
  
  定理: 先手必败 ⟺ 对每一个二进制位，
        所有堆在该位上1的个数之和 ≡ 0 (mod k+1)
  
  例: k=2, piles={7,5,3} → 111,101,011
      bit2: 1+1+0=2, 2%3=2≠0 → 先手必胜
=============================================================*/
bool mooreNimWin(const vector<int>& piles, int k) {
    for (int bit = 0; bit < 30; bit++) {
        int sum = 0;
        for (int x : piles)
            sum += (x >> bit) & 1;
        if (sum % (k + 1) != 0) return true;
    }
    return false;
}


// ==================== 7. Fibonacci博弈 ====================
/*=============================================================
  ✅ 什么时候用:
     • "1堆n个石子，先手取1~n-1个，
        之后每次取的个数不超过上一次的2倍，取完者胜"
  
  定理: 先手必败 ⟺ n 是 Fibonacci 数
  
  背景: Zeckendorf定理 — 任何正整数可唯一分解为
        不相邻的Fibonacci数之和
  ⚠️ n=1时先手无法操作(取0~0个)，必败，1是Fibonacci数 ✓
=============================================================*/
bool isFibonacci(ll n) {
    if (n <= 0) return false;
    ll a = 1, b = 1;
    while (b < n) { ll c = a + b; a = b; b = c; }
    return b == n;
}
bool fibGameWin(ll n) {
    return !isFibonacci(n);
}


// ==================== 8. 阶梯博弈 (Staircase Nim) ====================
/*=============================================================
  ✅ 什么时候用:
     • "石子在阶梯 0,1,2,...,n 上，每次选某级台阶，
        将若干石子移到低一级台阶，无法操作者输"
     • 很多看似复杂的博弈可以转化为阶梯博弈！
  
  定理: 先手必胜 ⟺ 奇数级台阶上的石子XOR ≠ 0
  
  直觉: 偶数级台阶上的石子对胜负无影响
        （对手移偶数级到奇数级 → 你立刻移回偶数级）
        
  应用转化:
  • 某些"移动类"博弈 → 把每个空隙看作阶梯级
  • "翻硬币"博弈的变种
=============================================================*/
bool staircaseNimWin(const vector<int>& stairs) {
    int xorSum = 0;
    for (int i = 1; i < (int)stairs.size(); i += 2) // 只看奇数级
        xorSum ^= stairs[i];
    return xorSum != 0;
}


// ==================== 9. 多游戏合并 (SG定理实战) ====================
/*=============================================================
  ✅ 什么时候用:
     • 题目描述可以拆成若干个独立的子游戏
     • 每个子游戏可能是不同模型
  
  步骤:
  1. 识别出所有独立子游戏 G₁, G₂, ..., Gₙ
  2. 分别计算 SG(G₁), SG(G₂), ..., SG(Gₙ)
  3. 总SG = SG(G₁) ⊕ SG(G₂) ⊕ ⋯ ⊕ SG(Gₙ)
  4. 总SG = 0 → 先手必败; 总SG ≠ 0 → 先手必胜
  
  例: 3堆石子，每堆只能取{1,3,4}个
  → 3个独立的Bash变种子游戏
  → 分别算SG值 → XOR合并
=============================================================*/
// 示例: n个独立子游戏，每个子游戏有若干石子，可取moves中的个数
bool multiGameWin(const vector<int>& piles, const vector<int>& moves) {
    auto sg = computeSG(*max_element(piles.begin(), piles.end()), moves);
    int xorSum = 0;
    for (int p : piles)
        xorSum ^= sg[p];
    return xorSum != 0;
}


// ==================== 10. 绿色hackenbush / 图上博弈 (补充) ====================
/*=============================================================
  ✅ 什么时候用:
     • "图上删边博弈"
     
  树上Green Hackenbush:
  • 每条边的SG值 = 该边到根路径上的深度奇偶性
  • 等价于: 每个叶节点到根的距离的 XOR
  
  一般图: 需要用到 Colon Principle 等更复杂理论
  → ICPC省赛级别一般不会考到一般图版本
=============================================================*/


// ==================== 测试 ====================
int main() {
    // --- Nim ---
    assert(nimWin({1, 2, 3}) == false);   // 1^2^3 = 0 必败
    assert(nimWin({1, 2, 4}) == true);    // 1^2^4 = 7 必胜
    assert(nimWin({0}) == false);          // 空堆必败
    cout << "[Nim] 测试通过\n";

    // --- Bash ---
    assert(bashWin(10, 3) == true);    // 10%4 = 2 ≠ 0
    assert(bashWin(8, 3) == false);    // 8%4 = 0
    assert(bashWin(1, 5) == true);     // 取1个即可
    cout << "[Bash] 测试通过\n";

    // --- Wythoff ---
    // 冷局序列: (0,0) (1,2) (3,5) (4,7) (6,10)
    assert(wythoffLose(0, 0) == true);
    assert(wythoffLose(1, 2) == true);
    assert(wythoffLose(3, 5) == true);
    assert(wythoffLose(4, 7) == true);
    assert(wythoffLose(1, 3) == false);  // 不是冷局 → 先手胜
    assert(wythoffLose(2, 2) == false);  // 先手两堆各取2个 → 胜
    cout << "[Wythoff] 测试通过\n";

    // --- SG函数 ---
    // 可取 {1, 3, 4}
    auto sg = computeSG(12, {1, 3, 4});
    // SG值: 0 1 0 1 2 3 2 0 1 0 1 2 3
    assert(sg[0] == 0);  // 无法操作 → 必败
    assert(sg[1] == 1);  // 取1个到0 → 必胜
    assert(sg[2] == 0);  // 只能到sg[1]=1 → mex{1}=0 必败
    assert(sg[4] == 2);  // 可到sg[3]=1,sg[1]=1,sg[0]=0 → mex{0,1}=2
    assert(sg[7] == 0);  // 必败！
    cout << "[SG函数] 测试通过\n";

    // SG周期性检测
    auto sg_long = computeSG(200, {1, 3, 4});
    int period = findPeriod(sg_long, 30);
    cout << "  {1,3,4}的SG周期 = " << period << "\n";
    // 应该输出 period = 7

    // --- 多游戏合并 ---
    // 3个子游戏，石子数分别为 2, 7, 4，每次可取{1,3,4}
    // sg[2]=0, sg[7]=0, sg[4]=2 → XOR = 0^0^2 = 2 ≠ 0 先手胜
    assert(multiGameWin({2, 7, 4}, {1, 3, 4}) == true);
    // sg[2]=0, sg[7]=0 → XOR = 0 先手败
    assert(multiGameWin({2, 7}, {1, 3, 4}) == false);
    cout << "[多游戏合并] 测试通过\n";

    // --- Anti-Nim ---
    assert(antiNimWin({1, 1}) == true);      // 全≤1, XOR=0 → 偶数堆 胜
    assert(antiNimWin({1, 1, 1}) == false);  // 全≤1, XOR=1 → 奇数堆 败
    assert(antiNimWin({2, 3}) == true);      // 有>1, XOR=1≠0 → 胜
    assert(antiNimWin({1, 2, 3}) == false);  // 有>1, XOR=0 → 败
    cout << "[Anti-Nim] 测试通过\n";

    // --- Nim-k (Moore) ---
    // k=2: 每次最多从2堆各取≥1
    // {1,1,1}: bit0: 3个1, 3%3=0 → 先手败
    assert(mooreNimWin({1, 1, 1}, 2) == false);
    // {1,2,3}: bit0: 1+0+1=2, 2%3≠0 → 先手胜
    assert(mooreNimWin({1, 2, 3}, 2) == true);
    cout << "[Nim-k(Moore)] 测试通过\n";

    // --- Fibonacci博弈 ---
    assert(fibGameWin(1) == false);   // Fib → 败
    assert(fibGameWin(2) == false);   // Fib → 败
    assert(fibGameWin(3) == false);   // Fib → 败
    assert(fibGameWin(4) == true);    // 非Fib → 胜
    assert(fibGameWin(5) == false);   // Fib → 败
    assert(fibGameWin(7) == true);    // 非Fib → 胜
    assert(fibGameWin(8) == false);   // Fib → 败
    cout << "[Fibonacci博弈] 测试通过\n";

    // --- 阶梯博弈 ---
    // stairs = {3, 5, 2, 4}  (级别0,1,2,3)
    // 奇数级: stairs[1]=5, stairs[3]=4 → 5^4=1 ≠ 0 先手胜
    assert(staircaseNimWin({3, 5, 2, 4}) == true);
    // stairs = {3, 5, 2, 5} → 奇数级: 5^5=0 先手败
    assert(staircaseNimWin({3, 5, 2, 5}) == false);
    cout << "[阶梯博弈] 测试通过\n";

    cout << "\n=== 所有博弈论测试通过 ===\n";
    return 0;
}