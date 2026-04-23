#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*=============================================================
  【数位DP】
  
  ⭐ 省赛低频率，难度高
  
  ═══════════════════════════════════════════════════════════
  【什么时候用？】
  
  ✅ 信号词：
     • "1到N中满足某条件的数的个数"
     • "数字之和"、"数位之和"
     • "不含某个数字"、"相邻数字不同"
     • N很大（如 10^18）但只关心数位性质
  
  ═══════════════════════════════════════════════════════════
  【核心思想】
  
  把数字按位拆分，从高位到低位枚举，记录：
  1. 当前是否受限（是否顶到上界）
  2. 是否已经开始（前导零处理）
  3. 其他状态（如数位和、余数等）
  
  ═══════════════════════════════════════════════════════════
  【复杂度】
  
  • 状态数：O(位数 × 状态数)
  • 通常：O(log N × 状态)
  
  ═══════════════════════════════════════════════════════════
  【典型例题】
  
  • 1到N中数字1出现的次数
  • 1到N中不含4的数的个数
  • 1到N中各数位之和为K的数的个数
  
=============================================================*/

// ═══════════════════════════════════════════════════════════
// 【模板1】数位DP框架（记忆化搜索版）
// ═══════════════════════════════════════════════════════════
struct DigitDP {
    vector<int> digits;  // 存储N的每一位（高位在前）
    map<tuple<int, int, bool, bool>, ll> memo;  // 记忆化
    
    // 【核心DFS】
    // pos: 当前处理第几位（从0开始，高位到低位）
    // state: 问题相关的状态（如数位和、余数等）
    // limit: 当前是否受上界限制
    // started: 是否已经放置了非零数字（处理前导零）
    ll dfs(int pos, int state, bool limit, bool started) {
        // 【边界】处理完所有位
        if (pos == digits.size()) {
            if (!started) return 0;  // 全是前导零，不算
            return checkState(state);  // 检查状态是否满足条件
        }
        
        // 【记忆化】
        auto key = make_tuple(pos, state, limit, started);
        if (memo.count(key)) return memo[key];
        
        // 【枚举当前位】
        int maxDigit = limit ? digits[pos] : 9;
        ll result = 0;
        
        for (int d = 0; d <= maxDigit; d++) {
            bool newStarted = started || (d > 0);
            int newState = newStarted ? updateState(state, d) : state;
            bool newLimit = limit && (d == maxDigit);
            
            result += dfs(pos + 1, newState, newLimit, newStarted);
        }
        
        return memo[key] = result;
    }
    
    // 【需要根据题目重写】更新状态
    virtual int updateState(int state, int digit) {
        return state + digit;  // 默认：数位和
    }
    
    // 【需要根据题目重写】检查最终状态
    virtual ll checkState(int state) {
        return 1;  // 默认：统计个数
    }
    
    // 【计算[1, N]中满足条件的数的个数】
    ll solve(ll N) {
        if (N <= 0) return 0;
        
        // 拆分数字
        digits.clear();
        for (ll x = N; x > 0; x /= 10) {
            digits.push_back(x % 10);
        }
        reverse(digits.begin(), digits.end());
        
        memo.clear();
        return dfs(0, 0, true, false);
    }
    
