cmake_minimum_required(VERSION 3.26)
project(_PEA_GA_01)

set(CMAKE_CXX_STANDARD 17)

add_executable(_PEA_GA_01 main.cpp
        loadFromFile.cpp
        Matrix.cpp
        GeneticAlgorithm.cpp
        Matrix.h
        loadFromFile.h
        GeneticAlgorithm.h
)
