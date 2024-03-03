#include "GeneticAlgorithm.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

GeneticAlgorithm::GeneticAlgorithm() {
    matrixWeights = new Matrix();
    matrix = matrixWeights->getMatrixWeights();
}

void GeneticAlgorithm::startAlgorithm(double probability, int populationSize, int populationCopyNumber,
                                      int generationNumber, int selectionType, int crossoverType, int algorithmTimeLimit) {

    srand(100);
    mt19937 gen(rand());

    double crossoverCoefficient = 0.8;
    double mutationCoefficient = 0.01;

    generateRandomParents(gen, populationCopyNumber);



    mainLoop(gen, probability, populationSize, populationCopyNumber, generationNumber, selectionType, crossoverType, crossoverCoefficient, mutationCoefficient, algorithmTimeLimit);


}

// main loop of the algorithm
void GeneticAlgorithm::mainLoop(mt19937 &engine, double probability, int populationSize, int populationCopyNumber,
                                int generationNumber, int selectionType, int crossoverType, double crossoverCoefficient, double mutationCoefficient, int algorithmTimeLimit) {

    auto start = high_resolution_clock::now();

    pair<int, int> parents;
    auto pointer1 = population.begin();
    auto pointer2 = population.begin();

    for (int i = 0; i < generationNumber; i++) {
        for (int j = 0; j < populationSize; j++) {

            // Check elapsed time and stop if it exceeds the limit
            auto currentTime = high_resolution_clock::now();
            auto elapsedDuration = duration_cast<seconds>(currentTime - start);
            if (elapsedDuration.count() >= algorithmTimeLimit) {
                cout << "Algorytm zatrzymal sie, poniewaz osiagnieto limit czasu" << endl;
                return;
            }

            vector<unsigned> child1(matrixWeights->getSize(), 0);
            vector<unsigned> child2(matrixWeights->getSize(), 0);

            sort(population.begin(), population.end(), cmp);
            vector<float> fitnessValueTable(population.size(), 0);
            countFitnessValue(fitnessValueTable);
            parents = rouletteWheelSelection(engine, fitnessValueTable);

            pointer1 = population.begin() + parents.first;
            pointer2 = population.begin() + parents.second;

            switch (crossoverType) {
                case 0:
                    makePartiallyMappedCrossover(
                            pointer1->second,
                            pointer2->second,
                            child1,
                            child2,
                            engine, crossoverCoefficient);
                    break;
                case 1:
                    makeOrderCrossoverOperator(pointer1->second,
                                               pointer2->second,
                                               child1,
                                               child2,
                                               engine, crossoverCoefficient);
                    break;
            }

            checkMutation(engine, child1, probability, i * populationSize + j, mutationCoefficient);
            checkMutation(engine, child2, probability, i * populationSize + j, mutationCoefficient);
        }

        // Check elapsed time and stop if it exceeds the limit
        auto currentTime = high_resolution_clock::now();
        auto elapsedDuration = duration_cast<seconds>(currentTime - start);
        if (elapsedDuration.count() >= algorithmTimeLimit) {
            cout << "Algorytm zatrzymal sie, poniewaz osiagnieto limit czasu" << endl;
            return;
        }

        sort(population.begin(), population.end(), cmp);
        copyPopulation(populationCopyNumber);
    }

    showPath(globalPath);
    showPRD(generationNumber * populationSize);
}

// generate random parents for the algorithm
void GeneticAlgorithm::countFitnessValue(vector<float> &fitness) {

    for (int i = 0; i < population.size(); i++) {
        fitness.at(i) = ((float) finalCost / (population.begin() + i)->first);
    }

}

// roulette wheel selection
pair<int, int> GeneticAlgorithm::rouletteWheelSelection(mt19937 &engine, vector<float> &fitness) {

    float sum = 0;

    for (int i = 0; i < population.size(); i++) {
        sum += fitness.at(i);
    }

    uniform_real_distribution<float> randomNumber(0, sum);
    float r = randomNumber(engine);
    float r2 = randomNumber(engine);

    if (r2 < r)
        swap(r2, r);


    pair<int, int> parents(-1, 0);
    float sum2 = 0;

    for (int i = 0; i < population.size(); i++) {
        sum2 += fitness.at(i);
        if (r <= sum2 && parents.first == -1)
            parents.first = i;
        else if (r2 <= sum2) {
            parents.second = i;
            return parents;
        }
    }

    return parents;
}


void GeneticAlgorithm::generateRandomParents(std::mt19937 engine, int populationCopyNumber) {
    vector<unsigned int> vertexes;

    for (unsigned int i = 0; i < matrixWeights->getSize(); i++) vertexes.push_back(i);

    for (unsigned int i = 0; i < populationCopyNumber; i++) {
        shuffle(vertexes.begin(), vertexes.end(), engine);
        pair<int, vector<unsigned int>> p = {this->calculateCost(vertexes), vertexes};
        if (p.first < finalCost) {
            finalCost = p.first;
            globalPath = p.second;
            showPRD(i);
        }
        population.push_back(p);
    }

}

