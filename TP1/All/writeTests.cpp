#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    string file = "tests_ej2.txt";
    vector<int> ks = {2, 20, 50, 1000, 10000, 100000}; // para los randoms
    int N = ks.size();

    cout << "Generando tests random:" << endl;

    vector<vector<pair<int,int>>> tests;
    // genero los vectores random
    for (int k : ks){
        vector<pair<int,int>> test;
        for (int i = 0; i < k; i++) {
            test.push_back(make_pair(rand() % 100000, rand() % 100000));
        }
        tests.push_back(test);
    }

    ofstream fout(file);

    if (!fout.is_open()) {
        cerr << "Error opening file"<< file << endl;
        return 1;
    }

    fout << N << endl;

    for (int i = 0; i < N; i++) {
        int n = ks[i];
        fout << n << endl;

        for (int j = 0; j < n-1; j++) {
            fout << tests[i][j].first << " ";
        }
        fout << tests[i][n-1].first;
        fout << endl;

        for (int j = 0; j < n-1; j++) {
            fout << tests[i][j].second << " ";
        }
        fout << tests[i][n-1].second;
        fout << endl;
    }

    fout.close();

    cout << "Generando tests bordes:" << endl;
    string file2 = "tests_ej2_bordes.txt";
    vector<vector<pair<int,int>>> tests2;
    vector<int> ks2 = {5, 8, 13, 21, 34, 55}; // para los bordes con coeficiente
    vector<int> ks3 = {2, 3, 5, 8, 13}; // para los bordes con 0's
    int N2 = ks2.size() + ks3.size();

    // genero los vectores bordes
    for (int k : ks2){
        vector<pair<int,int>> test;
        for (int i = 0; i < k; i++) {
            int a = rand() % 100000;
            int b = rand() % 100000;
            if (i % 3 == 0) b = 2*a;        // fuerzo a que haya un caso en el que el coeficiente sea 2
            test.push_back(make_pair(a,b));
        }
        tests2.push_back(test);
    }
    for (int k : ks3){
        vector<pair<int,int>> test;
        for (int i = 0; i < k; i++) {
            int a = rand() % 100000;
            int b = rand() % 100000;
            if (i % 3 == 0) b = 0;        // fuerzo a que haya un caso en el que el coeficiente sea 0
            if (i % 2 == 0) {
                test.push_back(make_pair(a,b));
            }
            else{
                test.push_back(make_pair(b,a));
            }
        }
        tests2.push_back(test);
    }

    ofstream fout2(file2);

    if (!fout2.is_open()) {
        cerr << "Error opening file"<< file2 << endl;
        return 1;
    }

    fout2 << N2 << endl;

    for (int i = 0; i < N2; i++) {
        int n = tests2[i].size();
        fout2 << n << endl;
        for (int j = 0; j < n-1; j++) {
            fout2 << tests2[i][j].first << " ";
        }
        fout2 << tests2[i][n-1].first;
        fout2 << endl;
        for (int j = 0; j < n-1; j++) {
            fout2 << tests2[i][j].second << " ";
        }
        fout2 << tests2[i][n-1].second;
        fout2 << endl;
    }


    return 0;
}