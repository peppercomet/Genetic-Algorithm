
#ifndef _PEA_GA_01_GENETICALGORITHM_H
#define _PEA_GA_01_GENETICALGORITHM_H

#include <vector>
#include <random>
#include <ctime>
#include "Matrix.h"

using namespace std;

class GeneticAlgorithm {

private:

    Matrix *matrixWeights;

    std::vector<std::pair<int, std::vector<unsigned int>>> population;

    static bool cmp(std::pair<int, std::vector<unsigned int>> &a,
                    std::pair<int, std::vector<unsigned int>> &b) {
        return a.first < b.first;
    }
    int **matrix;

    int finalCost = INT_MAX;
    std::vector<unsigned int> globalPath;

    void mainLoop(mt19937 &engine, double probability, int populationSize, int populationCopyNumber,
                  int generationNumber, int selectionType, int crossoverType, double crossoverCoefficient, double mutationCoefficient, int algorithmTimeLimit);

    void generateRandomParents(std::mt19937 engine, int populationCopyNumber);

    void countFitnessValue(vector<float> &fitness);

    pair<int, int> rouletteWheelSelection(mt19937 &engine, vector<float> &fitness);


    void checkMutation(std::mt19937 engine, vector<unsigned int> &child, double probability, int iter, double mutationCoefficient);

    void makeMutationBest(std::pair<int, std::vector<unsigned int>> *path);

    int swapNeighbors(std::vector<unsigned int> *path, int i, int j);

    void makePartiallyMappedCrossover(const vector<unsigned int> &parent1, const vector<unsigned int> &parent2,
                                      std::vector<unsigned int> &child1, std::vector<unsigned int> &child2,
                                      std::mt19937 engine, double crossoverCoefficient);

    static void
    copyCutPoints(const std::vector<unsigned int> &parent, std::vector<unsigned int> *child, int a, int b, int i,
                  unsigned int &firstNoCopied);

    static void
    completeRestPoints(unsigned int firstNoCopied, const std::vector<unsigned int> &parent,
                       std::vector<unsigned int> *child, int a, int b);

    static void
    makeOrderCrossoverOperator(const vector<unsigned int>& parent1, const vector<unsigned int>& parent2, vector<unsigned int> &child1,
                               vector<unsigned int> &child2, mt19937 engine, double crossoverCoefficient);

    static void
    copySequenceFromOrderCrossoverOperator(const vector<unsigned int> &parent, vector<unsigned int> &child,
                                           const int &a,
                                           const int &b);

    void copyPopulation(int number);

    void showPRD(int iter);

    int calculateCost(std::vector<unsigned int> path);

    static void showPath(std::vector<unsigned int> path);

public:
    GeneticAlgorithm();

    void startAlgorithm(double probability, int populationSize, int populationCopyNumber,
                        int generationNumber,  int selectionType, int crossoverType, int algorithmTimeLimit);

    void makeMutationRandomly(pair<int, vector<unsigned int>> *path, mt19937 &engine, double mutationCoefficient);


};





#endif //_PEA_GA_01_GENETICALGORITHM_H
