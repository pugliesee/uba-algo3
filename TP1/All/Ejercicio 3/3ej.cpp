#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

enum Operacion{IZQ, DER};

int contadorSoluciones = 0;
vector<char> solucion;

/*void f(vector<int> &saldos, int saldoParcial, int i, vector<vector<vector<char>>> &m, Operacion anterior) {
	bool tabla = (saldoParcial >= 0) ? 0 : 1;
	cout << "Saldo parcial: "<< saldoParcial << endl;
	// CASO BASE
	if (i == -1) {
		if (saldoParcial == 0) {
			contadorSoluciones++;
			if (solucion[0] == '-'){
				if (anterior == SUMA) {
					solucion[0] = '?';
				}
			}
			else if (solucion[0] == '+'){
				if (anterior == RESTA) {
					solucion[0] = '?';
				}
			}
		}
		return;
	}

	// Chequear si lo tenemos en la matriz
	char letra = m[tabla][i][abs(saldoParcial)];
	if (letra != '&') {
		contadorSoluciones += (letra != 'X') ? 1 : 0;       // Sumamos la solución
		if ((letra == '+' && anterior == SUMA) || (letra == '-' && anterior == RESTA)) {
			solucion[i+1] = '?';
		}

		if (anterior == RESTA) {

			int saldoAnterior = saldoParcial - saldos[i + 1];
			if (saldoAnterior < 0) {
				// Lo busco en la tabla negativa
				char simbolo = m[1][i+1][abs(saldoAnterior)];
				if (simbolo != '&') {
					solucion[i+1] = '?';
				}
			} else {
				char simbolo = m[0][i+1][saldoAnterior];
				if (simbolo != '&') {
					solucion[i+1] = '?';
				}
			}

		} else {
			int saldoAnterior = saldoParcial + saldos[i + 1];

			if (saldoAnterior < 0) {
				// Lo busco en la tabla negativa
				char simbolo = m[1][i+1][abs(saldoAnterior)];
				if (simbolo != '&') {
					solucion[i+1] = '?';
				}
			} else {
				char simbolo = m[0][i+1][saldoAnterior];
				if (simbolo != '&') {
					solucion[i+1] = '?';
				}
			}

		}

	} else {
		// Llamados recursivos:

		int saldoSuma = saldoParcial - saldos[i];
		int saldoResta = saldoParcial + saldos[i];


		// CASO SUMAR SALDO
		int contadorAntes = contadorSoluciones;
		f(saldos, saldoSuma, i - 1, m, SUMA);

		// CASO RESTAR SALDO
		int contadorMedio = contadorSoluciones;
		f(saldos, saldoResta, i - 1, m, RESTA);

		// Comparamos si ponemos 'X', '+', '-' o '?'.
		if (contadorSoluciones == contadorAntes) m[tabla][i][abs(saldoParcial)] = 'X';
		else if (contadorSoluciones == contadorMedio) m[tabla][i][abs(saldoParcial)] = '+';
		else if (contadorMedio != contadorAntes) m[tabla][i][abs(saldoParcial)] = '?';
		else m[tabla][i][abs(saldoParcial)] = '-';

		if ((m[tabla][i][abs(saldoParcial)] != 'X') && solucion[i] == '&') {
			solucion[i] = m[tabla][i][abs(saldoParcial)];
		}
	}

}

void resolver(vector<int> &saldos, int saldoTotal, vector<vector<vector<char>>> &m) {
	// m de (n * suma_total)
	int N = saldos.size();

	f(saldos, saldoTotal, N-1, m, SUMA);

	cout << "Soluciones " << contadorSoluciones << endl;

	for (int i = 0; i < solucion.size(); i++) {
		cout << solucion[i];

	}
	cout << endl;

}*/

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
		if ((m[tabla][i][abs(saldoParcial)].first == 1 || m[tabla][i][abs(saldoParcial)].second == 1)) {
			// SI TENEMOS ALGÚN CAMINO VÁLIDO...
			/*
			if (anterior == IZQ) {
				m[tabla][i][abs(saldoParcial)].first = 1;
			} else {
				m[tabla][i][abs(saldoParcial)].second = 1;
			}
			*/
		}
		// En caso contrario, no tocamos la tabla
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
	//f(saldos, saldoTotal, N, m, DER);      // Computamos la matriz M (relleno como naruto a lo loco).

	/*
	cout << "Tablita + " << endl;

	for (int i = 0; i < m[0].size(); i++){
		cout << "FILA: " << i << endl;
		for (int j = 0; j < m[0][i].size(); j++){
			cout << "(" << m[0][i][j].first << ", " << m[0][i][j].second << ")\t";
		}
		cout << endl;
		cout << endl;
	}

	cout << "Tablita - " << endl;

	for (int i = 0; i < m[1].size(); i++) {
		cout << "FILA: " << i << endl;
		for (int j = 0; j < m[1][i].size(); j++){
			cout << "(" << m[1][i][j].first << ", " << m[1][i][j].second << ")\t";
		}
		cout << endl;
		cout << endl;
	}
	*/

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
		int N;
		fin >> N;
		int saldoTotal;
		fin >> saldoTotal;
		saldoTotal /= 100;


		vector<int> saldos;
		int suma = abs(saldoTotal);
		for (int i = 0; i < N; i++) {
			int valor;
			fin >> valor;
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

	fin.close();

	return 0;
}


