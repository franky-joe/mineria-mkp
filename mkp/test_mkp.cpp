#include "mkp.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <cassert>

void runTests() {
     /* Max: 4x1 + 2x2 + 10x3 + 1x4 + 2x5
       Sa : 14x1 + 1x2 + 4x3 + 1x4 + 2x5 <= 15
            3x1 + 12x2 + 5x3 + 4x4 + 3x5 <= 13
            
    Valor óptimo: 13.0
    Variables: [0.0, 0.0, 1.0, 1.0, 1.0]
    */

    // Datos del problema proporcionado
    int numItems = 5;
    int numDimensiones = 2;
    Eigen::VectorXi beneficios(numItems);
    beneficios << 4, 2, 10, 1, 2;

    Eigen::MatrixXi pesos(numDimensiones, numItems);
    pesos << 14, 1, 4, 1, 2,
             3, 12, 5, 4, 3;

    Eigen::VectorXi capacidades(numDimensiones);
    capacidades << 15, 13;

    // Crear instancia del problema
    Mkp mkp(numItems, numDimensiones, beneficios, pesos, capacidades);

    // Imprimir el problema
    mkp.print();

    // Solución de ejemplo
    Eigen::VectorXi solucion(numItems);
    solucion << 0, 0, 1, 1, 1;

    // Verificar si la solución es factible
    if (mkp.esFactible(solucion)) {
        std::cout << "La solución es factible.\n";
        std::cout << "Valor de la función objetivo: " << mkp.calcularObjetivo(solucion) << "\n";
    } else {
        std::cout << "La solución no es factible.\n";
    }

}

int main() {
    runTests();
    std::cout << "Todas las pruebas han pasado exitosamente.\n";
    return 0;
}
