#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <queue>

using namespace std;

enum Trayecto{IDA, VUELTA};

void bfs2(int t0, int inicio,int final , vector<vector<int>> &listaAdj, vector<int> &distancias, vector<int> &vertices){
    queue<int> cola;
    cola.push(inicio);
    distancias[inicio] = t0;
    while(!cola.empty()){
        int nodoActual = cola.front();
        cola.pop();
        for (int nodo : listaAdj[nodoActual]){
            if(distancias[nodo] == -1) { // No está visitado
                int tiempoActual = distancias[nodoActual] + 1;
                if (not(vertices[nodo] > 0 && tiempoActual >= vertices[nodo])) {
                    distancias[nodo] = tiempoActual;
                    if (nodo == final) return;  // Ya lo tenemos
                    cola.push(nodo);
                }
            }
        }
    }

}

void resolver(vector<vector<int>> &listaAdy, vector<int> &vertices, int hospital, int paciente) {
    // BFS IDA --> Del hospital hacia el paciente
    int largo = listaAdy.size();
    vector<int> distancias(largo,-1);
    bfs2(0, hospital, paciente, listaAdy, distancias, vertices);

    int tiempoIDA = distancias[paciente];
    if(tiempoIDA == -1) {
        cout << "IMPOSIBLE" << endl;
        return;
    }
    // BFS VUELTA --> Del paciente hacia el hospital
    vector<int> distancias2(largo,-1);
    bfs2(tiempoIDA, paciente, hospital, listaAdy, distancias2, vertices);
    int tiempoVUELTA = distancias2[hospital];
    if(tiempoVUELTA == -1) {
        cout << "IMPOSIBLE" << endl;
        return;
    }
    cout << tiempoIDA << " " << tiempoVUELTA << endl;
}


int main() {
    ifstream fin("2ej_input.txt");
    if (!fin.is_open()) {
        cerr << "Error: could not open input file 2ej_input.txt" << endl;
        return 1;
    }
    int tests;      // Cantidad de tests
    fin >> tests;


    while(tests--) {
        int n; //filas
        int m; //columnas
        fin >> n >> m;

        vector<int> vertices;
        for (int i = 0; i < n*m; i++){
            int a;
            fin >> a;
            vertices.push_back(a);
        }

        vector<int> vacio;
        vector<vector<int>> vecinos(n*m, vacio);

        //armamos la lista de adyacencia guardando en ella

        // ABAJO
        for(int filas = 0; filas < n-1; filas++) {
            for(int nodo = 0; nodo < m; nodo ++) {
                int indice = filas*m + nodo;
                int abajo = (filas + 1)*m + nodo;
                vecinos[indice].push_back(abajo);
            }
        }
        // ARRIBA
        for(int filas = 1; filas < n; filas++) {
            for(int nodo = 0; nodo < m; nodo ++) {
                int indice = filas*m + nodo;
                int arriba = (filas - 1)*m + nodo;
                vecinos[indice].push_back(arriba);
            }
        }
        // DERECHA
        for(int columnas = 0; columnas < m-1; columnas++) {
            for(int nodo = 0; nodo < n; nodo ++) {
                int indice = nodo*m + columnas;
                int derecha = nodo * m + columnas + 1;
                vecinos[indice].push_back(derecha);
            }
        }
        // IZQUIERDA
        for(int columnas = 1; columnas < m; columnas++) {
            for(int nodo = 0; nodo < n; nodo ++) {
                int indice = nodo*m + columnas;
                int izquierda = m * nodo + columnas - 1;
                vecinos[indice].push_back(izquierda);
            }
        }


        int hospital_fila;
        int hospital_columna;
        int paciente_fila;
        int paciente_columna;
        fin >> hospital_fila >> hospital_columna >> paciente_fila >> paciente_columna;

        int hospital = hospital_fila*m + hospital_columna;
        int paciente = paciente_fila * m + paciente_columna;

        resolver(vecinos,vertices,hospital, paciente);

    }
}