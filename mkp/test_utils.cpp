#include "utils.hpp"
#include "mkp.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <cassert>

int testLeerDatosDesdeArchivo() {
  // Datos del problema
    int numItems, numDimensiones, optimoConocido;
    Eigen::VectorXi beneficios;
    Eigen::MatrixXi pesos;
    Eigen::VectorXi capacidades;

    std::string filePath = "datos/flei.data";

    

    // Leer datos desde archivo
    try {
        Utils::leerDatosDesdeArchivo(filePath, numItems, numDimensiones, optimoConocido, beneficios, pesos, capacidades);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    std::cout << "Archivo: " << filePath << "\n";
    std::cout << "Número de ítems: " << numItems << "\n";
    std::cout << "Número de dimensiones: " << numDimensiones << "\n";
    std::cout << "Vector de beneficios (p):\n" << beneficios << "\n";
    std::cout << "Matriz de pesos (w):\n" << pesos << "\n";
    std::cout << "Vector de capacidades (c):\n" << capacidades << "\n";
    return 0;
}
int testCambioBase(){
    int numItems = 20;

    // Test binario a decimal
    Eigen::VectorXi vectorBinario(numItems);
    vectorBinario << 0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1;
    int solucionEnDecimal = Utils::binarioAEntero(vectorBinario);
    std::cout << "Test binario a decimal \n";
    std::cout << "Vector binario:\n" << vectorBinario << "\n";
    std::cout << "Numero Decimal: " << solucionEnDecimal << "\n";
    // Test 2 binario a decimal 
    Eigen::VectorXi vectorBinario2 = Eigen::VectorXi::Zero(8); // Vector de tamaño 8 inicializado a cero
    vectorBinario2 << 1, 0, 1, 1, 1, 1, 1, 1;
    solucionEnDecimal = Utils::binarioAEntero(vectorBinario2);
    std::cout << "Test 2 binario a decimal \n";
    std::cout << "Vector binario:\n" << vectorBinario2 << "\n";
    std::cout << "Numero Decimal: " << solucionEnDecimal << "\n";
    // Test decimal a binario 
    int numeroEntero = 349525;
    Eigen::VectorXi solucionBinaria = Utils::enteroABinario(numeroEntero, numItems);
    std::cout << "Test decimal a binario \n";
    std::cout << "Numero Decimal: " << numeroEntero << "\n";
    std::cout << "Vector binario: \n" << solucionBinaria << "\n";

    // Test 2 decimal a binario 
    numeroEntero = 5;
    solucionBinaria = Utils::enteroABinario(numeroEntero, numItems);
    std::cout << "Test 2 decimal a binario \n";
    std::cout << "Numero Decimal: " << numeroEntero << "\n";
    std::cout << "Vector binario:\n" << solucionBinaria << "\n";
    std::cout << "Vector binario invertido:\n" << solucionBinaria.reverse() << "\n";


    return 0;
}
void testEscribirArchivo(){
    int numItems = 20;
    // Test binario a decimal
    Eigen::VectorXi vectorBinario(numItems);
    vectorBinario << 0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1;

    std::string nombreArchivo = "datos.csv";
    
   
    std::string encabezado = Utils::vectorXiToStringVariables(numItems);
    Utils::escribirLineaCSV(nombreArchivo, encabezado);
    std::string linea = Utils::vectorXiToString(vectorBinario);
    Utils::escribirLineaCSV(nombreArchivo, linea);
    std::cout << "Se escribió la línea correctamente en " << nombreArchivo << std::endl;
}

void testGenerarEspira() {
    int n = 100;
    std::vector<std::pair<int, int>> pares = Utils::generarNParesEspiral(n);
    std::cout << "Generando n = " << n << " pares del espiral\n";

    for (auto i = 0u; i < pares.size(); ++i) {
        const auto& pair = pares[i];
        std::cout << "n = " << (i + 1) << " ==> (" << pair.first << ", " << pair.second << ")\n";
    }
    std::cout << std::endl;
}



int main() {
    //testLeerDatosDesdeArchivo();
    testCambioBase();
    //testEscribirArchivo();
    //testGenerarEspira();
    std::cout << "Todas las pruebas han pasado exitosamente.\n";
    return 0;
}
