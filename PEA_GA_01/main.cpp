#include <iostream>
#include <chrono>
#include <iomanip>
#include "GeneticAlgorithm.h"

using namespace std;
using namespace std::chrono;

void menu() {
    int a;
    do {
        GeneticAlgorithm *gens;
        std::cout << "\n1. Zacznij algorytm genetyczny  \n"
                  << "Wpisz 1:";
        std::cin >> a;
        switch (a - 1) {
            case 0:
                gens = new GeneticAlgorithm();
                double p;
                int  populationSize, populationCopyNumber, generationNumber, selectionType, crossoverType;
                int algorithmTimeLimit;
                std::cout << "Podaj prawdopodobienstwo \n";
                cin >> p;
                std::cout << "Podaj rozmiar populacji \n";
                cin >> populationSize;
                std::cout << "Podaj liczbe kopiowanych osobnikow \n";
                cin >> populationCopyNumber;
                std::cout << "Podaj liczbe generacji \n";
                cin >> generationNumber;
                selectionType = 0;
                std::cout << "Podaj typ krzyzowania \n"
                             "0 - Partially Mapped Crossover\n"
                             "1 - Order Crossover Operator\n";
                cin >> crossoverType;

                std::cout << "Podaj maksymalny czas dzialania algorytmu w sekundach: \n";
                cin >> algorithmTimeLimit;

                auto start = high_resolution_clock::now();

                gens->startAlgorithm(p,
                                     populationSize,
                                     populationCopyNumber,
                                     generationNumber,
                                     selectionType,
                                     crossoverType,
                                     algorithmTimeLimit);

                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<chrono::duration<double>>(stop - start);

                std::cout << "Czas wykonania: " << std::fixed << std::setprecision(2)
                                                << std::chrono::duration<double>(duration).count() << " s " << std::endl;


                delete gens;
                break;
        }

    } while (a != 2);
}

int main() {
    menu();
    return 0;
}
