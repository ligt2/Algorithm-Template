#include<bits/stdc++.h>
using namespace std;
const int MOD = 998244353;//必须是质数
const int MAXN = 60;   //根据上限修改 
long long fact[MAXN];   //阶乘 
long long infact[MAXN]; // 阶乘的逆元 

long long qpow(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return res;
}

void init() { // 预处理计算阶乘和阶乘逆元
    infact[0]=fact[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    infact[MAXN - 1] = qpow(fact[MAXN - 1], MOD - 2);
    for (int i = MAXN - 2; i >= 1; i--) {
        infact[i] = (infact[i + 1] * (i + 1)) % MOD;
    }
}


// 取模版本的组合数 
long long nCr(int n, int m) {     
    if (m < 0 || m > n) return 0;
    return fact[n] * infact[m] % MOD * infact[n - m] % MOD;
}

//不取模版本 
long long ncr(long long n, long long r) {
    if (r < 0 || r > n) return 0;
    if (r > n / 2) r = n - r;
    
    __int128 res = 1;
    for (long long i = 1; i <= r; ++i) {
        res = res * (n - i + 1);
        res /= i; 
    }
    
    return (long long)res;
}


int main() {
    init(); //取模版本的预处理！: 复杂度 O(MAXN+logMOD) 
    
    assert(nCr(10, 5) == 252);           
    assert(fact[10] * infact[10] % MOD == 1);  
    assert(nCr(50, 25) == (nCr(49, 24) + nCr(49, 25)) % MOD);
    cout << "Passed!" << endl;
    
    
      // ===== 极简测试案例（不取模版）=====
    cout << "\n不取模版测试：" << endl;
    cout << "C(10,5) = " << ncr(10,5) << " (预期：252)" << endl;   // 普通值
    cout << "C(20,10) = " << ncr(20,10) << " (预期：184756)" << endl;// 稍大值
    cout << "C(5,3) = " << ncr(5,3) << " (预期：10)" << endl;       // 优化r=n-r
    return 0;
}