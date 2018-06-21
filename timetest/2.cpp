#include <bits/stdc++.h>

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...)
#endif

#ifdef __WIN32
#define LLFORMAT "I64"
#define Rand() ((rand() << 15) + rand())
#else
#define LLFORMAT "ll"
#define Rand() (rand())
#endif

using namespace std;

const int maxn = 1e5 + 10, maxm = 2e5 + 10;

int n, m_n, q_n, u[maxn], v[maxn], i[maxn], ans[maxn], bel[maxn], mu[maxn], mv[maxn], par[maxn], fuck_id;
char par_c[maxn];
string m[maxm];
vector<pair<int, char> > g[maxn];
vector<int> has[maxn], border[maxm];
bool vis[maxn];

int extgcd(int a, int &x, int b, int &y) {
	if(!b) {
		x = 1;
		y = 0;
		return a;
	}
	int t = a / b, ret = extgcd(b, y, a - t * b, x);
	y -= t * x;
	return ret;
}

int F(int l1, int r1, int d1, int l2, int r2, int d2) {
	if(l1 > l2) {
		return F(l2, r2, d2, l1, r1, d1);
	}
	int x, y, d = extgcd(d1, x, d2, y);
	if((l2 - l1) % d != 0) {
		return 0;
	}
	long long Y = (long long) (l2 - l1) / d * -y;
	d1 /= d;
	Y = (Y % d1 + d1) % d1;
	long long foo = (long long) Y * d2 + l2;
	if(foo > min(r1, r2)) {
		return 0;
	}
	long long k = (min(r1, r2) - foo) / (d1 * d2);
	return (int) k + 1;
}

struct Info {
	int l, r, d;

	Info() {}
	Info(int l, int r, int d): l(l), r(r), d(d) {}
};

int lowbit(int i) {
	return i & (-i);
}

namespace ac {
	int go[maxm][26], fail[maxm], SZ, pos[maxm], dfn[maxm], End[maxm], fen[maxm];
	vector<int> g[maxm];

	void M(int i, int x) {
		for(; i <= SZ; i += lowbit(i)) {
			fen[i] += x;
		}
		return;
	}

	int Q(int i) {
		int x = 0;
		for(; i; i ^= lowbit(i)) {
			x += fen[i];
		}
		return x;
	}

	void m(int i, int x) {
		M(dfn[i], x);
		return;
	}

	int q(int i) {
		return Q(End[i]) - Q(dfn[i] - 1);
	}

	void dfs(int u) {
		dfn[u] = ++SZ;
		for (int &v: g[u]) {
			dfs(v);
		}
		End[u] = SZ;
		return;
	}

	void ins(int i) {
		int u = 0;
		for (char &c: ::m[i]) {
			int j = c - 'a';
			if(!go[u][j]) {
				go[u][j] = ++SZ;
			}
			u = go[u][j];
		}
		pos[i] = u;
		return;
	}

	void build() {
		static int q[maxm], fnt, rar;
		fnt = rar = 0;
		for (int i = 0; i < 26; ++i) {
			if(go[0][i]) {
				q[rar++] = go[0][i];
			}
		}
		while(fnt != rar) {
			int u = q[fnt++];
			for (int i = 0; i < 26; ++i) {
				if(go[u][i]) {
					int v = fail[u];
					while(v && !go[v][i]) {
						v = fail[v];
					}
					fail[q[rar++] = go[u][i]] = go[v][i];
				}
				else {
					go[u][i] = go[fail[u]][i];
				}
			}
		}
		for (int i = 1; i <= SZ; ++i) {
			g[fail[i]].push_back(i);
		}
		SZ = 0;
		dfs(0);
		return;
	}
}

namespace sam {
	const int lgn = 19;
	int go[maxm << 1][26], mx[maxm << 1], SZ, pos[maxm], dfn[maxm << 1], End[maxm << 1], fen[maxm << 2], par[maxm << 1][lgn];
	set<int> s[maxm << 1];
	vector<int> g[maxm << 1];

	void M(int i, int x) {
		for(; i <= SZ; i += lowbit(i)) {
			fen[i] += x;
		}
		return;
	}

	int Q(int i) {
		int x = 0;
		for(; i; i ^= lowbit(i)) {
			x += fen[i];
		}
		return x;
	}

