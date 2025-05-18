#include "hack.h"
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;
using ll = long long;

const int SKIP = 22500;
const int MAX_N = 1e9;

ll calc(int n, int k){
  ll res = 0;

  for (int x = k; x < n; x += k){
    res += (n - x);
  }

  return res;
}

bool bip_query(vector<ll> A, vector<ll> B){
  vector<ll> C = A;
  C.insert(C.end(), B.begin(), B.end());
  ll total = collisions(C);

  if (total == 0) return 0;

  //What gap exactly matches the total
  int n = B.size();

  int lo = 0; //Too small (calc too big)
  int hi = n; //Too big (calc too small)
  int found = -1;
  while (lo + 1 < hi){
    int mid = (lo + hi) / 2;

    ll val = calc(n, mid);
    if (val > total) {
      lo = mid;
    }
    else if (val < total) {
      hi = mid;
    } else {
      assert (val == total);
      found = mid;
      break;
    }
  }

  if (found == -1){
    // B cannot possibly account for all collisions

    return true;
  }

  if (collisions({B[0], B[found]})){
    // The collisions that are multiples of B[found] - B[0] within B
    // account for all collisions within C (no bipartite collisions)

    return false;
  } else {
    // Fewer collisions in B than in C

    return true;
  }
}

vector<int> prime_factor(int x){
  vector<int> out;

  int test = 2;
  while (test * test <= x){
    if (x % test == 0){
      out.push_back(test);
      x /= test;
    } else {
      test += 1;
    }
  }

  if (x > 1) out.push_back(x);

  return out;
}

int hack() {
  vector<ll> A, B;

  for (int i = 1; i <= SKIP; i++){
    A.push_back(i);
  }

  for (int i = MAX_N / 2; i <= MAX_N + SKIP; i += SKIP){
    B.push_back(i);
  }

  while ((int) A.size() > 1 || (int) B.size() > 1) {
    if (A.size() > B.size()){
      assert (A.size() > 1);

      vector<ll> Al;
      vector<ll> Ar;

      int sz_a = A.size();
      for (int i = 0; i < sz_a / 2; i++){
        Al.push_back(A[i]);
      }
      for (int i = sz_a / 2; i < sz_a; i++){
        Ar.push_back(A[i]);
      }

      if (bip_query(Al, B)){
        A = Al;
      } else {
        A = Ar;
      }
    } else {
      assert (B.size() > 1);

      vector<ll> Bl;
      vector<ll> Br;

      int sz_b = B.size();
      for (int i = 0; i < sz_b / 2; i++){
        Bl.push_back(B[i]);
      }
      for (int i = sz_b / 2; i < sz_b; i++){
        Br.push_back(B[i]);
      }

      if (bip_query(A, Bl)){
        B = Bl;
      } else {
        B = Br;
      }
    }
  }

  int xl = A[0];
  int xr = B[0];

  assert (collisions({xl, xr}) == 1);
  assert (xl < xr);

  int diff = xr - xl;

  for (auto p : prime_factor(diff)) {
    diff /= p;
    if (collisions({1, diff + 1}) == 0){
      diff *= p;
    }
  }

  return diff;
}