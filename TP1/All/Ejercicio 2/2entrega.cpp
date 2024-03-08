#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
// GLOBALES

bool comparacion(pair<int, int> A, pair<int, int> B){
    if (A.first == 0) return true;
    if (B.first == 0) return false;

    float coefA = float(A.second) / float(A.first);
    float coefB = float(B.second) / float(B.first);

    return coefA >= coefB;
}

vector<pair<int, int>> transformaPair(vector<int> &M, vector<int> &C) {
    int N = M.size();
    vector<pair<int, int>> res;
    for (int i = 0; i < N; i++) {
        res.push_back(make_pair(M[i], C[i]));
    }
    return res;
}

//SOLUCIÓN ALGORITMO GREEDY, ordena los coeficientes de descontento de mayor a menor y los atiended primero a los de
//mayor decontento sumando su descontento al total acumulado

void swap(vector<pair<int,int>> &MyC, int i, int j){
    pair<int,int> aux = MyC[i];
    MyC[i] = MyC[j];
    MyC[j] = aux;
}


void f_prima(vector<pair<int, int>> &MyC) {
    //SORT
    int N = MyC.size();
    // sort(MyC.begin(), MyC.end(), comparacion);
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N - i - 1; ++j) {
            if (!comparacion(MyC[j], MyC[j + 1])) {
                swap(MyC, j, j + 1);
            }
        }
    }
    /*
    for (int i = 0; i < N; i++){
        cout <<"(" << MyC[i].first << ", " << MyC[i].second << ")" << endl;
    }
    */
    long int minutosPasados = 0;
    long int descontentoTotal = 0;

    for (int i = 0; i < N ; i++) {
        descontentoTotal += (minutosPasados + MyC[i].first) * MyC[i].second;
        minutosPasados += MyC[i].first;
    }

    cout<< (descontentoTotal % 1000000007) <<endl;
}

int main() {
    int tests;      // Cantidad de tests
    cin >> tests;

    while (tests--) {
        int N;
        cin >> N;
        vector<int> M;
        vector<int> C;
        for (int i = 0; i < N; i++) {
            int valor;
            cin >> valor;
            M.push_back(valor);
        }
        for (int i = 0; i < N; i++) {
            int valor;
            cin >> valor;
            C.push_back(valor);
        }
        vector<pair<int, int>> MyC = transformaPair(M, C);
        f_prima(MyC);
    }

    return 0;
}
