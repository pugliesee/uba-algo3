#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

long int INF = 1000000000;
vector<int> solGlobal;
int globalCosto = INF;


int busqBin(int puestoMedio, vector<int> &puestos, int low, int high) {
	if (low == high) return puestos[low];
	if (low + 1 == high) {
		if (puestos[low] == puestoMedio) return puestos[low];
		if (puestos[high] == puestoMedio) return puestos[high];
		if (abs(puestos[low] - puestoMedio) <= abs(puestos[high] - puestoMedio)) return puestos[low];
		return puestos[high];
	}

	int medio = (low + high) / 2;
	if (puestos[medio] == puestoMedio) return puestos[medio];
	if (puestos[medio] > puestoMedio) return busqBin(puestoMedio, puestos, low, medio);
	return busqBin(puestoMedio, puestos, medio, high);

}


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


//el i y el j es donde tenia la solucion anterior una proveeduria
int busqBinX(vector<int> &puestos, int puesto1, int puesto2){
	int puestoMedio = (puesto1 + puesto2)/2;
	return busqBin(puestoMedio, puestos, 0, puestos.size()-1);
}

int busqBinXIndex(vector<int> &puestos, int puesto1, int puesto2){
	int puestoMedio = (puesto1 + puesto2)/2;
	return busqBinIndex(puestoMedio, puestos, 0, puestos.size()-1);
}


int costoConBinaria(vector<int> &puestos, vector<int> &solucion) {
	int costo = 0;
	for (int puesto : puestos){
		int valor = busqBin(puesto, solucion, 0, solucion.size()-1);
		costo += abs(valor - puesto);
	}
	return costo;
}


int choripanes_backtracking(int i, int K, vector<int> &puestos, vector<int> &solucion){
	int N = puestos.size();
	if (K == 0) return costoConBinaria(puestos, solucion);
	if (i < 0) return INF;
	if (K > i+1) return INF;        // Cota factibilidad

	//Si lo que voy a pusehar a la slucion es mas grande que lo que tengo en la global
	//corto porque no va a ser menor lexicograficamente
	// el tema es que tiene que estar definido solGlobal
	//if(solGlobal[i] < puestos[i]) return INF;

	//solucion.push_back(puestos[i]);
	solucion[K-1] = puestos[i];     // O(1) en vez de O(n) por el push_back
	int resAgregado = choripanes_backtracking(i-1, K-1, puestos, solucion);
	//solucion.pop_back();
	int resSinAgregar = choripanes_backtracking(i-1, K, puestos, solucion);

	return min(resAgregado, resSinAgregar);
}
/*
vector<int> f(vector<int> &solucion, vector<int> &puestos){
	vector<int> solucion2(solucion);
	int last = solucion2[solucion.size()-1];
	solucion2.pop_back();
	vector<int> res(solucion2);
	int resCosto = INF;
	for (int i = solucion.size()-1; i > 0; i--){
		int a = solucion[i-1];
		int b = solucion[i];
		int midIndex = busqBinXIndex(puestos, a, b);
		float midValue = float(a+b)/2;

		if (midValue - puestos[midIndex] == puestos[midIndex+1] - midValue){     // está en el medio de 2 puestos
			solucion2[i-1] = puestos[midIndex+1];
			int costo = costoConBinaria(puestos, solucion2);
			if (costo < resCosto){
				res = vector<int>(solucion2);
				resCosto = costo;
			}
			else if (costo == resCosto) {
				bool menor = false;
				for (int j = 0; j < solucion2.size(); j++) {
					if (solucion2[j] < res[j]) {
						menor = true;
						break;
					}
					if (solucion2[j] > res[j]) {
						menor = false;
						break;
					}
				}
				if (menor) {
					res = vector<int>(solucion2);
					resCosto = costo;
				}
			}
		}

		solucion2[i-1] = puestos[midIndex];
		int costo = costoConBinaria(puestos, solucion2);
		if (costo < resCosto){
			res = vector<int>(solucion2);
			resCosto = costo;
		}
		else if (costo == resCosto) {
			bool menor = false;
			for (int j = 0; j < solucion2.size(); j++) {
				if (solucion2[j] < res[j]) {
					menor = true;
					break;
				}
				if (solucion2[j] > res[j]) {
					menor = false;
					break;
				}
			}
			if (menor) {
				res = vector<int>(solucion2);
				resCosto = costo;
			}
		}
		solucion2[i-1] = last;
		last = a;
	}
	globalCosto = resCosto;
	return res;
}
*/

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


int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Uso: "<< argv[0] << " <archivo de entrada>" << endl;
		return 1;
	}
	const char* input_file = argv[1];

	ifstream fin(input_file);
	if (!fin.is_open()) {
		cerr << "Error: could not open input file " << input_file << endl;
		return 1;
	}

	int tests;      // Cantidad de tests
	fin >> tests;

	while (tests--){
		int N; int K;
		fin >> N;   // # Puestos
		fin >> K;   // # Proveedurías

		vector<int> puestos;
		for (int i = 0; i < N; i++){
			int valor;
			fin >> valor;
			puestos.push_back(valor);
		}
		vector<vector<pair<int,int>>> memo(K+1,vector<pair<int,int>> (N,make_pair(-1,-1)));

		resolver(puestos, memo);
		memo = {};

	};

	fin.close();

	return 0;
}
