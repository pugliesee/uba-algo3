#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

using namespace std;

int INF = 10e7;
int maxHerramientas;

//edmonds y karp de la catedra

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

    while ((new_flow = bfs(s, t, parent,listAdy,capacidad)) > 0) {
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



void resolver(int n, int m, int chabones, vector<vector<int>> &listAdy, vector<vector<int>> &capacidades) {
    // Hacemos busqueda bin de la cant de gente que es flujo max
    int low = 0;
    int max = maxHerramientas; //maxHerramientas
    int flujo = 0;
    int maxactual= 0;

    vector<int> vacio(n,0);
    vector<vector<int>> capacidadesCopy(n, vacio);

    while (low + 1 < max) {

        int mid = (max + low) / 2;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                capacidadesCopy[i][j] = capacidades[i][j] / mid;
            }
        }

        flujo = maxflow(0, n-1, listAdy, capacidadesCopy);
        if (flujo < chabones) max = mid;
        else low = mid;

    }

    if (max == 0) {
        cout << 0 << endl;
        return;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            capacidadesCopy[i][j] = capacidades[i][j] / max;   // Dividimos por high porque low < high (y quizás low == 0)
        }
    }

    flujo = maxflow(0, n-1, listAdy, capacidadesCopy);
    int res;
    (flujo < chabones) ? res = low : res = max;

    cout << res * chabones << endl;

}


int main() {
    int tests;      // Cantidad de tests
    cin >> tests;

    while(tests--) {

        maxHerramientas = 0;        // (Re)seteamos

        int n, m, chabones;
        cin >> n >> m >> chabones;     // Esquinas, calles, trabajadores

        vector<int> vacio;
        vector<vector<int>> listAdy(n,vacio);
        vector<vector<int>> capacidades(n, vector<int>(n, 0));  // Matriz de n x n para las capacidades

        for (int i = 0; i < m; i++) {

            int v, w, c;
            cin >> v >> w >> c;

            if (v == 1) maxHerramientas += c;
            listAdy[v-1].push_back(w-1);    // Completamos lista de adyacencia y capacidades
            capacidades[v-1][w-1] = c; //c es la capacidad de la calle v->w, a priori las calles son dirigidas

        }

        maxHerramientas /= chabones;
        resolver(n, m, chabones, listAdy, capacidades);

    }
}