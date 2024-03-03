
#ifndef _PEA_GA_01_MATRIX_H
#define _PEA_GA_01_MATRIX_H


#include "LoadFromFile.h"

class Matrix {
public:
    Matrix() { loadData(); }

    Matrix(std::string fileName) { loadDataName(fileName); }

    Matrix(bool directed, int vNumber) {
        createNewMatrix(directed, vNumber);
    }

    int getSize() const;

    int getOptimum() const;

    int **getMatrixWeights() const;

private:
    int size;

    int optimum;

    int **matrixWeights = nullptr;

    void createTables();

    void createMatrix(LoadFromFile *loadFromFile);

    void loadData();

    void createNewMatrix(bool directed, int vNumber);

    void loadDataName(std::string fileName);
};





#endif //_PEA_GA_01_MATRIX_H
