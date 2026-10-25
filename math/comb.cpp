#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;

// combination mod prime
// https://youtu.be/8uowVvQ_-Mo?t=6002
// https://youtu.be/Tgd_zLfRZOQ?t=9928

// snukeさんの公式ライブラリから
// https://github.com/atcoder/live_library/blob/master/comb.cpp

// これはmod（ほとんど998244353?)の世界の逆元を整数n分まであらかじめ計算してるもの
// whileの中身の仕組みが正直わからん（フェルマーの小定理しかわからない...)
struct modinv {
    int n; vector<mint> d;
    modinv(): n(2), d({0,1}) {}
    mint operator()(int i) {
      // 一応元々はp.push_back(-d[mint::mod()%n]*(mint::mod()/n))だったんだけど、
      // 今はユークリッドの方の使い方がわからないから置き換えました。
      // フェルマーの小定理を利用して(m^p-1 ≡ 1 →　m^p-2 ≡ 1 / m)
      // -d[mint::mod()%n]*(mint::mod()/n)
      while (n <= i) d.push_back(mint(n).pow(mint::mod() - 2)), ++n;
      return d[i];
    }

    mint operator[](int i) const { return d[i];}
  } invs;

  // factorial（階乗）をあらかじめ(同じく mod998244353の世界)計算したもの
  struct modfact {
    int n; vector<mint> d;
    modfact(): n(2), d({1,1}) {}
    mint operator()(int i) {
      // 中身は至ってシンプルで、配列の一番後ろにあるやつに新たに数字をかけてるだけ。
      // 特定の数字(i)までいったら配列[i]の部分を返す。
      while (n <= i) d.push_back(d.back()*n), ++n;
      return d[i];
    }
    mint operator[](int i) const { return d[i];}
  } facts;

  // 上で求めたものの逆数バージョン(1/1!, 1/2!, 1/3! ...)
  // mod同士をそのまま割り算するには工夫が必要。
  // ここはフェルマーの小定理を利用して(m^p-1 ≡ 1 →　m^p-2 ≡ 1 / m)逆元を求めてる？
  // そしたらそれを配列の一番後ろの値に掛け算してる？
  struct modfactinv {
    int n; vector<mint> d;
    modfactinv(): n(2), d({1,1}) {}
    mint operator()(int i) {
      while (n <= i) d.push_back(d.back()*invs(n)), ++n;
      return d[i];
    }
    mint operator[](int i) const { return d[i];}
  } ifacts;

  // あとはcombination（10C2みたいなやつ）を計算する
  mint comb(int n, int k) {
    // nCk のnの部分が右側のkより小さいもしくはkが0未満であれば0を返す。
    if (n < k || k < 0) return 0;
    // そうでなければcombinationの公式、 n!/k!(n-k)! = n! * (k)^-1 * (n-k)^-1を求める。
    return facts(n)*ifacts(k)*ifacts(n-k);
  }