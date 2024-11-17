#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm> // Para std::min
#include <cctype>    // Para tolower

using namespace std;

// Tablas de costos globales
vector<int> cost_insert(26);           // Costos de inserción
vector<int> cost_delete(26);           // Costos de eliminación
vector<vector<int>> cost_replace(26, vector<int>(26)); // Matriz de costos de sustitución
vector<vector<int>> cost_transpose(26, vector<int>(26)); // Matriz de costos de transposición

// Funciones auxiliares para cargar costos desde archivos
void loadCostInsert(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de costos de inserción: " << filename << endl;
        exit(1);
    }
    for (int i = 0; i < 26; ++i) {
        file >> cost_insert[i];
    }
    file.close();
}

void loadCostDelete(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de costos de eliminación: " << filename << endl;
        exit(1);
    }
    for (int i = 0; i < 26; ++i) {
        file >> cost_delete[i];
    }
    file.close();
}

void loadCostReplace(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de costos de sustitución: " << filename << endl;
        exit(1);
    }
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            file >> cost_replace[i][j];
        }
    }
    file.close();
}

void loadCostTranspose(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de costos de transposición: " << filename << endl;
        exit(1);
    }
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            file >> cost_transpose[i][j];
        }
    }
    file.close();
}

// Función auxiliar para calcular el mínimo de tres valores
int min3(int a, int b, int c) {
    return min(min(a, b), c);
}

// Función principal para calcular la distancia mínima de edición con programación dinámica
int editDistanceDP(const string& S1, const string& S2) {
    int n = S1.size();
    int m = S2.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // Inicializar casos base
    dp[0][0] = 0;
    for (int i = 1; i <= n; ++i) {
        dp[i][0] = dp[i - 1][0] + cost_delete[S1[i - 1] - 'a'];
    }
    for (int j = 1; j <= m; ++j) {
        dp[0][j] = dp[0][j - 1] + cost_insert[S2[j - 1] - 'a'];
    }

    // Llenar la tabla de DP
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int cost = (S1[i - 1] == S2[j - 1]) ? 0 : cost_replace[S1[i - 1] - 'a'][S2[j - 1] - 'a'];
            dp[i][j] = min3(
                dp[i - 1][j] + cost_delete[S1[i - 1] - 'a'],        // Eliminación
                dp[i][j - 1] + cost_insert[S2[j - 1] - 'a'],        // Inserción
                dp[i - 1][j - 1] + cost                             // Sustitución
            );
            // Verificar transposición
            if (i > 1 && j > 1 && S1[i - 1] == S2[j - 2] && S1[i - 2] == S2[j - 1]) {
                dp[i][j] = min(dp[i][j], dp[i - 2][j - 2] + cost_transpose[S1[i - 1] - 'a'][S1[i - 2] - 'a']);
            }
        }
    }

    return dp[n][m];
}

int main() {
    // Cargar costos desde archivos
    loadCostInsert("cost_insert.txt");
    loadCostDelete("cost_delete.txt");
    loadCostReplace("cost_replace.txt");
    loadCostTranspose("cost_transpose.txt");

    // Leer cadenas de entrada
    string S1, S2;
    cout << "Ingrese la cadena S1: ";
    cin >> S1;
    cout << "Ingrese la cadena S2: ";
    cin >> S2;

    // Convertir las cadenas a minúsculas
    for (char& c : S1) {
        c = tolower(c);
    }
    for (char& c : S2) {
        c = tolower(c);
    }

    // Calcular distancia mínima de edición
    int result = editDistanceDP(S1, S2);
    cout << "La distancia mínima de edición es: " << result << endl;

    return 0;
}
