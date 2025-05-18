#include "permgame.h"
#include <cassert>
#include <cstdio>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>

// BEGIN SECRET
#include <string>
// END SECRET

static void exit_mes(const std::string& mes, double points = 0.0) {
    const std::string output_secret = "pqiEqPoBP5S4lurCzVusSd8sgVnBBnSy";
    printf("%s\n", output_secret.c_str());
    printf("%lf\n", points);
    printf("%s\n", mes.c_str());
    fclose(stdout);
    exit(0);
}

static std::mt19937 rng(123);

static int strategy = 0;
static int M, E, N;
static int U[405];
static int V[405];
static int P[405];

static int cnt[405];
static int moves_taken = 0;

static std::vector<std::vector<int>> adj;
static bool bad = false;
static void score_precomputation() {
    std::vector<int> deg(M);
    adj.resize(N);
    for (int i = 0; i < E; i++) {
        adj[U[i]].push_back(V[i]);
        adj[V[i]].push_back(U[i]);
    }
    for (auto& x : adj)
        bad |= x.size() >= 3;
}

static int score() {
    if (M == 2) return N;
    int score = 0;
    for (int i = 0; i < N; i++)
        score += P[i] == i;
    if (bad) return score;

    if (M == E+1) { // tree
        if (score >= N-M+1) return score;
        else return N-M+1;

    } else if (M % 2 == 1) {
        std::vector<int> cycles;
        std::vector<int> vis(N);
        for (int i = 0; i < N; i++) {
            if (vis[i]) continue;
            int j = i;
            cycles.push_back(0);
            while (!vis[j]) {
                cycles.back()++;
                vis[j] = 1;
                j = P[j];
            }
        }

        std::sort(cycles.begin(), cycles.end(), std::greater<int>());
        while (cycles.size() && cycles.back() == 1) cycles.pop_back();
        int sz = 0, ext = 0, odd = 0;
        for (int i : cycles)
            if (i % 2 == 0) sz += i;
            else odd++;
        for (int i : cycles) {
            if (i % 2 == 1 && sz < M) ext++, sz += i;
        }
        if (score >= N - M + 1) return score;
        else return score + odd - 2*(ext/2);
    } else if (M % 2 == 0) {
        if (score >= N - M + 1) return score;
        else if (score == N - M) return score+1;
        else return N - M - 1;
    } else assert(false);
}

static int optimal_move(const std::vector<int> &i) {
    std::vector<int> idx(E);
    std::iota(idx.begin(), idx.end(), 0);
    std::shuffle(idx.begin(), idx.end(), rng);
    int best = N, choice = 0;
    for (int j : idx) {
        std::swap(P[i[U[j]]], P[i[V[j]]]);
        int s = score();
        if (s < best) best = s, choice = j;
        std::swap(P[i[U[j]]], P[i[V[j]]]);
    }
    return choice;
}
static int greedy_move(std::vector<int> i) {
    int choice = 0;
    int best = 999;

    std::vector<int> idx(E);
    std::iota(idx.begin(), idx.end(), 0);
    std::shuffle(idx.begin(), idx.end(), rng);

    for (auto x : idx) {
        int change = (P[i[V[x]]] == i[U[x]]) - (P[i[U[x]]] == i[U[x]]) +
                     (P[i[U[x]]] == i[V[x]]) - (P[i[V[x]]] == i[V[x]]);
        if (change < best) {
            best = change;
            choice = x;
        }
    }
    return choice;
}
static int weird_move(const std::vector<int> &i) {
    if (rng() % 2) return optimal_move(i);
    if (rng() % 2) return greedy_move(i);
    return rng() % E;
}

int Bob(std::vector<int> i) {
    moves_taken++;
    if (moves_taken >= 12 * N) {
        exit_mes("Too many moves");
    }
    if (i.size() != M) {
        exit_mes("Invalid interaction, t is not size m");
    }
    for (auto x : i) {
        if (x < 0 || x >= N) {
            exit_mes("Invalid interaction, t[i] out of range");
        }
        if (cnt[x]) {
            exit_mes("Invalid interaction, t has duplicate elements");
        }
        cnt[x]++;
    }
    for (auto x : i)
        cnt[x]--;

    int choice;
    if (strategy == 0) choice = optimal_move(i);
    else if (strategy == 1) choice = greedy_move(i);
    else if (strategy == 2) choice = weird_move(i);

    std::swap(P[i[U[choice]]], P[i[V[choice]]]);
    // printf("choice: %d\n",choice);
    return choice;
}

int main() {
    // BEGIN SECRET
    const std::string input_secret = "uGrv8L9msSm7OMWkYDyr7NMBlSphJ7et";
    const std::string output_secret = "pqiEqPoBP5S4lurCzVusSd8sgVnBBnSy";
    char secret[1000];
    assert(1 == scanf("%999s", secret));
    if (std::string(secret) != input_secret) {
        printf("%s\n", output_secret.c_str());
        printf("PV\n");
        printf("Possible tampering with the input\n");
        fclose(stdout);
        return 0;
    }
    // END SECRET

    assert(1 == scanf("%d", &strategy));
    assert(2 == scanf("%d%d", &M, &E));
    std::vector<int> u, v;
    for (int x = 0; x < E; x++) {
        assert(2 == scanf("%d%d", &U[x], &V[x]));
        u.push_back(U[x]);
        v.push_back(V[x]);
    }

    assert(1 == scanf("%d", &N));

    std::vector<int> p;
    for (int x = 0; x < N; x++) {
        assert(1 == scanf("%d", &P[x]));
        p.push_back(P[x]);
    }

    score_precomputation();
    int optimal = score();

    int score = Alice(M, E, u, v, N, p);
    int actual_score = 0;
    for (int x = 0; x < N; x++)
        actual_score += P[x] == x;

    fclose(stdin);
    if (strategy == 0) assert(actual_score <= optimal);
    
    if (actual_score < score) {
        exit_mes("final score is less than reported score");
    }
    
    if (score != optimal){
        exit_mes("reported score is not optimal");
    }

    if (moves_taken <= 3 * N){
        exit_mes("OK", 1.0);
    } else if (moves_taken >= 12 * N){
        exit_mes("Too many moves", 0.0);
    } else {
        double ratio = (double) moves_taken / N;
        double points = 1.0 - log10(ratio - 2);
        exit_mes("Partially correct", points);
    }

    fclose(stdout);
    return 0;
}

