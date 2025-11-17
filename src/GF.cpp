#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

const int MOD = 1e9 + 7;

void reduce(int &x) {x = (x >= MOD ? x - MOD : x);}

void add(vector<int> &A, vector<int> &B, int shift = 0, int mul = 1) {
    A.resize(max(A.size(), B.size() + shift));
    for (int i = 0; i < B.size(); ++ i) reduce(A[i + shift] += (mul == 1 ? B[i] : MOD - B[i]));
}

vector<int> mul(vector<int> &A, vector<int> &B) {
    vector<int> re(A.size() + B.size() - 1);
    if(A.size() <= 32) {
        for (int i = 0; i < A.size(); ++ i)
            for (int j = 0; j < B.size(); ++ j)
                reduce(re[i + j] += 1ll * A[i] * B[j] % MOD);
        return re;
    }

    vector<int> AL, AR, BL, BR, mulL, mulR, mulminus;

    for (int i = 0; i < (A.size() >> 1); ++ i) AL.push_back(A[i]);
    for (int i = (A.size() >> 1); i < A.size(); ++ i) AR.push_back(A[i]);
    for (int i = 0; i < (B.size() >> 1); ++ i) BL.push_back(B[i]);
    for (int i = (B.size() >> 1); i < B.size(); ++ i) BR.push_back(B[i]);

    mulL = mul(AL, BL);
    mulR = mul(AR, BR);
    add(re, mulL);
    add(re, mulR, (A.size() >> 1) + (B.size() >> 1));

    add(AR, AL, 0, -1);
    add(BL, BR, 0, -1);
    mulminus = mul(AR, BL);
    add(mulminus, mulL);
    add(mulminus, mulR);
    add(re, mulminus, (A.size() >> 1));

    return re;
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
        mul1 = mul(F, F);
        mul2.push_back(1);
        add(mul2, F, 1);
        mul3 = mul(mul2, mul2);
        mul1.resize(N);
        mul3.resize(N);
        F = mul(mul1, mul3);
        F.insert(F.begin(), 0);
        F.resize(N);
        reduce(ans += F[n]);
    }

    cout << ans << "\n";
}
