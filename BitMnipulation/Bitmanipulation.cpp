#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  位运算 (Bit Manipulation)
  什么时候用？
  ✅ 明确信号词: "异或/XOR"、"子集"、"二进制"、"状态压缩"、
                 "选/不选"、"按位"
  ✅ 题目特征:
     • 选若干数使XOR和最大/最小/等于某值
     • 枚举所有子集 / 大小为k的子集
     • 用二进制表示状态（状压DP基础）
     • 涉及按位独立的计数/贡献
     
  ========== 基础位操作速查表 ==========
  
  操作              | 代码                    | 说明
  ------------------|-------------------------|------------------
  取第i位            | (x >> i) & 1            |
  设第i位为1         | x | (1LL << i)          | ⚠️ 必须1LL
  设第i位为0         | x & ~(1LL << i)         |
  翻转第i位          | x ^ (1LL << i)          |
  lowbit(最低位1)    | x & (-x)               | 树状数组核心
  去掉最低位1        | x & (x - 1)            |
  判断2的幂          | x > 0 && !(x & (x-1))  |
  n位全1掩码(int)    | (1 << n) - 1            | n ≤ 30
  n位全1掩码(ll)     | (1LL << n) - 1          | n ≤ 62
  最高位位置         | 63 - __builtin_clzll(x) | x > 0, ll版
  popcount(int)      | __builtin_popcount(x)   |
  popcount(ll)       | __builtin_popcountll(x) |
  末尾0个数(ctz)     | __builtin_ctz(x)        | = lowbit位置
  前导0个数(clz)     | __builtin_clz(x)        | int版
  
  ========== XOR 核心性质 ==========
  
  • a ^ a = 0,  a ^ 0 = a           (自反、单位元)
  • 交换律 + 结合律
  • a ^ b = c  ⟹  a = b ^ c        (可逆性！解方程利器)
  • 前缀异或: XOR(l..r) = pre[r] ^ pre[l-1]
  • 按位独立: 每一位互不影响 → 可逐位分析
  • XOR = 不进位加法
  
  ========== 常见陷阱 ⚠️ ==========
  
  1. 1 << i 当 i ≥ 31 溢出 → 必须写 1LL << i
  2. 优先级: (x >> i) & 1 括号不能省!
     x >> i & 1 实际执行的是 x >> (i & 1)  ← 大坑
  3. 负数右移是算术移位（补符号位），不是补0
  4. 枚举子集时别忘了空集的边界处理
  
=============================================================*/


// ==================== 1. 子集枚举 ====================
/*=============================================================
  场景                        | 写法                    | 复杂度
  ----------------------------|-------------------------|--------
  枚举[0, 2^n)所有集合          | for(S=0; S<(1<<n); S++) | O(2^n)
  枚举S的所有子集(含空集)       | 见下方                   | O(2^|S|)
  枚举n个元素中大小恰为k的集合  | Gosper's hack           | O(C(n,k))
  枚举所有集合的所有子集(总)     | 嵌套                    | O(3^n)
  
  ✅ 什么时候用:
     • 状压DP中需要枚举子集来转移
     • 暴力枚举所有"选/不选"方案
     • n ≤ 20 时的指数级暴力
     
  ❌ n > 20 基本不可行(2^20 ≈ 10^6, 3^20 ≈ 3.5×10^9)
=============================================================*/

// 【枚举集合S的所有子集（含空集和S本身）】
// 用法:
//   for (int sub = S; ; sub = (sub - 1) & S) {
//       // 处理 sub
//       if (sub == 0) break;  // 空集处理完后退出
//   }
//
// 原理: (sub-1)&S 的效果是在S的范围内"减1"
// 举例: S = 1011
//   sub: 1011 → 1010 → 1001 → 1000 → 0011 → 0010 → 0001 → 0000

// 【枚举大小恰为k的所有集合 — Gosper's Hack】
// 按升序枚举 popcount = k 的所有 n 位集合
// 用法:
//   for (int S = (1<<k)-1; S < (1<<n); ) {
//       // 处理 S
//       int c = S & (-S), r = S + c;
//       S = (((r ^ S) >> 2) / c) | r;
//   }

