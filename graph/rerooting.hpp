/*
https://github.com/key-moon/Library/blob/master/src/Algorithm/rerooting.csx
keymoon による C# の実装を noshi91 が C++ に移植したものです
*/

#include <functional>
#include <stack>
#include <vector>

// ReRooting<int> graph みたいなことができる
template <class T> class ReRooting {
public:
    int NodeCount;

private:
    // 隣接グラフ
    std::vector<std::vector<int>> Adjacents;
    std::vector<std::vector<int>> IndexForAdjacent;

    std::vector<T> Res;
    std::vector<std::vector<T>> DP;

    // 単位元
    T Identity;
    // 値同士の演算、max(A,B)とかmin(A,B)とか。　
    std::function<T(T, T)> Operate;
    // ノードを1つ足す操作。(値, 頂点番号) -> 新しい値。例：最遠距離なら x + 1
    std::function<T(T, int)> OperateNode;

public:
    // コンストラクタ
    // identity = 単位元
    // operate  = 演算
    // 
    ReRooting(int nodeCount, std::vector<std::vector<int>> edges, T identity,
                std::function<T(T, T)> operate,
                std::function<T(T, int)> operateNode) {
        NodeCount = nodeCount;

        Identity = identity;
        Operate = operate;
        OperateNode = operateNode;

        Adjacents.resize(NodeCount);
        IndexForAdjacent.resize(NodeCount);

        int M = edges.size();

        for (int i = 0; i < M; i++) {
            auto &edge = edges[i];

            IndexForAdjacent[edge[0]].push_back(Adjacents[edge[1]].size());
            IndexForAdjacent[edge[1]].push_back(Adjacents[edge[0]].size());

            Adjacents[edge[0]].push_back(edge[1]);
            Adjacents[edge[1]].push_back(edge[0]);

        }

        DP = std::vector<std::vector<T>>(NodeCount);
        Res = std::vector<T>(NodeCount);

        for (int i = 0; i < NodeCount; i++) DP[i].resize(int(Adjacents[i].size()));
        if (NodeCount > 1) Initialize();
        else if (NodeCount == 1) Res[0] = OperateNode(Identity, 0);
    }

    T Query(int node) { return Res[node]; }

private:
    void Initialize() {
        std::vector<int> parents(NodeCount);
        std::vector<int> order(NodeCount);

// step1: 適当な頂点からDFSして、それぞれの演算の初期化

#pragma region InitOrderedTree
        int index = 0;
        std::stack<int> stack;
        stack.push(0);
        parents[0] = -1;
        while (!stack.empty()) {
            auto node = stack.top(); stack.pop();
            order[index++] = node;
            for (int to : Adjacents[node]) {
                if (to == parents[node]) continue;
                stack.push(to);
                parents[to] = node;
            }
        }
#pragma endregion

// step2: 帰りがけでそれぞれの頂点の部分木に対して演算をする
#pragma region fromLeaf
        for (int i = NodeCount - 1; i >= 1; i--) {
            auto node = order[i];
            auto parent = parents[node];

            T accum = Identity;
            int parentIndex = -1;
            int Adjacent_size = Adjacents[node].size();
            for (int j = 0; j < Adjacent_size; j++) {
                if (Adjacents[node][j] == parent) {
                    parentIndex = j;
                    continue;
                }
                accum = Operate(accum, DP[node][j]);
            }
            // [IndexForAdjacent[node][parentIndex]]の説明
            // 普通の隣接グラフを思い浮かべたとき、G[0] = {1,2,3,4}, G[1] = {0,6,7}
            // みたいになってるはず
            // そして、parentIndexは隣接配列のどこが親の頂点かを示してる
            // そして、そしてIndexForAdjacent[node][parentIndex]は
            // 逆の話で、親頂点のどこが自分を指しているかを表している
            // とすると、DP[parent][...省略] = parent視点の分岐先の最大
            // みたいな形になる
            // やっていることはそれ。（この場合maxを取っている）
            DP[parent][IndexForAdjacent[node][parentIndex]] = OperateNode(accum, node);

        }
#pragma endregion

// やっていることをまとめると、うまく累積に対して自分が入らないように演算している
// IndexForAdjacent[...][...]がなぜ役立つのかここでわかる　
#pragma region toLeaf
        for (int i = 0; i < NodeCount; i++) {
            auto node = order[i];
            T accum = Identity;
            std::vector<T> accumsFromTail(Adjacents[node].size());
            accumsFromTail[accumsFromTail.size() - 1] = Identity;
            // accumsFromTail[j-1] = (j-1, N] <- (こんな書き方するか知らんけど...)
            // jより大きい累積を記録している。
            // それを0までやっている感じ
            for (int j = accumsFromTail.size() - 1; j >= 1; j--)
                accumsFromTail[j - 1] = Operate(DP[node][j], accumsFromTail[j]);

            // そうしたら、次は前から累積をしていく
            // DP[Adjacents[node][j]][IndexForAdjacent[node][j]]
            // これは行先から見た自分側の部分木を記録してる
            // OperateNode(Operate(accum, accumsFromTail[j]), node);
            // これでそのDPテーブルに対する計算をしてる。
            // また、accumが前から見た累積という役割を果たしている
            // 理由は、j = ? をした後に accum = Operate(accum DP[node][?])
            // をしているから
            // これをすることできれいに特定の向きに累積できている
            for (int j = 0; j < accumsFromTail.size(); j++) {
                DP[Adjacents[node][j]][IndexForAdjacent[node][j]] = OperateNode(Operate(accum, accumsFromTail[j]), node);
                accum = Operate(accum, DP[node][j]);
            }
            // あとは最後に自分の頂点を足して答えにする（最長の時はもらったら-1）するべきだが
            Res[node] = OperateNode(accum, node);
        }
#pragma endregion
    }
};