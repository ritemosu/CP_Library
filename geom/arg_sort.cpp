#include <bits/stdc++.h>
using namespace std;
using ll = long long;


/**
 * 偏角ソートをするためのベクトル構造体です。
 * 機能として
 * - bool up(): [0,180), [180, 360) の判定
 * - cross(P b): 外積（正なら a < b）
 * - <: 判定
 * ができる。
 */
struct P {
    ll x,y,i;

    /**
     * vectorなどの初期値用
     */
    P (): x(0), y(0) {};

    /**
     * コンストラクタで、今回は座標を受け取るだけ。
     * 必要になったらidxあってもいいかも
     */
    P (ll x, ll y): x(x), y(y) {}

    /**
     * [0,180), [180, 360) の半分で区切ることで、逆時計回りに番号付することができるため、
     * 上半分なのか、下半分なのかの判定をする。
     */
    bool up() {return y > 0 || (y == 0 && x > 0);}

    /**
     * ベクトルP1とP2を考える
     * P2 = 番号2, P1 = 番号1とつけたい場合、外積の値が正の値であることが条件
     * 逆に、逆向きだった場合負の値（つまり符号ありの面積?になる）
     * そのため一直線上にあった場合は0。
     */
    ll cross(const P& a) const {return x*a.y - a.x*y;}

    /**
     * 偏角ソートをするときに必要
     * std::sortにあるやつを使うんだけど、その時のコンパレータ的な存在？
     */
    bool operator< (P& a) {
        // それぞれの上半分と下半分が違う場合は、今のベクトルが[0,180)の方向を向いていたらこっちの方が先
        if (up() != a.up()) return up();
        // そうじゃない場合は外積が正の値かどうかを見るだけ
        else return cross(a) > 0;
    }

};

/**
 * ベクトル形式入力が難しいからoperatorで楽する
 */
istream& operator>>(istream& in, P& a) {
    return in >> a.x >> a.y;
}

/**
 * 使う機会あるかわからないけど、出力も楽に
 */
ostream& operator<<(ostream& os, const P& v) {
    return os << v.x << " " << v.y;
}
