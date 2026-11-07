// GitHub Link
// https://github.com/ritemosu/CP_Library/blob/main/graph/tree-diameter.hpp

#include <bits/stdc++.h>
using namespace std;

/**
 * 直径の長さと端点二つを返す
 * 
 * 返り値: {直径の辺数, 端点から端点までの頂点列}
 */
pair<int, vector<int>> tree_diameter(const vector<vector<int>>& g, int ord=0) {

    int n = g.size();

    auto bfs = [&](int s) {

        vector<int> dist(n, -1), par(n, -1);
        queue<int> q; q.push(s); dist[s] = 0;

        while (!q.empty()) {
            int v = q.front(); q.pop();

            for (int to : g[v]) if (dist[to] < 0) {
                dist[to] = dist[v] + 1; par[to] = v; q.push(to);
            }
        }

        return pair{dist, par};

    };

    auto [d0, _] = bfs(0);
    int a = n-1;
    if (ord==0) a = max_element(d0.begin(), d0.end()) - d0.begin();
    else for (int i=n-2; i>=0; --i) if (d0[i] > d0[a]) a = i;

    auto [da, par] = bfs(a);
    int b = n-1;
    if (ord==0) b = max_element(da.begin(), da.end()) - da.begin();
    else for (int i=n-2; i>=0; --i) if (da[i] > da[b]) b = i;

    vector<int> path;
    for (int v = b; v != -1; v = par[v]) path.push_back(v);
    return make_pair(da[b], path);
}

pair<long long, vector<int>> tree_diameter(const vector<vector<pair<int,long long>>>& g, int ord=0) {

    int n = g.size();

    auto bfs = [&](int s) {

        vector<long long> dist(n, -1), par(n, -1);
        queue<int> q; q.push(s); dist[s] = 0;

        while (!q.empty()) {
            int v = q.front(); q.pop();

            for (auto [to, cost] : g[v]) if (dist[to] < 0) {
                dist[to] = dist[v] + cost; par[to] = v; q.push(to);
            }
        }

        return pair{dist, par};

    };

    auto [d0, _] = bfs(0);
    int a = n-1;
    if (ord==0) a = max_element(d0.begin(), d0.end()) - d0.begin();
    else for (int i=n-2; i>=0; --i) if (d0[i] > d0[a]) a = i;

    auto [da, par] = bfs(a);
    int b = n-1;
    if (ord==0) b = max_element(da.begin(), da.end()) - da.begin();
    else for (int i=n-2; i>=0; --i) if (da[i] > da[b]) b = i;

    vector<int> path;
    for (int v = b; v != -1; v = par[v]) path.push_back(v);
    return make_pair(da[b], path);
}