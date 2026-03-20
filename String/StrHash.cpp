#include<bits/stdc++.h>
using namespace std;
using ll = long long;
/*
应用场景:
  • 判断两子串是否相等（O(1)）
  • 最长公共子串（二分+哈希）
  • 回文判断（正反哈希比较）
  • 字符串去重（map存哈希值）
  
典型题:
  • "给定n个字符串，问有多少对相同"
  • "问s[l1..r1]是否等于s[l2..r2]"
  • "最长回文子串"（配合二分）
*/
struct StrHash {
    static constexpr ll P1 = 1e9+7, P2 = 1e9+9, B = 131;
    int n;
    vector<ll> h1, h2, pw1, pw2;

    // 构造函数：传入字符串，预处理哈希值和幂次
    StrHash(string& s) : n(s.size()), h1(n+1), h2(n+1), pw1(n+1), pw2(n+1) {
        pw1[0] = pw2[0] = 1;
        for (int i = 0; i < n; i++) {
            h1[i+1] = (h1[i] * B + s[i]) % P1;
            h2[i+1] = (h2[i] * B + s[i]) % P2;
            pw1[i+1] = pw1[i] * B % P1;
            pw2[i+1] = pw2[i] * B % P2;
        }
    }

    // 获取 s[l..r] 的哈希值 (0-indexed，左闭右闭)
    pair<ll,ll> get(int l, int r) {
        ll v1 = (h1[r+1] - h1[l] * pw1[r-l+1] % P1 + P1) % P1;
        ll v2 = (h2[r+1] - h2[l] * pw2[r-l+1] % P2 + P2) % P2;
        return {v1, v2};
    }

    // 判断两个子串是否相等：s[l1..r1] == s[l2..r2]
    bool equal(int l1, int r1, int l2, int r2) {
        return get(l1, r1) == get(l2, r2);
    }
};

// 独立测试
int main() {
    string s = "abcabc";
    StrHash sh(s);
    // 测试："abc" (0-2) 和 "abc" (3-5) 相等
    assert(sh.equal(0, 2, 3, 5));
    // 测试："ab" (0-1) 和 "ca" (2-3) 不相等
    assert(!sh.equal(0, 1, 2, 3));
    cout << "[StrHash] 测试通过\n";
    return 0;
}