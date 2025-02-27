#ifndef UTILS_HPP
#define UTILS_HPP

#include <Eigen/Dense>
#include <string>
#include <vector>

class Utils {
public:
    static void leerDatosDesdeArchivo(const std::string& nombreArchivo, 
                                      int& numItems, 
                                      int& numDimensiones, 
                                      int& optimoConocido,
                                      Eigen::VectorXi& beneficios, 
                                      Eigen::MatrixXi& pesos, 
                                      Eigen::VectorXi& capacidades);

    static Eigen::VectorXi enteroABinario(int numero, int longitud);

    static int binarioAEntero(const Eigen::VectorXi& binario);

    static std::string vectorXiToString(const Eigen::VectorXi& vector);

    static std::string vectorXiToStringVariables(int numItems);

    static void escribirLineaCSV(const std::string& nombreArchivo, const std::string& linea);

    static std::vector<std::pair<int, int>> generarNParesEspiral(int n);


private:
    static std::vector<int> leerNumerosDesdeArchivo(const std::string& nombreArchivo);
};

#endif // UTILS_HPP
