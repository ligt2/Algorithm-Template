#include<bits/stdc++.h>
using namespace std;
using ll = long long;
/*
应用场景:
  • O(n)求最长回文子串（暴力O(n²)会TLE）
  • 统计回文子串个数
  
典型题:
  • "最长回文子串"
  • "回文子串个数"（注意：中心扩展也可以）
  
何时不用: 只判断整个串是否回文 → 双指针O(n)
*/
struct Manacher {
    string s;          // 预处理后的字符串（插入#）
    vector<int> p;     // p[i]：以i为中心的回文半径（包含自身）
    int maxLen;        // 最长回文子串长度（原字符串）
    int center;        // 最长回文子串中心（预处理后字符串）

    // 构造函数：预处理并计算回文半径
    Manacher(string& str) {
        // 插入#，统一奇偶长度回文："abc" → "#a#b#c#"
        s = "#";
        for (char c : str) { s += c; s += '#'; }
        int n = s.size();
        p.resize(n);
        int mx = 0, id = 0; // mx：当前最右回文边界，id：对应中心
        maxLen = 0; center = 0;

        for (int i = 0; i < n; i++) {
            // 初始化回文半径
            p[i] = mx > i ? min(p[2*id - i], mx - i) : 1;
            // 扩展回文半径
            while (i-p[i]>=0 && i+p[i]<n && s[i-p[i]]==s[i+p[i]]) p[i]++;
            // 更新最右边界和中心
            if (i + p[i] > mx) { mx = i + p[i]; id = i; }
            // 更新最长回文子串信息
            if (p[i] - 1 > maxLen) { maxLen = p[i] - 1; center = i; }
        }
    }

    // 获取原字符串的最长回文子串
    string longestPalindrome() {
        // 映射回原字符串的起始位置
        int start = (center - maxLen) / 2;
        // 提取并删除#字符
        string res = s.substr(start * 2 + 1, maxLen * 2 - 1);
        res.erase(remove(res.begin(), res.end(), '#'), res.end());
        return res;
    }
};

// 独立测试
int main() {
    string s = "babad";
    Manacher mc(s);
    string lp = mc.longestPalindrome();
    // 最长回文子串为"bab"或"aba"
    assert(lp == "bab" || lp == "aba");
    cout << "[Manacher] 测试通过，最长回文子串：" << lp << "\n";
    return 0;
}