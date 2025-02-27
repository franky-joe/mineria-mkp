#include "mkp.hpp"
#include <iostream>

Mkp::Mkp(int nItems, int nDimensiones, Eigen::VectorXi beneficios, Eigen::MatrixXi pesos, Eigen::VectorXi capacidades)
    : n(nItems), m(nDimensiones), p(beneficios), w(pesos), c(capacidades) {}

Mkp::~Mkp() {}

bool Mkp::esFactible(Eigen::VectorXi x) {
    Eigen::VectorXi pesosTotales = w * x;
    return (pesosTotales.array() <= c.array()).all();
}

int Mkp::calcularObjetivo(Eigen::VectorXi x) {
    return p.dot(x);
}

std::string Mkp::toString(){
    std::ostringstream oss;
    oss << "Número de ítems: " << n << "\n";
    oss << "Número de dimensiones: " << m << "\n";
    oss << "Función objetivo: Max ";
    for (int i = 0; i < n; ++i) {
        oss << (i > 0 ? " + " : "") << p(i) << "x" << (i+1);
    }
    oss << "\nRestricciones:\n";
    for (int j = 0; j < m; ++j) {
        oss << "C" << (j+1) << ": ";
        for (int i = 0; i < n; ++i) {
            oss << (i > 0 ? " + " : "") << w(j, i) << "x" << (i+1);
        }
        oss << " <= " << c(j) << "\n";
    }
    oss << "Variables de decisión: ";
    for (int i = 0; i < n; ++i) {
        oss << (i > 0 ? ", " : "") << "x" << (i+1);
    }
    oss << " ∈ {0, 1}\n";
    return oss.str();
}

void Mkp::print() {
    std::cout << toString();
}