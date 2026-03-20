#include<bits/stdc++.h>
using namespace std;

// ==================== 类型别名 ====================
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vi>;
using vvl = vector<vl>;

// 128位整数（部分平台支持）
// using i128 = __int128;
// using u128 = unsigned __int128;

// ==================== 常用常量 ====================
const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fLL;
const int MOD = 1e9 + 7;
const int MOD2 = 998244353;
const ld PI = acosl(-1.0);
const ld EPS = 1e-9;

// ==================== 常用宏 ====================
#define rep(i, n) for (int i = 0; i < (n); i++)
#define rep1(i, n) for (int i = 1; i <= (n); i++)
#define per(i, n) for (int i = (n) - 1; i >= 0; i--)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) ((int)(x).size())
#define pb push_back
#define eb emplace_back
#define fi first
#define se second

// ==================== 调试输出（提交时注释掉）====================
#ifdef LOCAL
    #define debug(x) cerr << "[" << #x << " = " << (x) << "] "
    #define debugln(x) cerr << "[" << #x << " = " << (x) << "]\n"
    #define debugv(v) { cerr << "[" << #v << ": "; for(auto& _x : v) cerr << _x << " "; cerr << "]\n"; }
#else
    #define debug(x)
    #define debugln(x)
    #define debugv(v)
#endif

// ==================== 快读快写（可选，一般cin够用）====================
inline int read() {
    int x = 0, f = 1; char c = getchar();
    while (c < '0' || c > '9') { if (c == '-') f = -1; c = getchar(); }
    while (c >= '0' && c <= '9') { x = x * 10 + c - '0'; c = getchar(); }
    return x * f;
}
inline ll readll() {
    ll x = 0, f = 1; char c = getchar();
    while (c < '0' || c > '9') { if (c == '-') f = -1; c = getchar(); }
    while (c >= '0' && c <= '9') { x = x * 10 + c - '0'; c = getchar(); }
    return x * f;
}
inline void write(ll x) {
    if (x < 0) { putchar('-'); x = -x; }
    if (x > 9) write(x / 10);
    putchar(x % 10 + '0');
}
inline void writeln(ll x) { write(x); putchar('\n'); }

// ==================== 常用函数 ====================
template<typename T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
template<typename T> T lcm(T a, T b) { return a / gcd(a, b) * b; }
template<typename T> T power(T a, ll b, T mod) {
    T res = 1; a %= mod;
    for (; b > 0; b >>= 1) { if (b & 1) res = res * a % mod; a = a * a % mod; }
    return res;
}
template<typename T> bool chmax(T& a, T b) { return a < b ? a = b, true : false; }
template<typename T> bool chmin(T& a, T b) { return a > b ? a = b, true : false; }

// ==================== 主逻辑 ====================
void solve() {
    // 在这里写题目代码
    
}

// ==================== 主函数 ====================
signed main() {
    // --- 文件输入输出（本地调试用）---
    #ifdef LOCAL
        // freopen("input.txt", "r", stdin);
        // freopen("output.txt", "w", stdout);
        // freopen("error.txt", "w", stderr);
    #endif
    
    // --- 快速IO ---
    cin.tie(nullptr)->sync_with_stdio(false);
    cout << fixed << setprecision(15);  // 浮点数精度
    
    // --- 单/多组测试 ---
    int T = 1;
    // cin >> T;  // 多组测试打开这行
    while (T--) {
        solve();
    }
    
    return 0;
}