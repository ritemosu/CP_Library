#include <bits/stdc++.h>
using namespace std;

constexpr long long BASE = 202611041041;
constexpr long long MOD  = (1LL<<61)-1;

struct Rollinghash {

    vector<ll> pw, h;

    Rollinghash(const string& s) {
        int n = s.size();
        h.assign(n+1,0);
        pw.assign(n+1,1);
        for (int i=0; i<n; i++) {
            h[i+1] = ((__int128_t)h[i] * BASE + s[i])%MOD;
            pw[i+1] = (pw[i] * BASE)%MOD;
        }   
    }

    long long get(int l, int r) const {
        ll x = (__int128_t)[l] * pw[r-l]%MOD;
        return ((h[r] - x)+MOD)%MOD;
    }

};