// 【枚举全集U的所有子集的所有子集 — O(3^n)】
// for (int S = 0; S < (1 << n); S++)
//     for (int sub = S; ; sub = (sub - 1) & S) {
//         // (S, sub) —— sub 是 S 的子集
//         if (sub == 0) break;
//     }
// 总复杂度 = Σ C(n,k)·2^k = 3^n（二项式定理）


// ==================== 2. 按位贡献法 ====================
/*=============================================================
  什么时候用？
  
  ✅ "求所有对的AND/OR/XOR之和"
  ✅ "求某个位运算表达式的总贡献"
  ✅ 信号: 求和 + 位运算
  
  核心思想: 每一位互相独立，逐位计算贡献再合并
  
  模式:
  for (int bit = 0; bit < 30; bit++) {   // 或 60 for ll
      int cnt1 = 0, cnt0 = 0;
      for (int x : a) {
          if ((x >> bit) & 1) cnt1++;
          else cnt0++;
      }
      // AND: 只有两个都是1才贡献 → C(cnt1, 2) 对
      // OR:  只要有一个是1就贡献 → C(n,2) - C(cnt0, 2) 对
      // XOR: 一个0一个1才贡献   → cnt0 * cnt1 对
      // 每对贡献 (1LL << bit)
  }
=============================================================*/

// 例: 求数组所有对 (i<j) 的 XOR 之和
ll sumPairXOR(const vector<int>& a) {
    int n = a.size();
    ll ans = 0;
    for (int bit = 0; bit < 30; bit++) {
        ll cnt1 = 0;
        for (int x : a)
            if ((x >> bit) & 1) cnt1++;
        ll cnt0 = n - cnt1;
        ans += cnt0 * cnt1 * (1LL << bit);
    }
    return ans;
}


// ==================== 3. 线性基 (XOR Basis) ====================
/*=============================================================
  什么时候用？
  
  ✅ 信号词: "异或最大"、"XOR最大/最小/第k小"、
            "能否XOR出某个值"、"XOR线性空间"
  ✅ 典型题:
     • "从n个数中选若干个使XOR和最大"      → queryMax
     • "判断x能否被选出的子集XOR出来"        → canRepresent
     • "所有可能XOR值中第k小"               → reduce + kthSmallest
     • "两个集合合并后的XOR最大值"           → merge
     
  复杂度: 插入 O(B), 查询 O(B),  B = log(值域) ≤ 62
  
  ✅ 核心理解:
     • 线性基 = XOR空间的一组"基向量"
     • 插入n个数后，基的大小 cnt ≤ B
     • 能表示的不同XOR值 = 2^cnt 个
     • 如果某次插入失败 → 该数可被已有基表示 → has_zero = true
     
  ❌ 反例: 需要删除元素 → 线段树分治 + 线性基
           需要区间查询 → 前缀线性基 / 离线
=============================================================*/
struct XorBasis {
    static const int B = 62;   // ll 用 62, int 用 30
    ll b[B + 1] = {};          // 基向量, b[i]的最高位是第i位
    bool has_zero = false;     // 能否XOR出0（存在线性相关）
    int cnt = 0;               // 基的大小（秩）

    // 插入x, 返回true表示线性无关（成功扩展基）
    bool insert(ll x) {
        for (int i = B; i >= 0; i--) {
            if (!((x >> i) & 1)) continue;
            if (!b[i]) { b[i] = x; cnt++; return true; }
            x ^= b[i];
        }
        has_zero = true;       // x被完全消去 → 可表示0
        return false;
    }

    // 贪心最大化: 将x与基异或使结果尽可能大
    // queryMax(0) = 能选出的最大XOR值
    ll queryMax(ll x = 0) {
        for (int i = B; i >= 0; i--)
            x = max(x, x ^ b[i]);
        return x;
    }

    // 能表示的最小非零XOR值
    ll queryMin() {
        if (has_zero) return 0;
        for (int i = 0; i <= B; i++)
            if (b[i]) return b[i];
        return 0;
    }

    // 判断x能否被表示
    bool canRepresent(ll x) {
        for (int i = B; i >= 0; i--) {
            if (!((x >> i) & 1)) continue;
            if (!b[i]) return false;
            x ^= b[i];
        }
        return true;
    }

