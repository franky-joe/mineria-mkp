#include "utils.hpp"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

void Utils::leerDatosDesdeArchivo(const std::string& nombreArchivo, 
                                  int& numItems, 
                                  int& numDimensiones, 
                                  int& optimoConocido,
                                  Eigen::VectorXi& beneficios, 
                                  Eigen::MatrixXi& pesos, 
                                  Eigen::VectorXi& capacidades) {
    // Leer todos los números del archivo
    std::vector<int> numeros = leerNumerosDesdeArchivo(nombreArchivo);
    auto it = numeros.begin();

    // Leer nItems y nDimensiones
    numItems = *it++;
    numDimensiones = *it++;

    // Leer optimo conocido
    optimoConocido = *it++;

    // Leer beneficios
    beneficios.resize(numItems);
    for (int i = 0; i < numItems; ++i) {
        beneficios[i] = *it++;
    }

    // Leer matriz de pesos
    pesos.resize(numDimensiones, numItems);
    for (int i = 0; i < numDimensiones; ++i) {
        for (int j = 0; j < numItems; ++j) {
            pesos(i, j) = *it++;
        }
    }

    // Leer capacidades
    capacidades.resize(numDimensiones);
    for (int i = 0; i < numDimensiones; ++i) {
        capacidades[i] = *it++;
    }
}

std::vector<int> Utils::leerNumerosDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se puede abrir el archivo: " + nombreArchivo);
    }

    std::vector<int> numeros;
    int numero;
    while (archivo >> numero) {
        numeros.push_back(numero);
    }

    archivo.close();
    return numeros;
}

Eigen::VectorXi Utils::enteroABinario(int numero, int longitud) {
    Eigen::VectorXi binario(longitud);
    for (int i = 0; i < longitud; ++i) {
        binario(i) = numero % 2;
        numero /= 2;
    }
    return binario.reverse(); 
}

int Utils::binarioAEntero(const Eigen::VectorXi& binario) {
    int numero = 0;
    int potencia = 1;
    for (int i = binario.size() - 1; i >= 0; --i) {
        numero += binario(i) * potencia;
        potencia *= 2;
    }
    return numero;
}

std::string Utils::vectorXiToString(const Eigen::VectorXi& vector) {
    std::stringstream ss;
    for (int i = 0; i < vector.size(); ++i) {
        if (i > 0) {
            ss << ",";
        }
        ss << vector(i);
    }
    return ss.str();
}

std::string Utils::vectorXiToStringVariables(int numItems){
    std::stringstream ss;
    for (int i = 0; i < numItems; ++i){
        if (i > 0){
            ss << ",";
        }
        ss << "x" << i+1;
    }
    return ss.str();
}

void Utils::escribirLineaCSV(const std::string& nombreArchivo, const std::string& linea) {
    std::ofstream archivo(nombreArchivo, std::ios::app); // Abrir el archivo en modo append

    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + nombreArchivo);
    }

    archivo << linea << "\n"; // Escribir la línea en el archivo
    archivo.close(); // Cerrar el archivo
}

std::vector<std::pair<int, int>> Utils::generarNParesEspiral(int n) {
    std::vector<std::pair<int, int>> spiral;
    std::cout << "prueba\n";
    spiral.emplace_back(1, 1);
    int x = 2, y = 1;
    int direccion = 0; 

    for (int i = 0; i < n; ++i) {
        spiral.emplace_back(x, y);

        // Decidir la siguiente dirección basada en el punto anterior
        if (x == 1){
            if (direccion == 2){
                direccion = 1;
            }else{
                direccion = 0;
            }
        }

        if (y == 1){
            if (direccion == 3){
                direccion = 0;
            }else{
                direccion = 1;
            }
        }

        if (x == y){
            if (direccion == 0){
                direccion = 3;
            }else{
                direccion = 2;
            }
        }
        // Mover en la dirección actual
        switch (direccion) {
            case 0: ++x; break; // Derecha
            case 1: ++y; break; // Arriba
            case 2: --x; break; // Izquierda
            case 3: --y; break; // Abajo
        }
    }

    return spiral;
}
