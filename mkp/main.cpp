#include "utils.hpp"
#include "mkp.hpp"
#include <Eigen/Dense>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <omp.h>
#include <getopt.h>

void generarCSV(const std::string& inputFilePath, const std::string& outputFilePath, int numHilos) {
    int numItems, numDimensiones, optimoConocido;
    Eigen::VectorXi beneficios;
    Eigen::MatrixXi pesos;
    Eigen::VectorXi capacidades;
    
    // Leer datos desde archivo
    try {
        Utils::leerDatosDesdeArchivo(inputFilePath, numItems, numDimensiones, optimoConocido, beneficios, pesos, capacidades);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    
    std::cout << "El optimo conocido es: " << optimoConocido << "\n";
    std::stringstream encabezado;
    encabezado << "n,f,obj,rel," << Utils::vectorXiToStringVariables(numItems);
    Utils::escribirLineaCSV(outputFilePath, encabezado.str());

    // Crear instancia del problema
    Mkp mkp(numItems, numDimensiones, beneficios, pesos, capacidades);
    
    // Imprimir el problema
    mkp.print();
    int hasta = pow(2, numItems);
    
    omp_set_num_threads(numHilos);
    
    #pragma omp parallel for
    for (int i = 1; i <= hasta; i++) {
        std::stringstream ss;
        ss << i << ",";
        
        // Obtener la solución binaria
        Eigen::VectorXi sol = Utils::enteroABinario(i, numItems);
        // Invertir la solución
        Eigen::VectorXi solucion = sol.reverse().eval();

        if (mkp.esFactible(solucion)) {
            ss << "1,";
        } else {
            ss << "0,";
        }
        
        int objetivo = mkp.calcularObjetivo(solucion);
        ss << objetivo << ",";
        float relativo = static_cast<float>(objetivo) / optimoConocido;
        ss << std::fixed << std::setprecision(3) << relativo << ","; // Fijar la precisión a 3 decimales
        ss << Utils::vectorXiToString(solucion);
        
        // Escribir la línea en el archivo inmediatamente
        Utils::escribirLineaCSV(outputFilePath, ss.str());
    }
}


int main(int argc, char* argv[]) {
    std::string inputFilePath;
    std::string outputFilePath = "output.csv";
    int numHilos = 4;
    
    int opt;
    while ((opt = getopt(argc, argv, "i:o:t:")) != -1) {
        switch (opt) {
            case 'i':
                inputFilePath = optarg;
                break;
            case 'o':
                outputFilePath = optarg;
                break;
            case 't':
                numHilos = std::stoi(optarg);
                break;
            default:
                std::cerr << "Uso: " << argv[0] << " -i <inputFile> -o <outputFile> -t <numHilos>" << std::endl;
                return 1;
        }
    }
    
    if (inputFilePath.empty()) {
        std::cerr << "Error: El archivo de entrada es obligatorio." << std::endl;
        return 1;
    }
    
    generarCSV(inputFilePath, outputFilePath, numHilos);
    
    return 0;
}
