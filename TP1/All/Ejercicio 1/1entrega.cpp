#include <iostream>
#include <vector>

using namespace std;

enum Posicion { ARR, ABA, IZQ, DER};
// GLOBALES
int INF = 100000;
vector<vector<bool>> visitadas = {};
bool hay_sol = false;

//  (min, max)
pair<int, int> f(vector<vector<char>> &tablero, int i, int j, Posicion anterior){
    int n = tablero.size();
    int m = tablero[0].size();

    // Está en la esquina == WIN :)
    if (i == n-1 && j == m-1){
        char letra = tablero[i][j];
        if (letra == '#'){
            return make_pair(INF,-INF);
        }
        hay_sol = true;
        return make_pair(0,0);
    }

    // FILTRADO ↓
    // No está en rango
    if (i < 0 || i >= n || j < 0 || j >= m) return make_pair(INF,-INF);
    // Forma parte del camino
    if (visitadas[i][j]) return make_pair(INF,-INF);
    // Fin de FILTRADO ↑

    visitadas[i][j] = true;

    char letra = tablero[i][j];
    pair<int,int> resultado;
    // Espacio para las posibles recursiones
    pair<int,int> sig1;
    pair<int,int> sig2;
    pair<int,int> sig3;
    switch(letra){
        case '#':
            resultado = make_pair(INF,-INF);
            break;
        case 'I':   // Un llamado recursivo
            switch(anterior){
                case ARR:
                    //vamos para abajo
                    sig1 = f(tablero,i+1,j,ARR);
                    break;
                case ABA:
                    //vamos para arriba
                    sig1 = f(tablero,i-1,j,ABA);
                    break;
                case IZQ:
                    //vamos para la derecha
                    sig1 = f(tablero,i,j+1,IZQ);
                    break;
                case DER:
                    //vamos para la izquierda
                    sig1 = f(tablero,i,j-1,DER);
                    break;
            }
            resultado = make_pair(sig1.first +1 , sig1.second + 1);
            break;
        case 'L':   // Dos llamados recursivos
            switch(anterior){
                case ARR:
                    //vamos para la izquierda
                    sig1 = f(tablero,i, j-1, DER);
                    //vamos para la derecha
                    sig2 = f(tablero,i, j+1, IZQ);
                    break;
                case ABA:
                    //vamos para la izquierda
                    sig1 = f(tablero,i, j-1, DER);
                    //vamos para la derecha
                    sig2 = f(tablero,i, j+1, IZQ);
                    break;
                case IZQ:
                    //vamos para arriba
                    sig1 = f(tablero,i-1, j, ABA);
                    //vamos para abajo
                    sig2 = f(tablero,i+1, j, ARR);
                    break;
                case DER:
                    //vamos para arriba
                    sig1 = f(tablero,i-1, j, ABA);
                    //vamos para abajo
                    sig2 = f(tablero,i+1, j, ARR);
                    break;
            }
            resultado = make_pair(min(sig1.first, sig2.first)+1, max(sig1.second, sig2.second)+1);
            break;
        case '+':   // Tres llamados recursivos
            switch(anterior) {
                case ARR:
                    //vamos para la izquierda
                    sig1 = f(tablero, i, j - 1, DER);
                    //vamos para la derecha
                    sig2 = f(tablero, i, j + 1, IZQ);
                    //vamos para abajo
                    sig3 = f(tablero, i + 1, j, ARR);
                    break;
                case ABA:
                    //vamos para la izquierda
                    sig1 = f(tablero, i, j - 1, DER);
                    //vamos para la derecha
                    sig2 = f(tablero, i, j + 1, IZQ);
                    //vamos para arriba
                    sig3 = f(tablero, i - 1, j, ABA);
                    break;
                case IZQ:
                    //vamos para la derecha
                    sig1 = f(tablero, i, j + 1, IZQ);
                    //vamos para abajo
                    sig2 = f(tablero, i + 1, j, ARR);
                    //vamos para arriba
                    sig3 = f(tablero, i - 1, j, ABA);
                    break;
                case DER:
                    //vamos para la izquierda
                    sig1 = f(tablero, i, j - 1, DER);
                    //vamos para abajo
                    sig2 = f(tablero, i + 1, j, ARR);
                    //vamos para arriba
                    sig3 = f(tablero, i - 1, j, ABA);
                    break;
            }
            resultado = make_pair(min(min(sig1.first, sig2.first),sig3.first)+1, max(max(sig1.second, sig2.second), sig3.second)+1);
            break;
        default:
            cerr<<"Tablero inválido."<<endl;
            resultado = make_pair(INF,-INF);
            break;
    }

    visitadas[i][j] = false;
    return resultado;
}

void resolver(vector<vector<char>> &tablero, int n, int m){
    // Llamamos a la función
    pair<int,int> res1 = f(tablero,0,0,IZQ);    // Ficticiamente de la izq
    pair<int,int> res2 = f(tablero,0,0,ARR);    // Ficticiamente de arriba
    pair<int,int> res_total = make_pair(min(res1.first, res2.first), max(res1.second, res2.second));

    // Imprimimos si es posible o no con su rta
    cout<< (hay_sol ? "POSIBLE " : "IMPOSIBLE");
    if (hay_sol) cout<< res_total.first << " " << res_total.second;
    cout<<endl;
}

int main() {
    int tests;      // Cantidad de tests
    cin >> tests;

    while (tests > 0){
        int nrows, ncols;
        cin >> nrows >> ncols;

        vector<vector<char>> tablero;
        // Completo la matriz:
        for (int i = 0; i < nrows; i++) {
            vector<char> fila;
            vector<bool> filaVisitadas;
            for (int j = 0; j < ncols; j++) {
                char valor;
                cin >> valor;
                fila.push_back(valor);
                filaVisitadas.push_back(false);
            }
            tablero.push_back(fila);
            visitadas.push_back(filaVisitadas);
        }
        // Llamamos a la función
        resolver(tablero,nrows,ncols);
        // Limpiamos variables globales
        visitadas = {};
        hay_sol = false;
        tests--;
    }

    return 0;
}
