#include "hack.h"
// #include <vector>
 
#include<bits/stdc++.h>
 
using namespace std;
 
int hack(){
  long long l,r;
  l=1; r=1000000000;
  while(l<r){
    long long m=(l+r)/2;
    // check whether [l,m]
    long long sq=sqrt(m-l+1);
    vector<long long> ask;
    for(long long i=1;i<=sq;i++){ ask.push_back(i); }
    long long ar=m+1;
    while(ar>sq){
      ask.push_back(ar);
      if((ar-sq)<=l){break;}
      ar-=sq;
    }
    // cout << ask.size() << "\n";
    if(collisions(ask)>0){r=m;}
    else{l=m+1;}
  }
  return l;
}