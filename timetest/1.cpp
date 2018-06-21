#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=4e7+10,M=3e6+10;
int prime[M],pcnt;
bitset<N> isp;
void init(){
	int n=4e7;
	for (int i=2;i<=n;i++){
		if (!isp[i]) prime[++pcnt]=i;
		for (int j=1;j<=pcnt&&i*prime[j]<=n;j++){
			isp[i*prime[j]]=1;
			if (i%prime[j]==0) break;
		}
	}
}
int T;ll n,k;
struct linear_space{
	vector<ll> a;
	ll *dis,p;
	linear_space():dis(0),p(0){}
	void init(vector<ll> &A){
		a=A;
		if (a.size()<3) return;
		p=a[0];
		dis=new ll[p];
		dis[0]=0;
		for (int i=1;i<p;i++) 
			dis[i]=1e18;
		for (ll v:a) insert(v);
	}
	void insert(ll v){
		ll lim=__gcd(v,p);
		for (ll i=0;i<lim;i++){
			for (ll x=i,y=-1,z;y!=i;x=y=z){
				z=(x+v)%p;
				dis[z]=min(dis[z],dis[x]+v);
			}
			for (ll x=i,y=-1,z;y!=i;x=y=z){
				z=(x+v)%p;
				dis[z]=min(dis[z],dis[x]+v);
			}
		}
	}
	typedef long long i128;
	void exgcd(ll a,ll b,ll c,i128 &x,i128 &y){
		if (!b) x=c/a,y=0;
		else{
			exgcd(b,a%b,c,y,x);
			if (x>=0) x%=b;else x+=(-x+b-1)/b*b;
			y=(c-a*x)/b;
		}
	}
	bool check(i128 a,i128 b,i128 n){
		if (n>=a*b) return 1;
		i128 x,y;
		exgcd(a,b,n,x,y);
		i128 t=(x<=0?(-x+b-1)/b:-(x/b));
		x+=t*b;y-=t*a;
		return y>=0;
	}
	bool query(ll n){
		if (a.size()==0) return false;
		if (a.size()==1) return n%a[0]==0;
		if (a.size()==2) return check(a[0],a[1],n);
		return dis[n%p]<=n;
	}
};
linear_space& factor(ll n){
	static map<ll,linear_space> Map;
	if (Map.count(n)) return Map[n];
	ll N=n;
	vector<ll> a;
	for (int i=1;(ll)prime[i]*prime[i]<=n;i++)
	if (n%prime[i]==0){
		a.push_back(prime[i]);
		while (n%prime[i]==0) n/=prime[i];
	}
	if (n>1) a.push_back(n);
	Map[N].init(a); 
	return Map[N];
}
int main()
{
	freopen("input1.txt", "r", stdin);
	freopen("output1.txt", "w", stdout);
	init();
	scanf("%d",&T);
	while (T--){
		scanf("%lld%lld",&n,&k);
		puts(factor(k).query(n)?"YES":"NO");
	}
	return 0;
}