//partially mapped crossover
void
GeneticAlgorithm::makePartiallyMappedCrossover(const vector<unsigned int> &parent1, const vector<unsigned int> &parent2,
                                               std::vector<unsigned int> &child1, std::vector<unsigned int> &child2,
                                               std::mt19937 engine, double crossoverCoefficient) {
    uniform_int_distribution<> cutPoints(1, matrixWeights->getSize() - 1);
    uniform_real_distribution<double> generateProbability(0.0, 1.0);

    int a = 0, b = 0;
    while (a == b || a > b) {
        a = cutPoints(engine);
        b = cutPoints(engine);
    }

    for (int i = a; i <= b; i++) {
        child1.at(i) = parent2.at(i);
        child2.at(i) = parent1.at(i);
    }
    if (generateProbability(engine) < crossoverCoefficient) {
        unsigned int firstNoCopied1 = a;
        unsigned int firstNoCopied2 = a;

        for (int i = 0; i < parent1.size(); i++) {
            if (i < a || i > b) {
                copyCutPoints(parent1, &child1, a, b, i, firstNoCopied1);
                copyCutPoints(parent2, &child2, a, b, i, firstNoCopied2);
            }
        }

        completeRestPoints(firstNoCopied1, parent2, &child1, a, b);
        completeRestPoints(firstNoCopied2, parent1, &child2, a, b);
    } else {
        // If crossover is not applied, copy the entire parents to the children
        child1 = parent1;
        child2 = parent2;
    }
}


//order crossover operator
void GeneticAlgorithm::makeOrderCrossoverOperator(const std::vector<unsigned int> &parent1,
                                                  const std::vector<unsigned int> &parent2,
                                                  std::vector<unsigned int> &child1,
                                                  std::vector<unsigned int> &child2,
                                                  std::mt19937 engine,
                                                  double crossoverCoefficient) {

    uniform_int_distribution<> cutPoints(1, static_cast<int>(parent1.size()) - 2);
    uniform_real_distribution<double> generateProbability(0.0, 1.0);
    int a = 0, b = 0;
    // Randomization of intersection points
    while (a == b) {
        a = cutPoints(engine);
        b = cutPoints(engine);
    }
    if (a > b)
        swap(a, b);
    // Copying intersection areas
    for (int i = a; i <= b; i++) {
        child1.at(i) = parent1.at(i);
        child2.at(i) = parent2.at(i);
    }
    if (generateProbability(engine) < crossoverCoefficient) {
        copySequenceFromOrderCrossoverOperator(parent2, child1, a, b);
        copySequenceFromOrderCrossoverOperator(parent1, child2, a, b);
    }
    else {
        // If crossover is not applied, copy the rest of the genes as they are
        for (int i = 0; i < a; i++) {
            child1.at(i) = parent1.at(i);
            child2.at(i) = parent2.at(i);
        }
        for (int i = b + 1; i < parent1.size(); i++) {
            child1.at(i) = parent1.at(i);
            child2.at(i) = parent2.at(i);
        }
    }
}


// copying the remaining vertices of the sequence that were not in the cut area
void GeneticAlgorithm::copySequenceFromOrderCrossoverOperator(const vector<unsigned int> &parent,
                                                              vector<unsigned int> &child, const int &a, const int &b) {
    int j = b + 1;
    auto v = parent.begin() + b + 1;

    do {

        if (find(child.begin() + a, child.begin() + b + 1, *v) == child.begin() + b + 1) {
            child.at(j) = *v;
            if (v != parent.end() - 1)
                v++;
            else
                v = parent.begin();
        } else {
            if (v != parent.end() - 1)
                v++;
            else
                v = parent.begin();
            continue;
        }

        if (j < child.size() - 1)
            j++;
        else
            j = 0;

    } while (j != a);

}

// copy cut points
void GeneticAlgorithm::copyCutPoints(const vector<unsigned int> &parent, vector<unsigned int> *child,
                                     int a, int b, int i, unsigned int &firstNoCopied) {

    unsigned int vertex = (parent.begin() + i).operator*();
    if (find(child->begin() + a, child->begin() + b + 1, vertex) == child->begin() + b + 1)
        (child->begin() + i).operator*() = vertex;
    else if (firstNoCopied == a)
        firstNoCopied = i;

}

// completing the rest of the vertices that were not in the cut area
void GeneticAlgorithm::completeRestPoints(const unsigned int firstNoCopied, const vector<unsigned int> &parent,
                                          vector<unsigned int> *child,
                                          int a, int b) {
    for (unsigned int i = firstNoCopied; i < child->size(); i++) {
        if (i < a || i > b) {
            for (int j = 0; j < parent.size(); j++) {
                if (j < a || j > b) {
                    if (find(child->begin(), child->end(), parent.at(j)) == child->end()) {
                        (child->begin() + i).operator*() = parent.at(j);
                        break;
                    }
                }
            }
        }
    }
}

