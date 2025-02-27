#ifndef MKP_HPP
#define MKP_HPP

#include <Eigen/Dense>

class Mkp {
private:
    int n; // Número de ítems
    int m; // Número de dimensiones
    Eigen::VectorXi p; // Vector de beneficios (enteros)
    Eigen::MatrixXi w; // Matriz de pesos (enteros)
    Eigen::VectorXi c; // Vector de capacidades (enteros)

public:
    Mkp(int nItems, int nDimensiones, Eigen::VectorXi beneficios, Eigen::MatrixXi pesos, Eigen::VectorXi capacidades);
    ~Mkp();
    bool esFactible(Eigen::VectorXi);
    int calcularObjetivo(Eigen::VectorXi);
    std::string toString();
    void print();

};

#endif // MKP_HPP
