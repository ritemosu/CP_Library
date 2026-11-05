#include <cassert>
#include <functional>
#include <vector>
using namespace std;

// 全方位木DPの概要
// 1. 問題設定: N個の頂点から成る木があります。それぞれの頂点の最長パスを求めてください。
// 2. 愚直解法: 各頂点からDFS or BFSをする。O(N^2) <- 計算量が悪い (N <= 2 x 10^5くらいの想定)
// 3. 次に、愚直解法のダメ（無駄）な点を考える。
// 4. 一番の無駄は、何回も同じ頂点からの最長パスを求めていること。
// 5. そして、このアルゴリズムはこの「何回も同じものを求める」という過程を省略する。
// 6. 手順としては以下の通り
//   6.1: まずは適当な頂点からそれぞれの頂点へDFSをする。
//   6.2: この木DPでは、葉のほうから演算をして、根のほうに値を伝播させていく。
//   6.3: 次に、根のほうから葉のほうへ、DPをしていく
//   6.4: この時に、重要な概念となるのは前後の累積○○
//   6.5: 特定の部分木min_maxみたいなのを素早く取得したい
//   6.6: 工夫として、ちょうどその部分木だけを含まない演算をするために、うまく累積を取る
// 　6.7: それをすべてに対してやる
//   6.8: それぞれの葉に対して、値を設定する
//   6.9: query(i)とかで素早くゲットできるようにする。
// 7. また、この全方位木DPはmax以外もできる。条件は何か？
//   7.1: 結合則が成り立つこと: (a☆b)☆c = a☆(b☆c) <- 今回の最長のmaxはこれを満たす
//   7.2: 単位元があること    : i.e INF(1e18)とか0
// 　（おまけ）: 交換則: これが成り立てばこれを可換モノイドと呼ぶ
//   （おまけ2): また、7.1と7.2が成り立つと、それをモノイドを成すという。
// 8. つまり、セグメントツリーのように抽象化をすることができる。具体的には以下の通り
//   8.1: 値同士をまとめる（merge。例: max）  -> operate(T x, T y)
//   8.2: まとめた値に頂点を足す（例: x）        -> putVertex(T x, int v)
//   8.3: 辺を通って隣へ渡す（例: x + w）        -> putEdge(T x, W w)
// 9. 使い方: コンストラクタ -> add(u, v, w) を全ての辺について呼ぶ -> init() -> query(v)

template <class T, class W = long long> class ReRooting {

public:
    // 頂点数
    int N;

private:

    // AdjacentList[v][j] = v の j 番目の隣の頂点
    vector<vector<int>> AdjacentList;
    // Index_of_Adjacent[v][j] = j 番目の隣の頂点から見て、v が何番目にあるか。
    // これがあると、rerootするときに逆向きの辺の位置をすぐ引ける。
    vector<vector<int>> Index_of_Adjacent;
    // Weight[v][j] = v と j 番目の隣を結ぶ辺の重み
    vector<vector<W>> Weight;

    // 値同士をまとめる（例: max）
    function<T(T,T)> operate;
    // まとめた値に頂点 v を足す（例: x）
    function<T(T,int)> putVertex;
    // 重み w の辺を通って隣へ渡す（例: x + w）
    function<T(T,W)> putEdge;

    // res[v] = v を根にしたときの答え
    vector<T> res;
    // DP[v][j] = v から j 番目の隣の方向を見たときの値。その辺を含む（putEdge 済み）。
    vector<vector<T>> DP;

    // operate の単位元
    T identity;
    // init() を呼んだかどうか
    bool initialized;

public:

    /**
     * arg1: int n: 頂点数
     * arg2: operate(T a, T b) -> T: 値同士をまとめる
     * arg3: putVertex(T x, int v) -> T: まとめた値に頂点 v を足す
     * arg4: putEdge(T x, W w) -> T: 重み w の辺を通って隣へ渡す
     * arg5: T identity: operate の単位元
     */
    ReRooting(int n, function<T(T,T)> _operate, function<T(T,int)> _putVertex, function<T(T,W)> _putEdge, T _identity)
        : N(n), AdjacentList(n), Index_of_Adjacent(n), Weight(n),
          operate(_operate), putVertex(_putVertex), putEdge(_putEdge),
          identity(_identity), initialized(false) {}

    // 辺 u-v を追加する（重み省略時は 1）
    void add(int u, int v, W w = 1) {
        Index_of_Adjacent[u].push_back((int)AdjacentList[v].size());
        Index_of_Adjacent[v].push_back((int)AdjacentList[u].size());

        AdjacentList[u].push_back(v);
        AdjacentList[v].push_back(u);

        Weight[u].push_back(w);
        Weight[v].push_back(w);
    }

    // 辺を全部追加した後に呼んで DP を計算する
    void init() {

        res.assign(N, identity);
        DP.resize(N);
        for (int i=0; i<N; ++i) DP[i].assign((int)AdjacentList[i].size(), identity);

        initialized = true;

        if (N == 1) {
            res[0] = putVertex(identity, 0);
            return;
        }

        vector<int> par(N, -1);
        vector<int> st;
        vector<int> paths;
        st.push_back(0);

        while (!st.empty()) {
            int i = st.back(); st.pop_back();
            paths.push_back(i);
            for (int to : AdjacentList[i]) {
                if (par[i] == to) continue;
                par[to] = i;
                st.push_back(to);
            }
        }

        #pragma region From leaf to parents;

            for (int i=N-1; i>0; --i) {
                int node         = paths[i];
                int parent       = par[node];
                int parent_index = -1;
                T num            = identity;

                int Adjacent_size = AdjacentList[node].size();

                for (int j=0; j < Adjacent_size; ++j) {
                    int to = AdjacentList[node][j];
                    if (parent == to) {
                        parent_index = j;
                        continue;
                    }
                    num = operate(num, DP[node][j]);
                }

                DP[parent][Index_of_Adjacent[node][parent_index]] = putEdge(putVertex(num, node), Weight[node][parent_index]);
            }

        #pragma endregion

        #pragma region From parents to leaf

            for (int i=0; i<N; ++i) {

                int node = paths[i];
                int M    = AdjacentList[node].size();
                // suf[j] = DP[node][j+1] から最後までをまとめた値
                vector<T> suf(M, identity);
                for (int j=M-1; j>0; --j) {
                    suf[j-1] = operate(DP[node][j], suf[j]);
                }

                // pre = DP[node][0] から DP[node][j-1] までをまとめた値
                T pre = identity;

                for (int j=0; j<M; j++) {
                    DP[AdjacentList[node][j]][Index_of_Adjacent[node][j]] = putEdge(putVertex(operate(pre, suf[j]), node), Weight[node][j]);
                    pre = operate(pre, DP[node][j]);
                }

                res[node] = putVertex(pre, node);

            }

        #pragma endregion

    }

    // v を根にしたときの答え（init() の後に呼ぶ）
    T query(int v) {
        assert(initialized);
        return res[v];
    }

};
