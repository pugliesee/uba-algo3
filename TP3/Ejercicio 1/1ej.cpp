#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include<algorithm>
#include <queue>

using namespace std;

int INF = (int)2e31;




void dijkstra(vector<vector<pair<int, int>>> &listaAdy, vector<bool> &visitados, vector<int> &distancias, int n, int s = 0) {
    queue<pair<int,int>> q;
    q.emplace(make_pair(0, s));

    while (q.size() != 0) {
        pair<int,int> arista = q.front();
        q.pop();
        
        int index = arista.second;
        int minimo = arista.first;
        int index = arista.second;
        visitados[index] = true;

        for(auto edge : listaAdy[index]){

            if (distancias[index] < minimo) continue;
            int new_dist = distancias[index] + edge.first;
            if (new_dist < distancias[edge.second]) {
                distancias[edge.second] = new_dist;
                q.emplace(make_pair(new_dist, edge.second));
            }
        }
    }
}


int main() {
    ifstream fin("./1ej_input.txt");
    if (!fin.is_open()) {
        cerr << "Error: could not open input file 1ej_input.txt" << endl;
        return 1;
    }
    int tests;      // Cantidad de tests
    fin >> tests;

    while(tests--) {
        int n, tuneles;
        fin >> n >> tuneles;

        vector<pair<int,int>> vacio;
        vector<vector<pair<int,int>>> listAdy(4*n,vacio);
        // Creamos las aristas triviales
        for (int i = 0; i < n-1; i++) {
            listAdy[i      ].push_back(make_pair(1,i +       1));   // Nivel 1
            listAdy[i +   n].push_back(make_pair(1,i +   n + 1));   // Nivel 2
            listAdy[i + 2*n].push_back(make_pair(1,i + 2*n + 1));   // Nivel 3
            listAdy[i + 3*n].push_back(make_pair(1,i + 3*n + 1));   // Nivel 4
        }

        while (tuneles--) {
            int empieza, termina;
            fin >> empieza >> termina;
            empieza--;
            termina--;
            listAdy[empieza      ].push_back(make_pair(2,termina +   n));   // Del 0 al 1
            listAdy[empieza +   n].push_back(make_pair(2,termina + 2*n));   // Del 1 al 2
            listAdy[empieza + 2*n].push_back(make_pair(2,termina + 3*n));   // Del 2 al 3
        }

        // Aristas de costo 0 para que el camino mínimo sea a un único nodo.
        listAdy[  n - 1].push_back(make_pair(0,4*n-1));
        listAdy[2*n - 1].push_back(make_pair(0,4*n-1));
        listAdy[3*n - 1].push_back(make_pair(0,4*n-1));

        vector<int> distancias(4*n, INF);
        distancias[0] = 0;
        vector<bool> visitados(4*n, false);

        dijkstra(listAdy, n, visitados, distancias);
    }
}