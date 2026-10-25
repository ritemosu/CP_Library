#include <iostream>
#include <random>

struct node_t {
    int val;
    int pri;
    node_t *ch[2];
    int cnt;

    node_t(int v) : val(v), pri(rand()), cnt(1) {
        ch[0] = ch[1] = nullptr;
    }
};

// ヘルパー関数
int get_count(node_t *t) { return t ? t->cnt : 0; }

node_t *update(node_t *t) {
    if (t) t->cnt = get_count(t->ch[0]) + get_count(t->ch[1]) + 1;
    return t;
}

node_t *rotate(node_t *t, int b) {
    node_t *s = t->ch[1 - b];
    t->ch[1 - b] = s->ch[b];
    s->ch[b] = t;
    update(t);
    update(s);
    return s;
}

// 値 v を挿入する（std::set風）
node_t *insert(node_t *t, int v) {
    if (!t) return new node_t(v);
    if (t->val == v) return t; // 重複を許さない場合（set）
    
    int b = (v > t->val);
    t->ch[b] = insert(t->ch[b], v);
    update(t);
    if (t->ch[b]->pri < t->pri) t = rotate(t, 1 - b);
    return t;
}

// 使用例
int main() {
    srand(time(NULL));
    node_t *root = nullptr;

    // データの挿入
    for (int v : {10, 5, 15, 3, 8}) {
        root = insert(root, v);
    }

    std::cout << "要素数: " << get_count(root) << std::endl;
    std::cout << 
    // 3, 5, 8, 10, 15 の順で木の中に保持されます
    return 0;
}