#include<bits/stdc++.h>
using namespace std;
using ll = long long;
/*
应用场景:
  • 异或最大子数组（前缀异或和+Trie）
  • 异或第k大（Trie上二分）
  
典型题:
  • "求数组中两数异或的最大值"
  • "子数组异或和的最大值"
  
核心思路: 贪心从高位选相反位
*/
struct XorTrie {
    // 01字典树节点：0/1两个分支
    struct Node { 
        Node* ch[2]{}; // 0和1分支，初始为空
    };
    Node* root = new Node();
    static const int B = 30; // int型最高位（从0开始）

    // 插入整数x（按二进制位从高到低插入）
    void insert(int x) {
        Node* p = root;
        for (int i = B; i >= 0; i--) {
            int u = (x >> i) & 1; // 获取第i位的值（0/1）
            if (!p->ch[u]) p->ch[u] = new Node();
            p = p->ch[u];
        }
    }

    // 查询与x异或的最大值
    int maxXor(int x) {
        Node* p = root;
        int res = 0;
        for (int i = B; i >= 0; i--) {
            int u = (x >> i) & 1;
            // 贪心选择相反位，最大化异或结果
            if (p->ch[!u]) {
                res |= (1 << i); // 该位异或为1，加入结果
                p = p->ch[!u];
            } else {
                p = p->ch[u]; // 无相反位，选相同位
            }
        }
        return res;
    }
};

// 独立测试
int main() {
    XorTrie xt;
    xt.insert(3);  // 二进制：0000...0011
    xt.insert(10); // 二进制：0000...1010
    xt.insert(5);  // 二进制：0000...0101
    
    // 5 ^ 10 = 15（二进制1111），为最大值
    assert(xt.maxXor(5) == 15);
    cout << "[XorTrie] 测试通过，最大异或值：" << xt.maxXor(5) << "\n";
    return 0;
}