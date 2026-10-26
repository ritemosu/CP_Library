#include <bits/stdc++.h>
using namespace std;

/**
 * エラトステネスの篩
 * 指定した数までの素数の全列挙をすることが可能
 * 
 * 機能としては
 * 
 * - 素数全列挙
 * - 素数判定
 * - 素因数分解
 * - 約数判定
 * 
 * などが可能
 */
struct Sieve {

    int n;

    vector<bool> f;
    vector<int> primes;

    // コンストラクタ。
    // ほとんどの場合M = 10^6でいいこと多いので、
    // あらかじめ下で指定してる。
    Sieve (int _n) : n(_n), f(_n+1,true) {
        f[0] = f[1] = false;
        for (long long i=2; i<=n; i++) {
            if (!f[i]) continue;
            primes.push_back(i);
            for (long long j=i*i; j<=n; j+=i) {
                f[j] = 0;
            }
        }
    }

    // 任意の数の素数判定
    bool is_prime(int x) {return f[x];}

    /**
     * 素因数分解をする
     * O(\sqrt{x})
     */
    vector<pair<int,int>> fact(int x) {
        vector<pair<int,int>> ps;
        long long y = x;
        for (long long p : primes) {
            if (p*p > x) break;
            int cnt = 0;
            while (y%p==0) y /= p, cnt++;
            if (cnt) ps.emplace_back(p,cnt);
            if (y == 1) break;
        }
        if (y != 1) ps.emplace_back(y,1);
        return ps;
    }

    /**
     * long long用
     */
    vector<pair<long long,long long>> fact(long long x) {
        vector<pair<long long,long long>> ps;
        long long y = x;
        for (long long p : primes) {
            if (p*p > x) break;
            int cnt = 0;
            while (y%p==0) y /= p, cnt++;
            if (cnt) ps.emplace_back(p,cnt);
            if (y == 1) break; 
        }
        if (y != 1) ps.emplace_back(y,1);
        return ps;
    }

    /**
     * 約数全列挙
     * 1 ~ \sqrt{N}までみていくのと、N ~ \sqrt{N}
     * で見る。よって計算量はO(\sqrt{N};
     * ただ、やるなら素数テーブルを用意するほうが早かったりする。
     */
    vector<long long> divisors_slow(long long N) {
        vector<long long> lower_nums, upper_nums;
        for (long long i=1; i*i<=N; i++) {
            if (N%i == 0) {
                lower_nums.push_back(i);
                if (i != N/i) {
                    upper_nums.push_back(N/i);
                } 
            }
        }
        while ((int)upper_nums.size()) {
            lower_nums.push_back(upper_nums.back());
            upper_nums.pop_back();
        }
        
        return lower_nums;
    }

    /**
     * 約数全列挙高速版
     * 
     * アルゴリズム
     * 約数はそもそも列挙した組み合わせで作れる
     * N = 12だとすると、
     * N = 2^2 * 3
     * 1, 2, 3, 4, 6, 12
     * これら全て2^a * 3^b の組み合わせで作成できる。
     * 小さい順から列挙する、最初配置だから1,2,4を作成
     * 次に{1,2,4} に対して、3^1 * 3^bまでかけてpush_back
     * これをすることで全組み合わせをすることが可能
     * 
     */
    vector<long long> divisors(long long N) {
        vector<long long> res = {1};
        for (auto [p,e] : fact(N)) {
            int sz = res.size();
            long long now = 1;
            for (int i=0; i<e; i++) {
                now *= p;
                for (int j=0; j<sz; j++) res.push_back(now*res[j]);
            }
        }
        sort(res.begin(), res.end());

        return res;
    }

    long long divisors_cnt(long long N) {
        long long res = 1;
        for (auto [p,e] : fact(N)) res *= (e+1);
        return res;
    }

} Sieve(1000000);