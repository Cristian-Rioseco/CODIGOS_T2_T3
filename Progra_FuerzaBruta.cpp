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

// Función recursiva para calcular la distancia mínima de edición usando fuerza bruta
int editDistanceBruteForce(const string& S1, const string& S2, int i, int j) {
    // Caso base: si una de las cadenas está vacía
    if (i == 0 && j == 0) {
        return 0;
    }
    if (i == 0) {
        int cost = 0;
        for (int k = 0; k < j; ++k) {
            cost += cost_insert[S2[k] - 'a'];
        }
        return cost;
    }
    if (j == 0) {
        int cost = 0;
        for (int k = 0; k < i; ++k) {
            cost += cost_delete[S1[k] - 'a'];
        }
        return cost;
    }

    int costSubstitute = (S1[i - 1] == S2[j - 1]) ? 0 : cost_replace[S1[i - 1] - 'a'][S2[j - 1] - 'a'];
    int substitute = editDistanceBruteForce(S1, S2, i - 1, j - 1) + costSubstitute;

    int deleteOp = editDistanceBruteForce(S1, S2, i - 1, j) + cost_delete[S1[i - 1] - 'a'];

    int insertOp = editDistanceBruteForce(S1, S2, i, j - 1) + cost_insert[S2[j - 1] - 'a'];

    int result = min3(substitute, deleteOp, insertOp);

    // Verificar transposición
    if (i > 1 && j > 1 && S1[i - 1] == S2[j - 2] && S1[i - 2] == S2[j - 1]) {
        int transposeCost = cost_transpose[S1[i - 2] - 'a'][S1[i - 1] - 'a'];
        int transposeOp = editDistanceBruteForce(S1, S2, i - 2, j - 2) + transposeCost;
        result = min(result, transposeOp);
    }

    return result;
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

    // Calcular distancia mínima de edición usando fuerza bruta
    int result = editDistanceBruteForce(S1, S2, S1.size(), S2.size());
    cout << "La distancia mínima de edición es: " << result << endl;

    return 0;
}
