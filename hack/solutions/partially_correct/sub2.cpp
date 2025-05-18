#include "hack.h"
#include <bits/stdc++.h>

using namespace std;

long long f(int n, int k = (int)1e6){
    int r = k % n;
    int cnt = k / n;

    long long res = 0;
    res += 1LL * r * cnt * (cnt + 1) / 2;
    res += 1LL * (n - r) * cnt * (cnt - 1) / 2;

    return res;
}

int hack(){
    int k = (int)1e6;
    vector<long long> x(k);
    iota(x.begin(), x.end(), 1);
    
    long long res = collisions(x);

    int l = 1, r = k;
    while (l <= r){
        int m = (l + r) >> 1;

        if (f(m) == res) return m;
        
        if (f(m) < res){
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    return 42;
}