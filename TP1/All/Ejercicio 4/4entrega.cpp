#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

long int INF = 1000000000;




// Ya no tira el más cercano. NO USAR PARA CALCULAR EL COSTO.
int busqBinIndex(int puestoMedio, vector<int> &puestos, int low, int high) {
	if (low == high) return puestos[low];
	if (low + 1 == high) {
		if (puestos[high] == puestoMedio) return high;
		return low;
	}

	int medio = (low + high) / 2;
	if (puestos[medio] == puestoMedio) return medio;
	if (puestos[medio] > puestoMedio) return busqBinIndex(puestoMedio, puestos, low, medio);
	return busqBinIndex(puestoMedio, puestos, medio, high);
}

void f(vector<int> &puestos, vector<vector<pair<int, int>>> &memo, int K, int i) {
	int N = puestos.size();

	if (i > N-K){
		memo[K][i] = make_pair(INF,-1);
		return;
	}

	if (memo[K][i] != make_pair(-1,-1)) return;

	int minimo = INF;

	for (int j = i+1; j < N; j++) {
		f(puestos, memo, K - 1, j);
		pair<int, int> value = memo[K - 1][j];

		// Tenemos una proveeduría en index I
		// ponemos una en index J
		int costo = value.first;
		int puestoMedio = (puestos[i] + puestos[j]) / 2;

		int indexMid = busqBinIndex(puestoMedio, puestos, i, j);
		int suma = 0;
		int cantidad = 0;
		for (int l = i+1; l <= indexMid; l++) {
			suma += puestos[l];
			cantidad++;
		}
		costo += abs(suma - cantidad * puestos[i]);

		suma = 0;
		cantidad = 0;
		for (int l = indexMid + 1; l < j; l++) {
			suma += puestos[l];
			cantidad++;
		}
		costo += abs(cantidad * puestos[j] - suma);     // miedo al negativo (?

		if (costo < minimo) {
			minimo = costo;
			memo[K][i] = make_pair(minimo, j);
		}

	}

}

void resolver(vector<int> &puestos, vector<vector<pair<int,int>>> &memo) {
	int N = puestos.size();
	int K = memo.size() - 1;

	// Inicializamos la primer fila K = 0.
	int suma = 0;
	int elementos = 0;
	for (int i = N-1; i >= K-1; i--) {
		memo[0][i] = make_pair(suma - puestos[i] * elementos, -1);
		suma += puestos[i];
		elementos++;
	}

	for (int i = 0; i < N; i++){
		f(puestos,memo,K-1,i);
	}

	int minimo = INF;
	for (int j = 0; j < N; j++) {
		pair<int, int> value = memo[K - 1][j];
		int costo = value.first;

		int suma = 0;
		int cantidad = 0;
		for (int l = 0; l < j; l++) {
			suma += puestos[l];
			cantidad++;
		}
		costo += abs(cantidad * puestos[j] - suma);     // miedo al negativo (?

		if (costo < minimo) {
			minimo = costo;
			memo[K][0] = make_pair(minimo, j);
		}
	}
	cout << minimo << endl;

	int index = memo[K][0].second;
	vector<int> res = {};
	for (int i = 0; i < K; i++) {
		res.push_back(puestos[index]);
		index = memo[K-i-1][index].second;
	}

	for (int i = 0; i < res.size(); i++) {
		cout << res[i] << " ";
	}
	cout << endl;

}


int main() {
	int tests;      // Cantidad de tests
	cin >> tests;

	while (tests--){
		int N; int K;
		cin >> N;   // # Puestos
		cin >> K;   // # Proveedurías

		vector<int> puestos;
		for (int i = 0; i < N; i++){
			int valor;
			cin >> valor;
			puestos.push_back(valor);
		}
		vector<vector<pair<int,int>>> memo(K+1,vector<pair<int,int>> (N,make_pair(-1,-1)));

		resolver(puestos, memo);
		memo = {};

	};

	return 0;
}
