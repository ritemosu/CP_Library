/***
 * 
 * 計算量(F|E|)の重めの最大フローアルゴリズムです。
 * 学習用に作ったからあまり使うことはないかも
 * 
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i, n) for (int i = 0; i < (n); ++i)

struct edge { int to, cap, rev; };

struct Flow {

    // 隣接行列の作成
    vector<vector<edge>> G;
    // BFSをする際の距離を記録する
    vector<int> level;
    // 各頂点で何個の辺をすでに調べたかを記録
    vector<int> iter;

    // 定数用のMAX;
    const long long MAX = 1001001001001;

    Flow(int n): G(n),level(n), iter(n) {}

    /**
     * メソッドの通り辺の情報を追加する
     * 逆辺を追加する時、まず順方向の場合は後にpush_backをするのでG[to].size()で表現できる
     * その逆はすでに追加しているのでG[from].size()-1で表現可能
     */
    void add_edge(int from, int to, int cap) {
        G[from].push_back((edge) {to, cap, (int)G[to].size()});
        G[to].push_back((edge) {from, 0, (int)G[from].size()-1});
    }

    /**
     * BFS
     * memsetでレベルの初期化を行い、あとは普通のBFSと一緒
     */
    void bfs(int s) {

        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while (que.size()) {
            int i = que.front(); que.pop();
            for (int j=0; j<(int)G[i].size(); j++) {

                if (level[G[i][j].to]!=-1 || G[i][j].cap <= 0) continue;
                level[G[i][j].to] = level[i] + 1;
                que.push(G[i][j].to);
            }
        }
    }

    /**
     * 実際の探索をDFSで行う
     * ここでiter[v]はすでにどこまでの辺を探索したかを参照型で記録しているため、何回も同じ辺を辿らない（これも効率化の一つ）
     * 実際にたどり着いた時点で一度探索を終了し、再び別の方でDFSを呼び出す。
     */
    int dfs (int v, int t, int f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < (int)G[v].size(); i++) {
            edge &e = G[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    /**
     * s = 始点、 T = 終点
     * 最初にBFSをして現在の最短距離（要領がある）を記録。
     * その後にその距離下に置いて最大に量を流せるように探索を続ける
     * 続けられなくなったら終了し、またBFSをしても終点に辿り着けなかった時点で最大値が確定するためそれを返す
     */
    int max_flow(int s, int t) {
        long long flow = 0;
        while(1) {
            bfs(s);
            if (level[t] < 0) return flow;
            fill(iter.begin(), iter.end(), 0);
            int f;
            while ((f = dfs(s,t,MAX)) > 0) {
                flow += f;
            }
        }
    }

};

int main(){

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    

    return 0;

}
