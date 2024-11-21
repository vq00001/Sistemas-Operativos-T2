#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "tabla_paginacion.cpp"

using namespace std;

// Algoritmos de reemplazo
enum ReplacementAlgorithm {
    FIFO, LRU, CLOCK, OPTIMAL
};

// Función para parsear el argumento del algoritmo
ReplacementAlgorithm parseAlgorithm(const string &algorithm) {
    if (algorithm == "FIFO") return FIFO;
    if (algorithm == "LRU") return LRU;
    if (algorithm == "CLOCK") return CLOCK;
    if (algorithm == "OPTIMAL") return OPTIMAL;
    throw invalid_argument("Algoritmo no soportado");
}

// Función para leer referencias desde el archivo
vector<int> readReferences(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("No se pudo abrir el archivo de referencias");
    }
    vector<int> references;
    int ref;
    while (file >> ref) {
        references.push_back(ref);
    }
    file.close();
    return references;
}

// Función principal
int main(int argc, char *argv[]) {
    int frames = 0;
    string algorithm;
    string referenceFile;

    // Parseo de argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            frames = stoi(argv[++i]);
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            algorithm = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            referenceFile = argv[++i];
        }
    }

    if (frames <= 0 || algorithm.empty() || referenceFile.empty()) {
        cerr << "Uso: ./mvirtual -m <marcos> -a <algoritmo> -f <archivo>" << endl;
        return 1;
    }

    cout << "Marcos: " << frames << ", Algoritmo: " << algorithm << ", Archivo: " << referenceFile << endl;

    try {
        ReplacementAlgorithm algo = parseAlgorithm(algorithm);
        vector<int> references = readReferences(referenceFile);

        // Crear la tabla de páginas con el número de marcos especificado
        PageTable pageTable(frames);


        // Insertar las referencias en la tabla de páginas
        for (size_t i = 0; i < references.size(); i++) {
            int pageNumber = references[i];
            if (pageTable.getFrame(pageNumber) == -1) {
                cout << "Insertando página " << pageNumber << endl;
                pageTable.insertPage(pageNumber, i % frames);
            }
        }

        pageTable.displayTable();  // Mostrar la tabla de páginas

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}