    // 化为行最简阶梯形（kthSmallest之前必须调用）
    // 效果: b[i]的第i位以外的高位全为0
    void reduce() {
        for (int i = B; i >= 0; i--)
            for (int j = i - 1; j >= 0; j--)
                if ((b[i] >> j) & 1) b[i] ^= b[j];
    }

    // 第k小的XOR值 (1-indexed)
    // 需要先调用 reduce()
    // 总共能表示 2^cnt 个值 (has_zero时含0)
    ll kthSmallest(ll k) {
        reduce();
        if (has_zero) k--;     // 0是最小的
        if (k == 0) return 0;

        vector<ll> base;
        for (int i = 0; i <= B; i++)
            if (b[i]) base.push_back(b[i]);

        if (k >= (1LL << (int)base.size())) return -1; // 不存在

        ll res = 0;
        for (int i = 0; i < (int)base.size(); i++)
            if ((k >> i) & 1) res ^= base[i];
        return res;
    }

    // 合并另一个线性基
    void merge(const XorBasis& o) {
        for (int i = B; i >= 0; i--)
            if (o.b[i]) insert(o.b[i]);
    }
};


// ==================== 4. 常用位运算技巧集锦 ====================

// 判断 x 是否是 2 的幂
bool isPow2(ll x) { return x > 0 && (x & (x - 1)) == 0; }

// 最高位的值 (如 x=12=1100 → 返回8=1000)
ll highbit(ll x) { return x ? 1LL << (63 - __builtin_clzll(x)) : 0; }

// 下一个与x有相同popcount的更大整数 (Gosper's hack核心)
int nextSamePopcount(int x) {
    int c = x & (-x), r = x + c;
    return (((r ^ x) >> 2) / c) | r;
}

// 格雷码 (Gray Code): 相邻两数只差一位
int toGray(int n) { return n ^ (n >> 1); }
int fromGray(int g) {
    int n = 0;
    for (; g; g >>= 1) n ^= g;
    return n;
}


// ==================== 测试 ====================
int main() {
    // --- 基础位运算 ---
    int x = 0b1010;  // = 10
    assert(((x >> 1) & 1) == 1);        // 第1位是1
    assert(((x >> 0) & 1) == 0);        // 第0位是0
    assert((x & (-x)) == 2);            // lowbit = 2 (第1位)
    assert(__builtin_popcount(x) == 2); // 两个1
    assert(isPow2(8) && !isPow2(6));
    assert(highbit(12) == 8);
    cout << "[基础位运算] 测试通过\n";

    // --- 子集枚举 ---
    int S = 0b1011; // 集合 {0,1,3}
    int subCnt = 0;
    for (int sub = S; ; sub = (sub - 1) & S) {
        subCnt++;
        if (sub == 0) break;
    }
    assert(subCnt == 8); // 2^3 = 8 个子集
    cout << "[子集枚举] 测试通过\n";

    // --- 按位贡献 ---
    // {1,2,3}: 对(1,2)→3, 对(1,3)→2, 对(2,3)→1 → 和=6
    assert(sumPairXOR({1, 2, 3}) == 6);
    cout << "[按位贡献] 测试通过\n";

    // --- 线性基 ---
    XorBasis lb;
    lb.insert(1); lb.insert(2); lb.insert(4);
    assert(lb.cnt == 3);                 // 三个线性无关
    assert(lb.queryMax() == 7);          // 1^2^4 = 7
    assert(lb.canRepresent(3));          // 1^2 = 3
    assert(lb.canRepresent(7));          // 1^2^4 = 7
    assert(!lb.canRepresent(8));         // 8无法表示

    XorBasis lb2;
    lb2.insert(3); lb2.insert(5); lb2.insert(6);
    // 3=011, 5=101, 6=110 → 3^5=6, 所以6线性相关
    assert(lb2.cnt == 2);
    assert(lb2.has_zero == true);        // 3^5^6 = 0
    assert(lb2.queryMax() == 6);         // max(3,5,6,3^5=6,3^6=5,5^6=3) = 6
    cout << "[线性基] 测试通过\n";

    // --- 格雷码 ---
    assert(toGray(5) == 7);    // 5=101 → gray=111
    assert(fromGray(7) == 5);
    cout << "[格雷码] 测试通过\n";

    cout << "\n=== 所有位运算测试通过 ===\n";
    return 0;
}