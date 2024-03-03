#include "Matrix.h"
#include "Matrix.h"
#include <iostream>
#include <random>
#include "LoadFromFile.h"


void Matrix::createTables() {

    matrixWeights = new int *[size];

    for (int i = 0; i < size; i++) {
        matrixWeights[i] = new int[size];
    }

}


void Matrix::loadData() {
    auto *loadFromFile = new LoadFromFile();


    if (loadFromFile->openFile()) {
        size = loadFromFile->getDataFromFile();
        createTables();
        createMatrix(loadFromFile);
        std::cout << "Wczytano dane\n";

    }
    delete loadFromFile;
}


void Matrix::loadDataName(std::string fileName) {
    auto *loadFromFile = new LoadFromFile();


    if (loadFromFile->openFileName(fileName)) {
        size = loadFromFile->getDataFromFile();
        createTables();
        createMatrix(loadFromFile);
        std::cout << "Wczytano dane\n";

    }
    delete loadFromFile;
}


void Matrix::createMatrix(LoadFromFile *loadFromFile) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrixWeights[i][j] = loadFromFile->getDataFromFile();
            if (i == j)
                matrixWeights[i][j] = INT_MAX;
        }
    }
    std::string nameOfFile = loadFromFile->getFileName();

    if (nameOfFile  == "ftv47.atsp") {
        optimum = 1776;
    } else if (nameOfFile == "ftv170.atsp") {
        optimum = 2755;
    } else if (nameOfFile == "rbg403.atsp") {
        optimum = 2465;
    } else {
        optimum = 0 ;
    }

}



void Matrix::createNewMatrix(bool directed, int vNumber) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, vNumber - 1);
    std::uniform_int_distribution<> dist2(0, 100000);

    size = vNumber;

    createTables();


    if (!directed) {
        for (int i = 0; i < size; i++) {
            matrixWeights[i][i] = INT_MAX;
            for (int j = i + 1; j < size; j++) {
                matrixWeights[i][j] = dist2(gen);
                matrixWeights[j][i] = matrixWeights[i][j];
            }
        }

    } else {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == j)
                    matrixWeights[i][i] = INT_MAX;
                else
                    matrixWeights[i][j] = dist2(gen);
            }
        }
    }

}


int Matrix::getSize() const {
    return size;
}

int **Matrix::getMatrixWeights() const {
    return matrixWeights;
}

int Matrix::getOptimum() const {
    return optimum;
}