    // 【计算[L, R]中满足条件的数的个数】
    ll solve(ll L, ll R) {
        return solve(R) - solve(L - 1);
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板2】统计数位和为K的数的个数
// ═══════════════════════════════════════════════════════════
struct DigitSumK : DigitDP {
    int targetSum;
    
    DigitSumK(int k) : targetSum(k) {}
    
    int updateState(int state, int digit) override {
        return state + digit;
    }
    
    ll checkState(int state) override {
        return (state == targetSum) ? 1 : 0;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板3】统计不含数字4的数的个数
// ═══════════════════════════════════════════════════════════
struct NoDigit4 {
    vector<int> digits;
    vector<vector<ll>> dp;  // dp[pos][limit]
    
    ll dfs(int pos, bool limit) {
        if (pos == digits.size()) return 1;
        
        if (!limit && dp[pos][0] != -1) return dp[pos][0];
        
        int maxDigit = limit ? digits[pos] : 9;
        ll result = 0;
        
        for (int d = 0; d <= maxDigit; d++) {
            if (d == 4) continue;  // 跳过4
            result += dfs(pos + 1, limit && (d == maxDigit));
        }
        
        if (!limit) dp[pos][0] = result;
        return result;
    }
    
    ll solve(ll N) {
        if (N <= 0) return 0;
        
        digits.clear();
        for (ll x = N; x > 0; x /= 10) {
            digits.push_back(x % 10);
        }
        reverse(digits.begin(), digits.end());
        
        dp.assign(digits.size(), vector<ll>(2, -1));
        return dfs(0, true) - 1;  // -1 排除0
    }
    
    ll solve(ll L, ll R) {
        return solve(R) - solve(L - 1);
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板4】统计数字1出现的次数
// ═══════════════════════════════════════════════════════════
struct CountDigit1 {
    // 更高效的数学解法
    ll solve(ll N) {
        if (N <= 0) return 0;
        
        ll count = 0;
        ll factor = 1;
        
        while (factor <= N) {
            ll lower = N % factor;
            ll current = (N / factor) % 10;
            ll higher = N / (factor * 10);
            
            if (current == 0) {
                count += higher * factor;
            } else if (current == 1) {
                count += higher * factor + lower + 1;
            } else {
                count += (higher + 1) * factor;
            }
            
            factor *= 10;
        }
        
        return count;
    }
};

// ═══════════════════════════════════════════════════════════
// 【模板5】统计被K整除的数的个数（数位和或数本身）
// ═══════════════════════════════════════════════════════════
struct DivisibleByK {
    vector<int> digits;
    int K;
    vector<vector<ll>> dp;  // dp[pos][remainder]
    
    DivisibleByK(int k) : K(k) {}
    
    ll dfs(int pos, int rem, bool limit, bool started) {
        if (pos == digits.size()) {
            return (started && rem == 0) ? 1 : 0;
        }
        
        if (!limit && started && dp[pos][rem] != -1) {
            return dp[pos][rem];
        }
        
        int maxDigit = limit ? digits[pos] : 9;
        ll result = 0;
        
        for (int d = 0; d <= maxDigit; d++) {
            bool newStarted = started || (d > 0);
            int newRem = newStarted ? (rem * 10 + d) % K : 0;
            result += dfs(pos + 1, newRem, limit && (d == maxDigit), newStarted);
        }
        
        if (!limit && started) dp[pos][rem] = result;
        return result;
    }
    
    ll solve(ll N) {
        if (N <= 0) return 0;
        
        digits.clear();
        for (ll x = N; x > 0; x /= 10) {
            digits.push_back(x % 10);
        }
        reverse(digits.begin(), digits.end());
        
        dp.assign(digits.size(), vector<ll>(K, -1));
        return dfs(0, 0, true, false);
    }
    
    ll solve(ll L, ll R) {
        return solve(R) - solve(L - 1);
    }
};

// ═══════════════════════════════════════════════════════════
// 【测试用例】
// ═══════════════════════════════════════════════════════════
int main() {
    cout << "===== 数位DP模板测试 =====\n\n";
    
    // ─────────────────────────────────────────────────────
    // 测试1：不含数字4的数
    // 1到20中不含4的：1,2,3,5,6,7,8,9,10,11,12,13,15,16,17,18,19,20
    // 共18个
    // ─────────────────────────────────────────────────────
    {
        NoDigit4 solver;
        
        assert(solver.solve(20) == 18);
        cout << "[不含4] 测试通过，1到20中不含4的数有 " << solver.solve(20) << " 个\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试2：数字1出现的次数
    // 1到13：1,10,11,12,13 → 1出现6次
    // ─────────────────────────────────────────────────────
    {
        CountDigit1 solver;
        
        assert(solver.solve(13) == 6);
        cout << "[数字1次数] 测试通过，1到13中1出现 " << solver.solve(13) << " 次\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试3：被K整除的数
    // 1到20中被3整除：3,6,9,12,15,18 → 6个
    // ─────────────────────────────────────────────────────
    {
        DivisibleByK solver(3);
        
        assert(solver.solve(20) == 6);
        cout << "[被3整除] 测试通过，1到20中被3整除的数有 " << solver.solve(20) << " 个\n";
    }
    
    // ─────────────────────────────────────────────────────
    // 测试4：数位和为K
    // 1到20中数位和为5的：5,14 → 2个
    // ─────────────────────────────────────────────────────
    {
        DigitSumK solver(5);
        
        assert(solver.solve(20) == 2);
        cout << "[数位和=5] 测试通过，1到20中数位和为5的数有 " << solver.solve(20) << " 个\n";
    }
    
    cout << "\n===== 所有数位DP测试通过 =====\n";
    return 0;
}