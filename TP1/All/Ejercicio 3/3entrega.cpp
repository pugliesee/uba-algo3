#include <iostream>
#include <vector>

using namespace std;

enum Operacion{IZQ, DER};

int contadorSoluciones = 0;
vector<char> solucion;

bool def(vector<vector<vector<pair<int,int>>>> &m, int i, int saldoParcial){
	if (saldoParcial < 0) {
		return (m[1][i][abs(saldoParcial)].first != -1 && m[1][i][abs(saldoParcial)].second != -1);
	} else {
		return (m[0][i][saldoParcial].first != -1 && m[0][i][saldoParcial].second != -1);
	}
}

void f(vector<int> &saldos, int saldoParcial, int i, vector<vector<vector<pair<int,int>>>> &m, Operacion anterior) {

	if (i == 0 && saldoParcial == 0) {
		// ESTAMOS EN UN CAMINO VÁLIDO
		if (anterior == IZQ) {
			m[0][0][0].first = 1;
		} else {
			m[0][0][0].second = 1;
		}
		return;
	}

	bool tabla = (saldoParcial < 0);
	// Chequeamos si lo tenemos en la matriz
	if (def(m, i, saldoParcial)) {

		return;

	} else {
		// Recursiones
		int saldoSumar = saldoParcial - saldos[i - 1];    // Contraintuitivo pero está al revés nuestra recursión.
		int saldoRestar = saldoParcial + saldos[i - 1];

		f(saldos, saldoSumar, i - 1, m, DER);
		f(saldos, saldoRestar, i - 1, m, IZQ);

		// está definido el i-1
		bool tabla_f1 = saldoSumar < 0;
		bool tabla_f2 = saldoRestar < 0;

		if (m[tabla_f1][i - 1][abs(saldoSumar)] != make_pair(0, 0)) {
			m[tabla][i][abs(saldoParcial)].first = 1;
		} else {
			m[tabla][i][abs(saldoParcial)].first = 0;
		}

		if (m[tabla_f2][i - 1][abs(saldoRestar)] != make_pair(0, 0)) {
			m[tabla][i][abs(saldoParcial)].second = 1;
		} else {
			m[tabla][i][abs(saldoParcial)].second = 0;
		}

	}
}

void resolver(vector<int> &saldos, int saldoTotal, vector<vector<vector<pair<int,int>>>> &m) {
	int N = saldos.size();

	f(saldos, saldoTotal, N, m, IZQ);      // Computamos la matriz M (relleno como naruto a lo loco).

	vector<char> solucion2;
	for (int i = 1; i < m[0].size(); i++) {

		pair<int, int> signo = make_pair(0, 0);
		for (int j = 0; j < m[0][0].size(); j++) {
			if (m[0][i][j].first == 1 || m[1][i][j].first == 1) signo.first = 1;
			if (m[0][i][j].second == 1 || m[1][i][j].second == 1) signo.second = 1;
			if (signo.first == 1 && signo.second == 1) break;
		}

		if (signo == make_pair(1, 1)) solucion2.push_back('?');
		else if (signo == make_pair(1, 0)) solucion2.push_back('+');
		else if (signo == make_pair(0, 1)) solucion2.push_back('-');

	}

	for (int i = 0; i < solucion2.size(); i++) {
		cout << solucion2[i];
	}
	cout << endl;
}

int main(int argc, char* argv[]) {

	int tests;      // Cantidad de tests
	cin >> tests;

	while (tests--){
		int N;
		cin >> N;
		int saldoTotal;
		cin >> saldoTotal;
		saldoTotal /= 100;

		vector<int> saldos;
		int suma = abs(saldoTotal);
		for (int i = 0; i < N; i++) {
			int valor;
			cin >> valor;
			valor /= 100;
			saldos.push_back(valor);
			suma += valor;
		}

		if (suma == 0){     // Caso más borde no hay
			while(N--){
				cout << '?';
			}
			cout << endl;
			continue;
		}
		vector<vector<pair<int,int>>> m_pos(N+1, vector<pair<int, int>>(suma+1, make_pair(-1, -1)));

		vector<vector<vector<pair<int,int>>>> m;
		m_pos[0] = vector<pair<int,int>>(suma+1, make_pair(0,0));
		m_pos[0][0] = make_pair(-1,-1);     //TODO: ver si conviene poner doble positivo
		m.push_back(m_pos); // Tabla de positivos
		m.push_back(m_pos); // Tabla de negativos
		solucion = vector<char>(N,'&');

		resolver(saldos, saldoTotal, m);
		contadorSoluciones = 0;
		solucion = {};
	}

	return 0;
}
