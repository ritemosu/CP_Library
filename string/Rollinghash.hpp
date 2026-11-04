#include <bits/stdc++.h>
using namespace std;

struct Rollinghash {

    const long long BASE = 1000000007;

    vector<long long> B, h;

    __int128_t

    Rollinghash(const string& s) {
        int n = s.size();
        h.assign(n+1,1);
        B.assign(n+1,0);
        for (int i=0; i<n; i++) {
            h[i+1] = h[i] * 
        }
    }

}