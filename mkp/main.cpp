#include "utils.hpp"
#include "mkp.hpp"
#include <Eigen/Dense>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

void generarCSV(const std::string& inputFilePath, const std::string& outputFilePath) {
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
    encabezado << "n,x,y,f,obj,rel," << Utils::vectorXiToStringVariables(numItems);
    Utils::escribirLineaCSV(outputFilePath, encabezado.str());

    // Crear instancia del problema
    Mkp mkp(numItems, numDimensiones, beneficios, pesos, capacidades);

    // Imprimir el problema
    mkp.print();
    int hasta = pow(2, numItems);
    
    // Rango de soluciones 
    auto cordenadas = Utils::generarNParesEspiral(hasta + 1);
   
    for (int i = 1; i <= hasta; i++) {
        const auto& pair = cordenadas[i - 1];
        std::stringstream ss;
        ss << i << ",";
        ss << pair.first << "," << pair.second << ",";
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
        ss << std::fixed << std::setprecision(3) << relativo<<","; // Fijar la precisión a 2 decimales
        std::cout << Utils::vectorXiToString(solucion) << "\n";

        ss << Utils::vectorXiToString(solucion);
        Utils::escribirLineaCSV(outputFilePath, ss.str());
    }
}
void generarCSVResumen(const std::string& inputFilePath, const std::string& outputFilePath) {
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
    encabezado << "n,x,y,f,rel" ;
    Utils::escribirLineaCSV(outputFilePath, encabezado.str());

    // Crear instancia del problema
    Mkp mkp(numItems, numDimensiones, beneficios, pesos, capacidades);

    // Imprimir el problema
    mkp.print();
    //int hasta = pow(2, numItems);
    int hasta = pow(2, numItems);
    // Rango de soluciones 
    auto cordenadas = Utils::generarNParesEspiral(hasta + 1);
   
    for (int i = 1; i <= hasta; i++) {
        const auto& pair = cordenadas[i - 1];
        std::stringstream ss;
        ss << i << ",";
        ss << pair.first << "," << pair.second << ",";
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
        //ss << objetivo << ",";
        float relativo = static_cast<float>(objetivo) / optimoConocido;
        ss << std::fixed << std::setprecision(3) << relativo; // Fijar la precisión a 2 decimales
        //ss << std::fixed << std::setprecision(3) << relativo<<","; // Fijar la precisión a 2 decimales

        //ss << Utils::vectorXiToString(solucion);
        Utils::escribirLineaCSV(outputFilePath, ss.str());
    }
}
struct Solucion {
    int indice;
    int x;
    int y;
    Eigen::VectorXi solucion;
    bool esFactible;
    int objetivo;
    float relativo;
};

void generarCSVResumenSinOptimo(const std::string& inputFilePath, const std::string& outputFilePath) {
    int numItems, numDimensiones, opt;
    Eigen::VectorXi beneficios;
    Eigen::MatrixXi pesos;
    Eigen::VectorXi capacidades;
    
    // Leer datos desde archivo
    try {
        Utils::leerDatosDesdeArchivo(inputFilePath, numItems, numDimensiones, opt, beneficios, pesos, capacidades);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    // Crear instancia del problema
    Mkp mkp(numItems, numDimensiones, beneficios, pesos, capacidades);
    mkp.print();

    int hasta = pow(2, numItems);
    auto cordenadas = Utils::generarNParesEspiral(hasta + 1);

    // Vector para almacenar todas las soluciones
    std::vector<Solucion> soluciones;
    int mejorObjetivo = std::numeric_limits<int>::min();

    // Generar y almacenar todas las soluciones
    for (int i = 1; i <= hasta; i++) {
        const auto& pair = cordenadas[i - 1];
        Eigen::VectorXi sol = Utils::enteroABinario(i, numItems);
        Eigen::VectorXi solucion = sol.reverse().eval();

        bool factible = mkp.esFactible(solucion);
        int objetivo = mkp.calcularObjetivo(solucion);

        // Actualizar el mejor objetivo si la solución es factible
        if (factible && objetivo > mejorObjetivo) {
            mejorObjetivo = objetivo;
        }

        soluciones.push_back({i, pair.first, pair.second, solucion, factible, objetivo, 0.0f});
    }

    // Calcular el valor relativo basado en el mejor objetivo encontrado
    for (auto& sol : soluciones) {
        if (mejorObjetivo != 0) {
            sol.relativo = static_cast<float>(sol.objetivo) / mejorObjetivo;
        } else {
            sol.relativo = 0.0f; // En caso de que no haya solución factible
        }
    }

    // Escribir las soluciones en el archivo CSV
    std::stringstream encabezado;
    encabezado << "n,x,y,f,rel";
    Utils::escribirLineaCSV(outputFilePath, encabezado.str());

    for (const auto& sol : soluciones) {
        std::stringstream ss;
        ss << sol.indice << ",";
        ss << sol.x << "," << sol.y << ",";
        ss << (sol.esFactible ? "1" : "0") << ",";
        ss << std::fixed << std::setprecision(3) << sol.relativo;
        Utils::escribirLineaCSV(outputFilePath, ss.str());
    }
}
int main() {

    std::string inputFilePath = "/home/roko/cursos/mkp/datos/solucionadosPequenos/menosDe20Items/pet4.dat";
    std::string outputFilePath = "pet4-todo.csv";
    //std::string outEntero = "pet1_11-14-full.csv";
    generarCSV(inputFilePath, outputFilePath);
    //generarCSVResumenSinOptimo(inputFilePath, outputFilePath);

    return 0;
}