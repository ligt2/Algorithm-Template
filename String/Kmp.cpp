#include<bits/stdc++.h>
using namespace std;
using ll = long long;
/*
应用场景:
  • 模式匹配（找所有出现位置）
  • 最小循环节（len - nxt[len-1]）
  • 最长公共前后缀（nxt数组）
  
典型题:
  • "模式串p在文本串s中出现几次"
  • "字符串的最小周期"
  • "给字符串前加几个字符使其循环"
*/
struct KMP {
    string pat;  // 模式串
    vector<int> nxt; // next数组：nxt[i]表示pat[0..i]的最长相等前后缀长度

    // 构造函数：预处理模式串的next数组
    KMP(string& p) : pat(p), nxt(p.size()) {
        int m = pat.size(), j = 0;
        for (int i = 1; i < m; i++) {
            // 回退到上一个匹配的位置
            while (j && pat[i] != pat[j]) j = nxt[j-1];
            if (pat[i] == pat[j]) j++;
            nxt[i] = j;
        }
    }

    // 匹配主串s，返回所有模式串起始位置（0-indexed）
    vector<int> match(string& s) {
        vector<int> res;
        int n = s.size(), m = pat.size(), j = 0;
        for (int i = 0; i < n; i++) {
            while (j && s[i] != pat[j]) j = nxt[j-1];
            if (s[i] == pat[j]) j++;
            // 匹配完成，记录位置并回退j（继续找下一个匹配）
            if (j == m) { 
                res.push_back(i - m + 1); 
                j = nxt[j-1]; 
            }
        }
        return res;
    }

    // 计算模式串的最小周期长度
    int minPeriod() {
        int m = pat.size();
        return m - nxt[m-1];
    }
};

// 独立测试
int main() {
    string s = "ababcababa", p = "aba";
    KMP kmp(p);
    // 测试匹配位置：0,2,7
    auto pos = kmp.match(s);
    assert(pos == (vector<int>{0, 2, 7}));
    // 测试最小周期："aba"的最小周期是3
    assert(kmp.minPeriod() == 3);
    cout << "[KMP] 测试通过，匹配次数：" << pos.size() << "\n";
    return 0;
}