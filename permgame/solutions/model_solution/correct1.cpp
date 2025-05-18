#include "permgame.h"
#include <bits/stdc++.h>
using namespace std;

#define ii pair<int,int>
#define iii tuple<int,int,int>
#define fi first
#define se second
#define debug(x) cout << #x << ": " << x << endl

#define vi vector<int>
#define vvi vector<vector<int>>
#define zz(x) x.insert(x.begin(),0);

#define pub push_back
#define pob pop_back
#define puf push_front
#define pof pop_front
#define lb lower_bound
#define ub upper_bound

#define rep(x,start,end) for(int x=(start)-((start)>(end));x!=(end)-((start)>(end));((start)<(end)?x++:x--))
#define all(x) (x).begin(),(x).end()
#define sz(x) (int)(x).size()

int n,m;
int arr[405];

vector<int> al[405];
ii edges[405];

vi chain; //this might be line or cycle

vvi cycs;

int decomp(){
	bool vis[405]={};
	cycs.clear();
	
	rep(x,0,n) if (!vis[x]){
		vi cyc={x};
		while (!vis[cyc.back()]){
			vis[cyc.back()]=true;
			cyc.pub(arr[cyc.back()]);
		}
		cyc.pob();
		cycs.pub(cyc);
	}
	
	sort(all(cycs),[](vi a,vi b){
		return sz(a)>sz(b);
	});
	
	int res=n;
	while (!cycs.empty() && sz(cycs.back())==1){
		res--;
		cycs.pob();
	}
	
	return res;
}

void send(vector<int> v){
	//v is in natural sorted order
	//we need to reshuffle it based on chain
	
	vi move(m);
	rep(x,0,m) move[chain[x]]=v[x];
	int idx=Bob(move);
	swap(arr[move[edges[idx].fi]],arr[move[edges[idx].se]]);
}

void splitOdd(vector<int> v){
	//split an odd cycle of size s>m into [2,s-2] or [1,s-1]
	
	vi res;
	for (int x=0;x<m;x+=2) res.pub(v[x]);
	for (int x=m-2;x>0;x-=2) res.pub(v[x]);
	send(res); 
}

void splitEven(vector<int> v){
	//split an even cycle of size s>=m+2 into [m,s-m] or [1,s-1]
	int s=sz(v);
	
	int curr=0;
	vi res={curr};
	rep(x,0,m/2-1){
		if (curr%(2*(s-m)-2)==0) curr+=s-m;
		else curr++;
		res.pub(curr);
	}
	
	curr++;
	res.pub(curr);
	
	curr-=(s-m);
	res.pub(curr);
	
	rep(x,0,m/2-2){
		if (curr%(2*(s-m)-2)==1) curr-=s-m;
		else curr--;
		res.pub(curr);
	}
	
	for (auto &it:res) it=v[it];
	
	send(res);
}

int Alice(int M, int E, vector<int> U, vector<int> V, int N, vector<int> P){
	n=N,m=M;
	
	rep(x,0,n) arr[x]=P[x];
	
	if (m==2){ //annoying case
		decomp();
		for (auto cyc:cycs) rep(x,1,sz(cyc)) Bob({cyc[0],cyc[x]});
		return n;
	}
	
	rep(x,0,E){
		al[U[x]].pub(V[x]);
		al[V[x]].pub(U[x]);
		edges[x]={U[x],V[x]};
	}
	
	int ans=0;
	rep(x,0,n) ans+=(arr[x]==x);
	
	bool bad=ans>=n-m+1;
	rep(x,0,m) if (sz(al[x])>=3) bad=true;
	
	if (bad) return ans;
	
	int r=0;
	rep(x,0,m) if (sz(al[x])==1) r=x;
	chain={r};
	rep(x,0,m-1){
		for (auto it:al[chain.back()]){
			if (sz(chain)==1 || chain[sz(chain)-2]!=it){
				chain.pub(it);
				break;
			}
		}
	}
	
    if (E+1==m || (m%2==0 && ans==n-m)){ //line graph or even cycle special case
		while (decomp()>=m){
			vi v;
			for (auto cyc:cycs) v.insert(v.end(),all(cyc));
			v.resize(m);
			send(v);
		}
		return n-m+1;
	}
	else if (m%2==1){ //odd cycle
		int goal=n-decomp();
		int odd=0,sz=0,ext=0;
		for (auto cyc:cycs){
			if (sz(cyc)%2==1) odd++;
			else sz+=sz(cyc);
		}
		
		for (auto cyc:cycs){
			if (sz(cyc)%2==1 && sz<m){
				ext++;
				sz+=sz(cyc);
			}
		}
		
		goal+=odd-2*(ext/2);
		
		while (decomp()>n-goal){
			vi v;
			
			//if ther is a big odd cycle, do opearation on them
			for (auto cyc:cycs) if (sz(cyc)%2==1){
				if (sz(cyc)>m){
					splitOdd(cyc);
					goto _done;
				}
				if (sz(cyc)==m){
					send(cyc);
					goto _done;;
				}
			}
			
			//there are no big odd cycles, we should try to combine cycles now
			for (auto cyc:cycs) if (sz(cyc)%2==0) v.insert(v.end(),all(cyc));
			while (sz(v)>m-1) v.pob(); //leave space for the odd cycle
			for (auto cyc:cycs) if (sz(cyc)%2==1) v.insert(v.end(),all(cyc));
			v.resize(m);
			send(v);
			
			_done:;
		}
		
		return goal;
	}
	else{ //even cycle
		while (decomp()>m+1){ //phase 1
			int tot=0;
			vi v;
			
			for (auto cyc:cycs) if (sz(cyc)==m){
				send(cyc);
				goto _done2;
			}
			
			for (auto cyc:cycs) if (sz(cyc)>=m+2){
				splitEven(cyc);
				goto _done2;
			}
			
			//try to combine cycles
			for (auto cyc:cycs) if (sz(cyc)>2) tot+=sz(cyc);
			
			if (tot<m+2) break; //end of phase 1
			
			for (auto cyc:cycs){
				v.insert(v.end(),all(cyc));
				//prevent the entire query being 1 cycle
				if (sz(v)==sz(cyc) && sz(v)>m-1) v.resize(m-1);
			}
			v.resize(m);
			send(v);
			
			_done2:;
		}
		
		while (decomp()>m+1){ //phase 2
			vi v;
			
			for (auto cyc:cycs) if (sz(cyc)==m){
				send(cyc);
				goto _done3;
			}
			
			for (auto cyc:cycs) if (sz(cyc)>=2*m-1){
				splitEven(cyc);
				goto _done3;
			}
			
			if (sz(cycs)==1) break; //end of phase 2
			
			for (auto cyc:cycs){
				v.insert(v.end(),all(cyc));
				//prevent the entire query being 1 cycle
				if (sz(v)==sz(cyc) && sz(v)>m-1) v.resize(m-1);
			}
			v.resize(m);
			send(v);
			
			_done3:;
		}
		
		while (decomp()>m+1){ //phase 3
			if (sz(cycs[0])==m) send(cycs[0]);
			else if (sz(cycs[0])>=m+2) splitEven(cycs[0]);
			else{
				vi v;
				for (auto cyc:cycs) v.insert(v.end(),all(cyc));
				v.resize(m);
				send(v);
			}
		}
		
		return n-m-1;
	}
}
