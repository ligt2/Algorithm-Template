// =====================================================================
// 对拍模板 — 用于验证算法正确性
// =====================================================================
// 文件结构:
//   sol.cpp    - 你的解法
//   brute.cpp  - 暴力解法
//   gen.cpp    - 数据生成器
//   checker.cpp - 本文件（对拍脚本）

#include<bits/stdc++.h>
using namespace std;
using ll = long long;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// 生成 [l, r] 随机整数
int randint(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

// 生成随机数据
void generate(ofstream& out) {
    // 示例：生成 n 个随机数
    int n = randint(1, 10);
    out << n << "\n";
    for (int i = 0; i < n; i++) {
        out << randint(1, 100) << " \n"[i == n-1];
    }
}

// 暴力解法
void brute(ifstream& in, ofstream& out) {
    // 读入
    int n; in >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) in >> a[i];
    
    // 暴力计算
    ll ans = 0;
    for (int i = 0; i < n; i++) ans += a[i];
    
    // 输出
    out << ans << "\n";
}

// 正解（待验证）
void solution(ifstream& in, ofstream& out) {
    // 读入
    int n; in >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) in >> a[i];
    
    // 你的算法
    ll ans = accumulate(a.begin(), a.end(), 0LL);
    
    // 输出
    out << ans << "\n";
}

int main() {
    for (int t = 1; ; t++) {
        // 1. 生成数据
        ofstream fout("input.txt");
        generate(fout);
        fout.close();
        
        // 2. 运行暴力解
        ifstream fin1("input.txt");
        ofstream out1("brute_out.txt");
        brute(fin1, out1);
        fin1.close(); out1.close();
        
        // 3. 运行正解
        ifstream fin2("input.txt");
        ofstream out2("sol_out.txt");
        solution(fin2, out2);
        fin2.close(); out2.close();
        
        // 4. 比较结果
        ifstream f1("brute_out.txt"), f2("sol_out.txt");
        string s1((istreambuf_iterator<char>(f1)), istreambuf_iterator<char>());
        string s2((istreambuf_iterator<char>(f2)), istreambuf_iterator<char>());
        
        if (s1 != s2) {
            cout << "WA on test " << t << "!\n";
            cout << "=== Input ===\n";
            ifstream fin("input.txt");
            cout << fin.rdbuf();
            cout << "\n=== Expected ===\n" << s1;
            cout << "=== Got ===\n" << s2;
            break;
        }
        
        cout << "Test " << t << " passed\n";
        if (t >= 1000) { cout << "All 1000 tests passed!\n"; break; }
    }
    return 0;
}