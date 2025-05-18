#include "hack.h"
#include <vector>

using namespace std;
using ll = long long;

const int SKIP = 27380;
const int MAX_N = 1e9;

bool bip_query(vector<ll> A, vector<ll> B){
  vector<ll> C = A;
  C.insert(C.end(), B.begin(), B.end());
  ll total = collisions(C);

  if (total == 0) return 0;
  return true;
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

    for(int i = 1; i <= SKIP; i++){
        A.push_back(i);
    }

    for(int i = SKIP * (MAX_N / 2 / SKIP); i <= MAX_N + SKIP; i += SKIP){
        B.push_back(i);
    }

    while((int) A.size() > 1 || (int) B.size() > 1) {
        if(A.size() > B.size()){
            vector<ll> Al;
            vector<ll> Ar;

            int sz_a = A.size();
            for(int i = 0; i < sz_a / 2; i++){
                Al.push_back(A[i]);
            }
            
            for(int i = sz_a / 2; i < sz_a; i++){
                Ar.push_back(A[i]);
            }

            if(bip_query(Ar, B)){
                A = Ar;
            } else {
                A = Al;
            }
        } else {
            vector<ll> Bl;
            vector<ll> Br;

            int sz_b = B.size();
            for(int i = 0; i < sz_b / 2; i++){
                Bl.push_back(B[i]);
            }

            for(int i = sz_b / 2; i < sz_b; i++){
                Br.push_back(B[i]);
            }

            if(bip_query(A, Bl)){
                B = Bl;
            } else {
                B = Br;
            }
        }
    }

    int xl = A[0];
    int xr = B[0];

    int diff = xr - xl;

    for (auto p : prime_factor(diff)) {
        diff /= p;
        if (collisions({1, diff + 1}) == 0){
            diff *= p;
        }
    }

    return diff;
}