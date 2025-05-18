#include "rotate.h"
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

#define pb push_back
#define ft first
#define sd second
#define pi pair<int, int>

template<typename T> bool mins(T& x, T y) { if (x > y) { x = y; return true; } return false; }

struct BIT {
    vector<ll> tre;
    int sz;
    BIT(int n) : sz(n), tre(n + 5, 0) {}
    void upd(int i, ll k) {
        i++;
        while (i <= sz) {
            tre[i] += k;
            i += i & -i;
        }
    }
    ll get(int i) {
        i++;
        ll res = 0;
        while (i > 0) {
            res += tre[i];
            i -= i & -i;
        }
        return res;
    }
    ll get(int l, int r) {
        if (l > r) return 0;
        return get(r) - get(l - 1);
    }
};

struct DSU {
    vector<int> e;
    DSU (int n) : e(n + 5, -1) {}
    int get(int x) { return e[x] <= -1 ? x : e[x] = get(e[x]); }
    int sz(int x) { return -e[get(x)]; }
    bool same(int x, int y) { return get(x) == get(y); }
    bool unite(int x, int y) {
        x = get(x), y = get(y);
        if (x == y) return false;
        if (-e[x] < -e[y]) swap(x, y);
        e[x] += e[y]; e[y] = x;
        return true;
    }
};

const int HALF = 50000;
const int QUAR = 25000;
const int INF = 1e9;

void energy(int n, std::vector<int> v){
    BIT bit_sum(HALF + 5), bit_cnt(HALF + 5);
    vector<vector<int>> members(n);
    vector<int> cnt(HALF + 5, 0);
    vector<int> who(HALF + 5, -1);
    multiset<int> all_lines;

    set<pi> comps;
    map<int, int> pp;
    DSU dsu(n + 5);

    for (int i = 0; i < n; i++) {
        v[i] %= HALF;
        int perp = (v[i] + QUAR) % HALF;
        if (pp.count(v[i])) {
            dsu.unite(pp[v[i]], i);
        } else if (pp.count(perp)) {
            dsu.unite(pp[perp], i);
        } else {
            who[v[i]] = who[perp] = i;
            pp[v[i]] = pp[perp] = i;
            all_lines.insert(perp);
            all_lines.insert(v[i]);
        }
    }

    for (int i = 0; i < n; i++) {
        int par = dsu.get(i);
        members[par].pb(i);
        if (par == i) {
            comps.insert({dsu.sz(par), par});
        }

        bit_cnt.upd(v[i], 1);
        bit_sum.upd(v[i], v[i]);
    }

    auto acute = [](int x, int y) -> int {
        return min(abs(x - y), HALF - abs(x - y));
    };

    auto contrib = [&](int line) -> ll {
        ll res = 0;
        if (line < QUAR) {
            int perp = line + QUAR;
            res += bit_sum.get(line, perp - 1) - bit_cnt.get(line, perp - 1) * line;
            res += 1ll * (HALF + line) * bit_cnt.get(perp, HALF - 1) - bit_sum.get(perp, HALF - 1);
            res += bit_cnt.get(0, line - 1) * line - bit_sum.get(0, line - 1);
        } else {
            int perp = (line + QUAR) % HALF;
            res += bit_cnt.get(perp + 1, line) * line - bit_sum.get(perp + 1, line);
            res += bit_sum.get(line + 1, HALF - 1) - bit_cnt.get(line + 1, HALF - 1) * line;
            res += bit_cnt.get(0, perp) * (HALF - line) + bit_sum.get(0, perp); 
        }
        return res;
    };

    auto check = [&](vector<int> &lines, int angle) -> ll {
        ll res = 0;

        for (auto id : lines) {
            bit_cnt.upd(v[id], -1);
            bit_sum.upd(v[id], -v[id]);
        }

        for (auto id : lines) {
            res -= contrib(v[id]);
            res += contrib((v[id] + angle) % HALF);
        }

        for (auto id : lines) {
            bit_cnt.upd(v[id], 1);
            bit_sum.upd(v[id], v[id]);
        }

        return res;
    };

    auto rot = [&](int from, int to, int angle) {
        comps.erase(comps.find({(int)members[to].size(), to}));
        all_lines.erase(all_lines.find(v[from]));
        all_lines.erase(all_lines.find((v[from] + QUAR) % HALF));

        for (auto i : members[from]) {
            bit_cnt.upd(v[i], -1);
            bit_sum.upd(v[i], -v[i]);
            (v[i] += angle) %= HALF;
            bit_cnt.upd(v[i], 1);
            bit_sum.upd(v[i], v[i]);
        }

        while (!members[from].empty()) {
            members[to].pb(members[from].back());
            members[from].pop_back();
        }

        comps.insert({members[to].size(), to});
    };

    while ((int)comps.size() > 1) {
        auto [_, id] = *comps.begin();
        comps.erase(comps.begin());
        
        int line = v[id];

        int left = -1, right = -1;
        int mn_left = INF, mn_right = INF;
        {
            auto p1 = all_lines.upper_bound(line);
            auto p2 = all_lines.begin();
            if (p1 != all_lines.end()) {
                mn_left = acute(*p1, line);
                left = who[*p1];
            }
            if (line >= QUAR && *p2 != line && mins(mn_left, acute(*p2, line))) {
                left = who[*p2];
            }
        }
        {
            auto p1 = all_lines.lower_bound(line);
            auto p2 = all_lines.rbegin();
            if (p1 != all_lines.begin()) {
                p1--;
                mn_right = acute(*p1, line);
                right = who[*p1];
            }
            if (line < QUAR && *p2 != line && mins(mn_right, acute(*p2, line))) {
                right = who[*p2];
            }
        }

        if (mn_left != INF && check(members[id], mn_left) >= 0) {
            rotate(members[id], mn_left);
            rot(id, left, mn_left);
            continue;
        }   
        if (mn_right != INF && check(members[id], HALF - mn_right) >= 0) {
            rotate(members[id], HALF - mn_right);
            rot(id, right, HALF - mn_right);
            continue;
        }   
        assert(false);
    }
    vector<int> fir, sec;
    for (int i = 0; i < n; i++) {
        if (v[i] == v[0]) fir.pb(i);
        else sec.pb(i);
    }
    if (fir.size() > sec.size()) swap(fir, sec);
    while ((int)sec.size() > (n / 2)) {
        int cur = sec.back();
        sec.pop_back();
        (v[cur] += QUAR) %= HALF;
        rotate({cur}, QUAR);
    }
}