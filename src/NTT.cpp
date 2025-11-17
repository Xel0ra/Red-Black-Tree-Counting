#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
using ll = __int128;

const int MOD = 1e9 + 7;

const int MOD1 = 998244353;
const int MOD2 = 1004535809;
const int MOD3 = 1012924417;
const int G1 = 3;
const int G2 = 3;
const int G3 = 5;

ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll g = exgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return g;
}

ll modinv(ll a, ll m) {
    ll x, y;
    ll g = exgcd(a, m, x, y);
    return (x % m + m) % m;
}

void bit_rev(vector<int> &a, int n) {
    vector<int> rev(n);
    for (int i = 0; i < n; ++ i) {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) rev[i] |= n >> 1;
    }
    for (int i = 0; i < n; ++ i) if (i < rev[i]) swap(a[i], a[rev[i]]);
}

ll qpow(ll x, ll y, ll mod) {
    ll re = 1;
    for (; y; y >>= 1, x = x * x % mod) if(y & 1) re = re * x % mod;
    return re;
}

void ntt(vector<int> &a, int mod, int g, int inv) {
    int n = a.size();
    bit_rev(a, n);
    
    for (int len = 2; len <= n; len <<= 1) {
        int exp = (mod - 1) / len;
        int wlen = qpow(g, exp, mod);
        if (inv) wlen = modinv(wlen, mod);
        
        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len / 2; ++ j) {
                int u = a[i + j];
                int v = (ll)a[i + j + len / 2] * w % mod;
                a[i + j] = (u + v) % mod;
                a[i + j + len / 2] = (u - v + mod) % mod;
                w = (ll)w * wlen % mod;
            }
        }
    }
    
    if (inv) {
        ll inv_n = modinv(n, mod);
        for (int &x : a) x = (ll)x * inv_n % mod;
    }
}

vector<int> poly_mult_mod(const vector<int> &a, const vector<int> &b, int mod, int g) {
    vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    int len = a.size() + b.size() - 1;
    
    while (n < len) n <<= 1;
    fa.resize(n, 0);
    fb.resize(n, 0);
    
    ntt(fa, mod, g, 0);
    ntt(fb, mod, g, 0);
    
    vector<int> fc(n);
    for (int i = 0; i < n; ++i) fc[i] = (ll)fa[i] * fb[i] % mod;
    
    ntt(fc, mod, g, 1);
    
    fc.resize(len);
    return fc;
}

pair<ll, ll> crt2(ll a1, ll m1, ll a2, ll m2) {
    ll x, y;
    ll g = exgcd(m1, m2, x, y);
    
    ll lcm = (ll)m1 / g * m2;
    x = (x * ((a2 - a1) / g)) % (m2 / g);
    x = (x + (m2 / g)) % (m2 / g);
    ll res = (a1 + x * m1) % lcm;
    return {res, lcm};
}

ll crt3(ll a1, ll a2, ll a3) {
    auto [x1, m1] = crt2(a1, MOD1, a2, MOD2);
    auto [x2, m2] = crt2(x1, m1, a3, MOD3);
    return x2 % MOD;
}

vector<int> poly_mult(const vector<int> &a, const vector<int> &b) {
    vector<int> c1 = poly_mult_mod(a, b, MOD1, G1);
    vector<int> c2 = poly_mult_mod(a, b, MOD2, G2);
    vector<int> c3 = poly_mult_mod(a, b, MOD3, G3);
    
    int len = c1.size();
    vector<int> res(len);
    for (int i = 0; i < len; ++ i) res[i] = crt3(c1[i], c2[i], c3[i]);
    return res;
}

void reduce(int &x) {x = (x >= MOD ? x - MOD : x);}

void add(vector<int> &A, vector<int> &B, int shift = 0, int mul = 1) {
    A.resize(max(A.size(), B.size() + shift));
    for (int i = 0; i < B.size(); ++ i) reduce(A[i + shift] += (mul == 1 ? B[i] : MOD - B[i]));
}

int main() {
    int n;
    cin >> n;
    int logn = log2(n) + 2, ans = 0;

    int N = 1;
    while(N < n) N <<= 1;
    vector<int> F(N, 0);
    F[0] = 1;

    for (int i = 0; i < logn; ++ i) {
        vector<int> mul1, mul2, mul3;
        mul1 = poly_mult(F, F);
        mul2.push_back(1);
        add(mul2, F, 1);
        mul3 = poly_mult(mul2, mul2);
        mul1.resize(N);
        mul3.resize(N);
        F = poly_mult(mul1, mul3);
        F.insert(F.begin(), 0);
        F.resize(N);
        reduce(ans += F[n]);
    }

    cout << ans << "\n";
}