// checking on the basis of probability whether a mutation may occur
void GeneticAlgorithm::checkMutation(std::mt19937 engine, vector<unsigned int> &child, double probability, int iter, double mutationCoefficient) {
    uniform_real_distribution<double> generateProbability(0.0, 1.0);

    population.emplace_back(calculateCost(child), child);

    pair<int, vector<unsigned int>> pointerLast((population.end() - 1)->first, (population.end() - 1)->second);

    if (finalCost > pointerLast.first) {
        finalCost = pointerLast.first;
        globalPath = pointerLast.second;
        showPRD(iter);
    }

    double genProb = generateProbability(engine);
    if (genProb <= 0.1)
        makeMutationRandomly(&pointerLast, engine, mutationCoefficient);
    else if (genProb <= probability)
        makeMutationBest(&pointerLast);
    else
        return;

    population.push_back(pointerLast);

    if (finalCost > pointerLast.first) {
        finalCost = pointerLast.first;
        globalPath = pointerLast.second;
        showPRD(iter);
    }
}

// finding the best mutation
void GeneticAlgorithm::makeMutationBest(pair<int, vector<unsigned int>> *path) {
    int bestI, bestJ, bestSwap = INT_MAX;
    int swapValue;

    for (int i = 1; i < path->second.size() - 3; i++) {
        for (int j = i + 1; j < path->second.size() - 2; j++) {
            swapValue = swapNeighbors(&path->second, i, j);
            if (swapValue < bestSwap) {
                bestI = i;
                bestJ = j;
                bestSwap = swapValue;
            }
        }
    }
    swap(path->second.at(bestI), path->second.at(bestJ));
    path->first += bestSwap;
}

// random mutation
void GeneticAlgorithm::makeMutationRandomly(pair<int, vector<unsigned int>> *path, mt19937 &engine, double mutationCoefficient) {
    uniform_int_distribution<> swapIndex(1, path->second.size() - 2);

    int i = 0, j = 0;

    while (i == j) {
        i = swapIndex(engine);
        j = swapIndex(engine);
    }

    if (j < i)
        swap(i, j);

    path->first += mutationCoefficient * swapNeighbors(&path->second, i, j);
    swap(path->second.at(i), path->second.at(j));

}

//
int GeneticAlgorithm::swapNeighbors(vector<unsigned int> *path, int i, int j) {
    int subtractOldEdges = 0;
    int addNewEdges = 0;

    if (j - i == 1) {
        subtractOldEdges += matrix[path->at(i - 1)][path->at(i)];
        subtractOldEdges += matrix[path->at(i)][path->at(j)];
        subtractOldEdges += matrix[path->at(j)][path->at(j + 1)];

        addNewEdges += matrix[path->at(i - 1)][path->at(j)];
        addNewEdges += matrix[path->at(j)][path->at(i)];
        addNewEdges += matrix[path->at(i)][path->at(j + 1)];

    } else {
        subtractOldEdges += matrix[path->at(i - 1)][path->at(i)];
        subtractOldEdges += matrix[path->at(i)][path->at(i + 1)];
        subtractOldEdges += matrix[path->at(j - 1)][path->at(j)];
        subtractOldEdges += matrix[path->at(j)][path->at(j + 1)];

        addNewEdges += matrix[path->at(i - 1)][path->at(j)];
        addNewEdges += matrix[path->at(j)][path->at(i + 1)];
        addNewEdges += matrix[path->at(j - 1)][path->at(i)];
        addNewEdges += matrix[path->at(i)][path->at(j + 1)];
    }

    return addNewEdges - subtractOldEdges;
}

void GeneticAlgorithm::showPRD(int iter) {
    std::cout << iter
              << " "
              << finalCost
              << "   "
              << 100 * (((float) (finalCost - matrixWeights->getOptimum()))
                        / (float) matrixWeights->getOptimum())
              << "% \n";
}

void GeneticAlgorithm::showPath(vector<unsigned int> path) {

    for (unsigned int i: path)
        cout << i << "->";

    cout << path.front() << "\n";
}

// route cost calculation
int GeneticAlgorithm::calculateCost(vector<unsigned int> path) {

    int cost = 0;
    auto i = path.begin();
    while (i != (path.end() - 1)) {
        cost += matrix[i.operator*()][(i + 1).operator*()];
        i.operator++();
    }
    cost += matrix[path.back()][path.front()];
    return cost;
}

// copying the best individuals from the population to the new generation
void GeneticAlgorithm::copyPopulation(int number) {
    std::vector<std::pair<int, std::vector<unsigned int>>> p2;
    for (int i = 0; i < number; i++) p2.push_back(population.at(i));
    population = p2;
}