	void m(int i, int x, int l) {
		if(~i) {
			M(dfn[i], x);
			M(End[i], -x);
			if(x > 0) {
				s[i].insert(l);
			}
			else {
				s[i].erase(l);
			}
		}
		return;
	}

	int q(int u, int L) {
		if(s[u].size()) {
			auto it = s[u].lower_bound(L);
			if(it != s[u].begin()) {
				return *--it;
			}
		}
		int I = dfn[par[u][0]];
		for (int i = lgn - 1; ~i; --i) {
			int p = par[u][i];
			if(Q(I) - Q(dfn[p] - 1) == 0) {
				u = p;
			}
		}
		u = par[u][0];
		if(!u) {
			return 0;
		}
		return *--s[u].end();
	}

	void dfs(int u) {
		dfn[u] = ++SZ;
		for (int i = 1; i < lgn; ++i) {
			par[u][i] = par[par[u][i - 1]][i - 1];
		}
		for (int &v: g[u]) {
			dfs(v);
		}
		End[u] = ++SZ;
		return;
	}

	void append(int &last, int i) {
		int p = last;
		if(~go[p][i]) {
			last = go[p][i];
			return;
		}
		int np = ++SZ;
		mx[np] = mx[p] + 1;
		while(~p && !~go[p][i]) {
			go[p][i] = np;
			p = par[p][0];
		}
		if(!~p) {
			par[np][0] = 0;
		}
		else {
			int q = go[p][i];
			if(mx[q] == mx[p] + 1) {
				par[np][0] = q;
			}
			else {
				int nq = ++SZ;
				memcpy(go[nq], go[q], sizeof(go[nq]));
				memcpy(par[nq], par[q], sizeof(par[nq]));
				mx[nq] = mx[p] + 1;
				par[q][0] = par[np][0] = nq;
				while(~p && go[p][i] == q) {
					go[p][i] = nq;
					p = par[p][0];
				}
			}
		}
		last = np;
		return;
	}

	void init() {
		memset(go, -1, sizeof go);
		memset(par, -1, sizeof par);
		return;
	}

	void build() {
		static pair<pair<int, int>, int> q[maxm];
		int fnt = 0, rar = 0;
		for (int i = 1; i <= m_n * 2; ++i) {
			q[rar++] = make_pair(make_pair(i, 0), 0);
		}
		while(fnt != rar) {
			int i = q[fnt].first.first, j = q[fnt].first.second, u = q[fnt].second;
			++fnt;
			append(u, ::m[i][j] - 'a');
			if(j == ::m[i].size() - 1) {
				pos[i] = u;
			}
			else {
				q[rar++] = make_pair(make_pair(i, j + 1), u);
			}
		}
		par[0][0] = 0;
		for (int i = 1; i <= SZ; ++i) {
			g[par[i][0]].push_back(i);
		}
		SZ = 0;
		dfs(0);
		return;
	}
}

int find_rt(int u) {
	static int q[maxn], fnt, rar, par[maxn], hs[maxn], sz[maxn];
	fnt = rar = 0;
	par[q[rar++] = u] = 0;
	while(fnt != rar) {
		u = q[fnt++];
		hs[u] = 0;
		sz[u] = 1;
		for (auto &e: g[u]) {
			if(!vis[e.first] && e.first != par[u]) {
				par[q[rar++] = e.first] = u;
			}
		}
	}
	for (int i = rar - 1; ~i; --i) {
		u = q[i];
		if((max(hs[u], rar - sz[u]) << 1) <= rar) {
			return u;
		}
		int p = par[u];
		sz[p] += sz[u];
		hs[p] = max(hs[p], sz[u]);
	}
}

void dfs1(int u, int p, int rt) {
	bel[u] = rt;
	for (auto &e: g[u]) {
		if(!vis[e.first] && e.first != p) {
			dfs1(e.first, u, rt);
		}
	}
	return;
}

