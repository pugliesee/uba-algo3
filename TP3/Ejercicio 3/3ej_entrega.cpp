#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

int INF = 10e7;


int bfs(int s, int t, vector<int>& parent, vector<vector<int>> &listAdy, vector<vector<int>> &capacidad) {
	fill(parent.begin(), parent.end(), -1);
	parent[s] = -2;
	queue<pair<int, int>> q;
	q.push({s, INF});

	while (!q.empty()) {
		int cur = q.front().first;
		int flow = q.front().second;
		q.pop();

		for (int next : listAdy[cur]) {
			if (parent[next] == -1 && capacidad[cur][next]) {
				parent[next] = cur;
				int new_flow = min(flow, capacidad[cur][next]);
				if (next == t)
					return new_flow;
				q.push({next, new_flow});
			}
		}
	}

	return 0;
}

int maxflow(int s, int t, vector<vector<int>> &listAdy, vector<vector<int>> &capacidad) {
	int n = listAdy.size();
	int flow = 0;
	vector<int> parent(n);
	int new_flow;

	while (new_flow = bfs(s, t, parent,listAdy,capacidad)) {
		flow += new_flow;
		int cur = t;
		while (cur != s) {
			int prev = parent[cur];
			capacidad[prev][cur] -= new_flow;
			capacidad[cur][prev] += new_flow;
			cur = prev;
		}
	}

	return flow;
}



vector<vector<int>> mediaListAdy (vector<vector<int>> &tablero, vector<vector<int>> &tablero2) {
	vector<vector<int>> listAdy;
	// El primer elemento de la lista de adyacencia es la FUENTE del flujo (el nodo "S")
	listAdy.push_back({});
	int n = tablero.size();

	bool aumentar = false;
	int filaNum = 1;
	for (int i = 0; i < n; i++) { // Recorro todas las filas y armo vector de subfilas
		for (int j = 0; j < n; j++) {
			if (tablero[i][j] == 1){
				if (aumentar) filaNum++;
				tablero2[i][j] = -1;
				aumentar = false;
			}
			else {
				aumentar = true;
				tablero2[i][j] = filaNum;
			}
		}
		if (aumentar) {
			filaNum++;
			aumentar = false;
		}
	}

	filaNum--;
	for (int i = 0; i < filaNum; i++){
		listAdy[0].push_back(i+1);
		listAdy.push_back({0});
	}

	return listAdy;
}


void terminarListaAdy(vector<vector<int>> &tablero2, vector<vector<int>> &listAdy) {
	int n = tablero2.size();
	int filas = listAdy.size();

	vector<vector<int>> tablero3 = tablero2;

	bool aumentar = false;
	int columnNum = filas;
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < n; i++) {
			if (tablero2[i][j] == -1){
				if (aumentar) columnNum++;
				tablero3[i][j] = -1;
				aumentar = false;
			}
			else {
				aumentar = true;
				tablero3[i][j] = columnNum;
			}
		}
		if (aumentar) {
			columnNum++;
			aumentar = false;
		}
	}

	set<pair<int,int>> aristas;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tablero2[i][j] != -1) {
				aristas.insert(make_pair(tablero2[i][j], tablero3[i][j]));
			}
		}
	}

	int t = columnNum;
	for (int i = filas; i < columnNum; i++){
		listAdy.push_back({t});
	}
	listAdy.push_back({});

	for (pair<int, int> arista : aristas) {
		listAdy[arista.first].push_back(arista.second);
		listAdy[arista.second].push_back(arista.first);
	}

	for (int i = filas; i < columnNum; i++) listAdy[columnNum].push_back(i);


}



int main() {
	int tests;      // Cantidad de tests
	cin >> tests;

	while(tests--) {

		int n;
		cin >> n;   // Tamaño del tablero

		vector<vector<int>> tablero;
		for (int i = 0; i < n; i++) {   // Armamos la matriz
			vector<int> ifila;

			for (int j = 0; j < n; j++) {
				int num;
				cin >> num;
				ifila.push_back(num);
			}

			tablero.push_back(ifila);
		}

		vector<vector<int>> tablero2 = tablero;
		vector<vector<int>> listAdy = mediaListAdy(tablero, tablero2);
		terminarListaAdy(tablero2, listAdy);

		// Hardcodeamos una matriz "capacidad", llena de unos
		int nlist = listAdy.size();
		vector<vector<int>> capacidad(nlist,vector<int>(nlist,0));
		for (int i = 0; i < nlist; i++) {
			for (int elem : listAdy[i]) {
				if (i < elem) capacidad[i][elem] = 1;
			}
		}

		cout << maxflow(0, nlist-1, listAdy, capacidad) << endl;

	}
}
