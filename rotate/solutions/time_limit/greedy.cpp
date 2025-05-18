#include "rotate.h"
#include <bits/stdc++.h>

using namespace std;

const int MAGIC = 50000;

long long calc_energy(vector<int> a){
    int n = (int) a.size();

    long long ans = 0;
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            int d = abs(a[i] - a[j]);
            ans += min(d, MAGIC - d);
        }
    }

    return ans;
}

void energy(int n, vector<int> v){
    long long max_nrg = 0;
    vector<pair<int, int>> answer;

    for (int x = 0; x < MAGIC; x++){
        int y = x + MAGIC / 2;
        long long cur = calc_energy(v);
        vector<int> w = v;
        vector<pair<int, int>> how;

        for (int i = 0; i < n; i++){
            w[i] = x;
            long long cx = calc_energy(v);
            
            w[i] = y;
            long long cy = calc_energy(v);
            
            if (max(cx, cy) < cur){
                w[i] = v[i];
                continue;
            }

            if (cx >= cy){
                cur = cx;
                w[i] = x;
                how.emplace_back(i, x - v[i]);
            } else {
                cur = cy;
                w[i] = y;
                how.emplace_back(i, y - v[i]);
            }
        }

        if (cur >= max_nrg){
            max_nrg = cur;
            answer = how;
        }

    }

    for (auto [i, x] : answer){
        x %= MAGIC;
        if (x < 0) x += MAGIC;
        if (x == 0) continue;

        rotate({i}, x);
    }

}