void dfs2(int u, int p, int acp, int samp, int l) {
	par[u] = p;
	ac::m(acp, 1);
	sam::m(samp, 1, l);
	static vector<int> nhas;
	nhas.clear();
	for (int i: has[u]) {
		if(bel[::u[i]] == bel[::v[i]]) {
			nhas.push_back(i);
			continue;
		}
		if(u == ::u[i]) {
			ans[i] += ac::q(ac::pos[::i[i] + m_n]);
			mu[i] = sam::q(sam::pos[::i[i] + m_n], m[::i[i]].size());
		}
		else {
			ans[i] += ac::q(ac::pos[::i[i]]);
			mv[i] = sam::q(sam::pos[::i[i]], m[::i[i]].size());
		}
	}
	has[u] = nhas;
	for (auto &e: g[u]) {
		if(!vis[e.first] && e.first != p) {
			par_c[e.first] = e.second;
			dfs2(e.first, u, ac::go[acp][e.second - 'a'], ~samp ? sam::go[samp][e.second - 'a'] : -1, l + 1);
		}
	}
	ac::m(acp, -1);
	sam::m(samp, -1, l);
	return;
}

void solve(int rt) {
	rt = find_rt(rt);
	bel[rt] = rt;
	for (auto &e: g[rt]) {
		if(!vis[e.first]) {
			dfs1(e.first, rt, e.first);
		}
	}
	par[rt] = 0;
	for (auto &e: g[rt]) {
		if(!vis[e.first]) {
			par_c[e.first] = e.second;
			dfs2(e.first, rt, ac::go[0][e.second - 'a'], sam::go[0][e.second - 'a'], 1);
		}
	}
	vis[rt] = 1;
	for (auto &e: g[rt]) {
		if(!vis[e.first]) {
			solve(e.first);
		}
	}
	return;
}

int main() {
	ios::sync_with_stdio(false);
	freopen("input2.txt", "r", stdin);
	freopen("output2.txt", "w", stdout);
	cin >> n >> m_n >> q_n;
	for (int i = 1; i < n; ++i) {
		int u, v;
		char c;
		cin >> u >> v >> c;
		g[u].push_back(make_pair(v, c));
		g[v].push_back(make_pair(u, c));
	}
	for (int i = 1; i <= m_n; ++i) {
		cin >> m[i];
		m[i + m_n] = m[i];
		reverse(m[i + m_n].begin(), m[i + m_n].end());
	}
	sam::init();
	for (int i = 1; i <= m_n * 2; ++i) {
		ac::ins(i);
		int n = m[i].size();
		auto &b = border[i];
		b.resize(n + 1);
		b[0] = -1;
		for (int k = 0; k < n; ++k) {
			int j = b[k];
			while(~j && m[i][j] != m[i][k]) {
				j = b[j];
			}
			b[k + 1] = j + 1;
		}
	}
	ac::build();
	sam::build();
	for (int i = 1; i <= q_n; ++i) {
		cin >> u[i] >> v[i] >> ::i[i];
		has[u[i]].push_back(i);
		has[v[i]].push_back(i);
	}
	solve(1);
	for (int i = 1; i <= q_n; ++i) {
		if(mu[i] && mv[i]) {
			static vector<Info> vu, vv;
			auto &bu = border[::i[i]], &bv = border[::i[i] + m_n];
			int mu = ::mu[i], mv = ::mv[i];
			vu.clear();
			vv.clear();
			while(mu) {
				int nu = bu[mu], d = mu - nu, t = mu - (mu >> 1), k = t / d;
				if(k * d == t) {
					--k;
				}
				nu = mu - k * d;
				vu.push_back(Info(nu, mu, d));
				mu = bu[nu];
			}
			reverse(vu.begin(), vu.end());
			while(mv) {
				int nv = bv[mv], d = mv - nv, t = mv - (mv >> 1), k = t / d;
				if(k * d == t) {
					--k;
				}
				nv = mv - k * d;
				vv.push_back(Info(nv, mv, d));
				mv = bv[nv];
			}
			reverse(vv.begin(), vv.end());
			int len = m[::i[i]].size();
			for (auto t: vu) {
				int l = t.l, r = t.r, d = t.d;
				while(vv.size()) {
					auto s = vv.back();
					int ll = s.l, rr = s.r, dd = s.d;
					if(l + ll > len) {
						vv.pop_back();
						continue;
					}
					ans[i] += F(len - r, len - l, d, ll, rr, dd);
					if(r + 1 + ll > len) {
						vv.pop_back();
					}
					else {
						break;
					}
				}
			}
		}
		printf("%d\n", ans[i]);
	}
	return 0;
}