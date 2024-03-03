#include "LoadFromFile.h"
#include <iostream>
#include <sstream>

std::string nameOfFile;


bool LoadFromFile::openFile() {

    std::cout << "Podaj nazwe pliku" << std::endl;
    std::cin >> nameOfFile;
    std::cout << "\n";
    file.open(nameOfFile);
    if (!file.is_open()) {
        std::cout << "Nie ma takiego pliku\n";
        exit(0);
        return false;
    } else {
        std::string line;
        std::getline(file, line); // Read the first line
        std::cout << "Otworzono plik " << line << "\n";
        return true;
    }
}

bool LoadFromFile::openFileName(const std::string& nameOfFile) {
    file.open(nameOfFile);
    if (!file.is_open()) {
        std::cout << "Nie ma takiego pliku\n";
        return false;
    } else {
        std::string line;
        std::getline(file, line); // Read the first line
        std::cout << "Otworzono plik " << line << "\n";
        return true;
    }
}

int LoadFromFile::getDataFromFile() {
    int number;

    std::string line;

    while (!gotDimension && file >> line) {
        if (line == "DIMENSION:") {
            gotDimension = true;
            // Read the dimension
            file >> number;
            return number;
        }
    }

    // Skip lines until reaching the EDGE_WEIGHT_SECTION
    while (!gotMatrix && file >> line) {
        if (line == "EDGE_WEIGHT_SECTION") {
            gotMatrix = true;
        }
    }

    // Read the matrix element
    file >> number;

    return number;
}

std::string LoadFromFile::getFileName() {
    return nameOfFile;
}

