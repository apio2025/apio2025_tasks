#include "hack.h"
#include <bits/stdc++.h>

using namespace std;

map<vector<long long>, long long> memo;
long long ask(vector<long long> x){
    sort(x.begin(), x.end());
    if (memo.find(x) != memo.end()){
        return memo[x];
    }
    return memo[x] = collisions(x);
}

vector<long long> merge(vector<long long> vl, vector<long long> vr){
    vector<long long> res = vl;
    res.insert(res.end(), vr.begin(), vr.end());
    return res;
}

long long solve(vector<long long> A, vector<long long> B){
    int szA = (int) A.size(), szB = (int) B.size();
    if (szA == 1 && szB == 1) return llabs(A[0] - B[0]);

    if (szA < szB){
        swap(A, B);
        swap(szA, szB);
    }

    vector<long long> L(A.begin(), A.begin() + szA / 2);
    vector<long long> R(A.begin() + szA / 2, A.end());

    long long cnt = ask(merge(L, B));
    cnt -= ask(L);
    cnt -= ask(B);

    if (cnt > 0){
        return solve(L, B);
    } else {
        return solve(R, B);
    }
}

int hack(){
    int L = 22370;
    memo.clear();
    
    vector<long long> A;
    for (int i = 1; i <= L; i++){
        A.push_back(i);
    }

    vector<long long> B;
    for (int i = 1; i <= L; i++){
        int mid = (int)5e8;
        B.push_back(mid + i * L);
    }

    long long n = solve(A, B);

    long long m = n;

    vector<pair<long long, int>> primes;
    for (long long p = 2; p * p <= m; p++){
        int q = 0;
        while (m % p == 0){
            m /= p;
            q++;
        }
        if (q) primes.emplace_back(p, q);
    }

    if (m != 1){
        primes.emplace_back(m, 1);
    }

    for (auto [p, q] : primes){
        while (q--){
            if (ask({1, n / p + 1}) == 0) break;
            n /= p;
        }
    }

    return n;
}