#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

void dfs(int nodo, int tiempo, vector<vector<int>> &listasAdy, vector<vector<int>> &T, vector<int> &TIN, vector<vector<int>> &back_edges_que_apuntan_a, int padre = -1, pair<int,int> invalida = make_pair(-1,-1)) {
    TIN[nodo] = tiempo;
    for (int hijo : listasAdy[nodo]) {
        if (make_pair(nodo,hijo) == invalida || make_pair(hijo,nodo) == invalida) continue;
        if (TIN[hijo] == -1) {      // Si no está visitado
            T[nodo].push_back(hijo);
            dfs(hijo, tiempo + 1, listasAdy, T, TIN, back_edges_que_apuntan_a, nodo, invalida);   //padre es nodo
        }
        else if (TIN[hijo] < TIN[nodo] && padre!=hijo) {   // Si está visitado, es una back edge
            back_edges_que_apuntan_a[hijo].push_back(nodo); // (nodo <-> hijo) es la back edge
            back_edges_que_apuntan_a[nodo].push_back(hijo);
        }
    }
}

void low(int nodo, vector<vector<int>> &T, vector<int> &TIN, vector<int> &LOW, vector<vector<int>> &back_edges_que_apuntan_a, pair<int,int> invalida = make_pair(-1,-1)) {
    // Low[nodo] := min {tin[nodo], tin[p], low[hijos]}, con p una back edge que apunta a nodo.
    int res = TIN[nodo];
    for (int p : back_edges_que_apuntan_a[nodo]) {
        if (make_pair(nodo,p) == invalida || make_pair(p,nodo) == invalida) continue;
        res = min(res, TIN[p]);
    }
    for (int hijo : T[nodo]) {
        if (make_pair(nodo,hijo) == invalida || make_pair(hijo,nodo) == invalida) continue;
        low(hijo, T, TIN, LOW, back_edges_que_apuntan_a);
        res = min(res, LOW[hijo]);
    }
    LOW[nodo] = res;
}

void recorrer(int nodo, vector<vector<int>> &T, vector<int> &TIN, vector<int> &LOW, set<pair<int,int>> &resultado, pair<int,int> invalida = make_pair(-1,-1)){
    for (int hijo : T[nodo]) {
        recorrer(hijo, T, TIN, LOW, resultado, invalida);
        if (TIN[nodo] < LOW[hijo]) {
            pair<int,int> puente;
            if (hijo < nodo) puente = make_pair(hijo, nodo);
            else puente = make_pair(nodo, hijo);
            resultado.insert(puente);
        }
    }
}

void resolver(int n, int m, vector<vector<int>> &listasAdy, vector<pair<int, int>> &aristas) {
    // Llamamos dfs con raíz 0.
    // Computamos el time of entry into node (TIN).
    // Luego calculamos el lowest time of entry into node (LOW) para cada nodo.
    // El LOW de un nodo es el mínimo de los TIN de sus hijos y de sus backedges.
    vector<int> vacio;
    set<pair<int,int>> resultado;
    for (pair<int,int> invalida : aristas) {
        vector<vector<int>> T(n,vacio);     // Para el árbol dfs
        vector<int> TIN(n, -1);             // Notemos que nos sirve de noción de: visitado[v] <=> TIN[v] != -1
        vector<vector<int>> back_edges_que_apuntan_a(n,vacio);

        dfs(0, 0, listasAdy, T, TIN, back_edges_que_apuntan_a,-1,invalida);

        vector<int> LOW(n, -1);
        low(0, T, TIN, LOW, back_edges_que_apuntan_a,invalida);
        // Es puente <=> TIN[v] < LOW[v]
        recorrer(0, T, TIN, LOW, resultado,invalida);
    }

    cout << resultado.size() << endl;
    for (pair<int,int> arista : resultado) {
        cout << arista.first << " " << arista.second << endl;
    }
}


int main() {
    int tests;      // Cantidad de tests
    cin >> tests;

    while(tests--) {
        int n;
        int m;
        cin >> n >> m;
        vector<int> vacio;
        vector<vector<int>> listasAdy(n, vacio);
        vector<pair<int, int>> aristas;

        for (int i = 0; i < m; i++) {
            int a;
            int b;
            cin >> a >> b;

            aristas.push_back(make_pair(a, b));
            listasAdy[a].push_back(b);
            listasAdy[b].push_back(a);

        }

        resolver(n,m,listasAdy, aristas);
    }
}