#include<bits/stdc++.h>
using namespace std;
using ll = long long;
/*
应用场景:
  • 前缀查询（"以xxx开头的字符串有几个"）
  • 自动补全/拼写检查
  • 字符串集合去重+计数
  
典型题:
  • "给n个单词，问有多少个以'abc'开头"
  • "字典里有多少个单词"
  • "最长公共前缀"
*/
struct Trie {
    // 字典树节点：26个小写字母分支 + 计数
    struct Node { 
        Node* ch[26]{}; // 初始化为空指针
        int cnt = 0;    // 以该节点为结尾的字符串数量
    };
    Node* root = new Node(); // 根节点

    // 插入字符串（仅支持小写字母）
    void insert(string& s) {
        Node* p = root;
        for (char c : s) {
            int u = c - 'a'; // 映射为0-25
            if (!p->ch[u]) p->ch[u] = new Node(); // 不存在则新建节点
            p = p->ch[u];
        }
        p->cnt++; // 字符串结尾，计数+1
    }

    // 统计字符串出现次数
    int count(string& s) {
        Node* p = root;
        for (char c : s) {
            int u = c - 'a';
            if (!p->ch[u]) return 0; // 字符串不存在
            p = p->ch[u];
        }
        return p->cnt;
    }

    // 判断前缀是否存在
    bool startsWith(string& prefix) {
        Node* p = root;
        for (char c : prefix) {
            int u = c - 'a';
            if (!p->ch[u]) return false;
            p = p->ch[u];
        }
        return true;
    }
};

// 独立测试
int main() {
    Trie trie;
    string w1 = "apple", w2 = "app", w3 = "banana";
    trie.insert(w1); trie.insert(w2); trie.insert(w3);
    
    // 测试计数：app出现1次
    assert(trie.count(w2) == 1);
    // 测试前缀：app是apple的前缀
    string w4 = "appl";
    assert(trie.startsWith(w4));
    // 测试不存在的字符串：appl计数为0
    assert(trie.count(w4) == 0);
    
    cout << "[Trie] 测试通过\n";
    return 0;
}