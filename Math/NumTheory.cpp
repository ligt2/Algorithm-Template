#include<bits/stdc++.h>
using ll = long long;
using ull = unsigned long long;
using u128 = unsigned __int128;
using i128 = __int128;
using namespace std;
const int INF = 0x3f3f3f3f;

struct Euler {
    ll M = 998244353;  // 按题目改
    // 快速幂: a^b % m
    ll pw(ll base, ll exp,ll mod=-1) {
        if(mod==-1) mod=M;
    	ll res = 1;
  		  while (exp) {
        	if (exp & 1) res = (res * base)%mod ;
        	base = (base * base)%mod ;
        	exp >>= 1; 
    	}
    	return res;
	}
    // 逆元（要求M是质数）
    ll inv(ll x) { return pw(x, M - 2); }
    // φ(n): O(√n)
    ll phi(ll n) {
        ll r = n;
        for (ll i = 2; i * i <= n; i++)
            if (n % i == 0) { r -= r / i; while (n % i == 0) n /= i; }
        if (n > 1) r -= r / n;
        return r;
    }
    // φ(a^b) % M: O(√a + log b);    公式: φ(a^b) = φ(a) × a^(b-1)，无需逆元，适用任意M
    ll phi_pw(ll a, ll b) {
        if (a <= 1 || b == 0) return (a == 1) ? 1 : 0;
        return phi(a) % M * pw(a, b - 1) % M;
    }
    // 分解质因数: O(√n)
    vector<pair<ll,int>> fac(ll n) {
        vector<pair<ll,int>> r;
        for (ll i = 2; i * i <= n; i++)
            if (n % i == 0) { int c = 0; while (n % i == 0) n /= i, c++; r.push_back({i, c}); }
        if (n > 1) r.push_back({n, 1});
        return r;
    }

    // 线性筛 [1,n]: O(n)

vector<ll> ph,p; vector<bool>is_prime;
void init(int n) {
	ph.resize(n+1);is_prime.resize(n+1,true);p.reserve(n/10);
    ph[1]=1;is_prime[1]=false;
	for(int i=2;i<=n;i++) {
		if(is_prime[i]) {
			p.push_back(i);
			ph[i]=i-1;	
		}
		for(int j:p) {
			if(1LL*i*j>n) break;
			is_prime[i*j]=false;
			if(i%j==0) {
				ph[i*j]=ph[i]*j;
				break;
			}	
            else ph[i*j]=ph[i]*(j-1);
		}	
	}
} 

    // 判断质数: O(√n)
    bool isp(ll n) { if (n < 2) return 0; for (ll i = 2; i * i <= n; i++) if (n % i == 0) return 0; return 1; }
    // gcd
    ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
    // 降幂: a^b mod m（b极大时，要求gcd(a,m)=1）
    ll red(ll a, ll b, ll m) { return pw(a, b % phi(m), m); }
} e;

signed main() {
  // 1. φ(n)
    assert(e.phi(10) == 4);   // 1,3,7,9
    assert(e.phi(12) == 4);   // 1,5,7,11
    assert(e.phi(13) == 12);  // 质数p → φ=p-1
    
    // 2. φ(a^b)
    assert(e.phi_pw(2, 3) == 4);  // φ(8)=4
    assert(e.phi_pw(3, 2) == 6);  // φ(9)=6
    
    // 3. 质因数分解
    auto f = e.fac(12);  // 12=22×3
    assert(f[0] == make_pair(2LL, 2) && f[1] == make_pair(3LL, 1));
    
    // 4. 线性筛
    e.init(20);
    assert(e.ph[1] == 1 && e.ph[6] == 2 && e.ph[7] == 6);
    
    // 5. 其他
    assert(e.isp(7) && !e.isp(9));
    assert(e.gcd(12, 8) == 4);
    
    cout << "All Passed!\n";
}



