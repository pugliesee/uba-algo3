#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;
double C;
double resKruskal = 0;
pair<long long, long long> DyR_res;


struct DSU {
	DSU(int n){
		padre = vector<int>(n);
		for(int v = 0; v < n; v++) padre[v] = v;
		tamano = vector<int>(n,1);
	}

	int find(int v){
		while(padre[v] != v) v = padre[v];
		return v;
	}

	void unite(int u, int v){
		u = find(u); v = find(v);
		if(tamano[u] < tamano[v]) swap(u,v);
		//ahora u es al menos tan grande como v
		padre[v] = u;
		tamano[u] += tamano[v];
	}

	vector<int> padre;
	vector<int> tamano;

	//tamano[v] <= n
	//INV: si padre[v] != v entonces tamano[padre[v]] >= 2*tamano[v]
};



bool comparacion(tuple<int,int,long long,long long> edge1, tuple<int,int,long long,long long> edge2){
	long long d1 = get<2>(edge1);
	long long r1 = get<3>(edge1);
	long long d2 = get<2>(edge2);
	long long r2 = get<3>(edge2);

	return (C* double(r1) - double(d1)) < (C* double(r2) - double(d2));
}


pair<long long, long long> kruskal(vector<tuple<int,int,long long,long long>>& E, int n) {
	long long resD = 0;
	long long resR = 0;
	sort(E.begin(), E.end(), comparacion);
	DSU dsu(n);
	resKruskal = 0;
	int aristas = 0;
	for(auto [u,v, d, r] : E) {
		//u y v estan en distinta cc?
		if (dsu.find(u) != dsu.find(v)) {
			dsu.unite(u,v);
			resD += d;
			resR += r;
			resKruskal += C * double(r) - double(d);
			aristas++;
		}
		if (aristas == n-1) break;
	}
	//resKruskal = resD/resR;
	if (aristas == n-1) return make_pair(resD,resR);
	else { // No juntamos todas las CC.
		//cout << "IMPOSSIBLE\n";
		return make_pair(0,0);
	}
}

// listaAdy (vecino, distancia, repetidores) ; E (u, v, distancia, repetidores)
void resolver(int n, vector<tuple<int, int, long long, long long>> &E) {
	// Busq bin sobre C
	resKruskal = 0;
    int iteraciones = 0;
	double C_low = 0;
	double C_high = C;
	double eps = 10e-6;
	pair<long long, long long> DyR = kruskal(E, n);  // PRIMERA ITERACIÓN KRUSKAL
	while (C_low < C_high && abs(C_high - C_low) > eps && iteraciones < 70) {
        iteraciones++;
		double C_mid = (C_low + C_high) / 2;
		C = C_mid;
		double actual = double(DyR.first) / double(DyR.second);
		double actual_res = double(DyR_res.first) / double(DyR_res.second);
		if (actual > actual_res) DyR_res = DyR;
		DyR = kruskal(E,n);

		if (resKruskal >= 0) C_high = C_mid;
		else C_low = C_mid;

	}

    C = (C_low + C_high) / 2;
    DyR = kruskal(E, n);


	if (double(DyR.first) / double(DyR.second) < double(DyR_res.first) / double(DyR_res.second)) {
		cout << DyR_res.first << " " << DyR_res.second << endl;
	} else {
		cout << DyR.first << " " << DyR.second << endl;
	}



}


int main() {
	ifstream fin("./3ej_input.txt");
	if (!fin.is_open()) {
		cerr << "Error: could not open input file 3ej_input.txt" << endl;
		return 1;
	}
	int tests;      // Cantidad de tests
	fin >> tests;

	while(tests--) {
		C = 0;
		int n; //filas
		int m; //columnas
		fin >> n >> m;

		vector<tuple<int,int,long long,long long>> E;
		for (int i = 0; i < m; i++) {
			int u, v;
			long long d, r;
			fin >> u >> v >> d >> r;  // (uv) ARISTA, d DISTANCIA, r REPETIDORES
			tuple<int,int,long long,long long> edge = make_tuple(u-1,v-1,d,r);
			E.push_back(edge);
			C += double(d);
		}
		DyR_res = make_pair(0, -1);
		resolver(n, E);

	}
}
