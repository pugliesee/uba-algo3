#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

int INF = 10000000;

set<pair<int, int>> aristas;

void floydWarshall(int n, vector<vector<int>> &pesos) {
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				pesos[i][j] = min(pesos[i][j], pesos[i][k] + pesos[k][j]);
			}
		}
	}
}

void minimizarAristas(int n, vector<vector<int>> &matrizFW) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) {
			for (int k = 0; k < i; k++) {
				if (matrizFW[i][j] == matrizFW[i][k] + matrizFW[k][j] && k > j) {
					aristas.erase(make_pair(i, j));
					aristas.erase(make_pair(j, i));
				}
			}
		}
	}
}


void resolver(int n, vector<vector<int>> &matrizFW) {
	// Primero me fijo que haya solución

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) {
			for (int k = 1; k < i; k++) {

				if (matrizFW[i][j] > matrizFW[i][k] + matrizFW[k][j] && k > j) {
					cout << "IMPOSIBLE" << endl;
					return;
				}

			}
		}
	}

	cout << "POSIBLE" << endl;

	minimizarAristas(n, matrizFW);  // Quitamos todas las aristas necesarias del set que teníamos.

	// Me queda armar un grafo con aristas de peso 1. --> Así, podemos calcular Floyd-Warshall una vez y conseguir la matriz de distancias resultante.

	vector<int> infinito(n, INF);
	vector<vector<int>> pesos(n, infinito);

	for (auto arista : aristas) {
		pesos[arista.first][arista.second] = 1;
		pesos[arista.second][arista.first] = 1;
	}

	floydWarshall(n, pesos);

	for (int i = 0; i < n; i ++) {

		for (int j = 0; j < n; j++) {
			if (i == j) {
				cout << "0" << " ";
				continue;
			}
			cout << pesos[i][j] << " ";
		}

		cout << endl;
	}

}


int main() {
	int tests;      // Cantidad de tests
	cin >> tests;

	while(tests--) {

		int nodos;
		cin >> nodos;   // Cantidad de nodos

		aristas = {};

		// Construyo la matriz simétrica
		vector<int> ceros(nodos, 0);
		vector<vector<int>> matrizFW(nodos, ceros); // Matriz de n x n

		for (int i = 1; i < nodos; i++) {
			for (int j = 0; j < i; j++) {

				int elem;
				cin >> elem;    // Peso del camino mínimo desde i hasta j (y desde j hasta i)

				matrizFW[i][j] = elem;
				aristas.insert(make_pair(i, j));

			}
		}

		// Ya tenemos la matriz simétrica armada. Resolvemos.
		resolver(nodos, matrizFW);

	}
}
