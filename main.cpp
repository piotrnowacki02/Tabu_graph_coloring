#include <chrono>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Vertex.hpp"
#include "Graph.hpp"
#include "Step.hpp"

using namespace std;

void generateTxt(string filename, int numberOfVertices)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku do zapisu." << std::endl;
        return;
    }
    file << numberOfVertices << std::endl;
    std::srand(std::time(0));
    for (int i = 1; i <= numberOfVertices; ++i) {
        for (int j = i + 1; j <= numberOfVertices; ++j) {
            // Losowe wybieranie, czy istnieje krawędź między wierzchołkami 50/50 albo istnieje albo nie who knows
            if (std::rand() % 100 < 60) {
                file << i << " " << j << std::endl;
            }
        }
    }
}

int main()
{
    string file = "gc500.txt";
    // generateTxt(file, 350);
    int k = 70;
    int rep = 100;
    int maxTabuList = 2;
    int maxIter = 9000000;


    // int testI = -1;
    // reiter: 
    // testI++;
    // // testy dla rep le450
    // // if(testI == 10 || testI == 40)
    // //     rep += 3;
    // // if(testI == 20 ||testI == 30 )
    // //     rep += 2;
    // // if(testI == 50)
    // //     rep += 5;
    // // if(testI == 60)
    // //     rep += 10;
    // if(testI == 10 || testI == 20 || testI == 30)
    //     rep += 50;
    // if(testI == 40 || testI == 50)
    //     rep += 100;
    

    
    Graph graph;

    graph.readGraphFromTxt(file);
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    graph.colorizeGreedy();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    graph.printHowManyColors();
    std::cout << "Czas = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]\n" << std::endl;
    graph.UpdateVis();
    begin = std::chrono::steady_clock::now();
    graph.tabuSearch(k, rep, maxTabuList , maxIter);
    end = std::chrono::steady_clock::now();
    int duration = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    std::cout << "Czas = " << duration << "[s]\n" << std::endl;


    //wyniki zapisywane do pliku
    // std::string resultFilePath = "Wyniki_" + file;
    // std::ofstream outputFile(resultFilePath, std::ios::app);
    // outputFile << k << "\t" << rep << "\t" << maxTabuList << "\t\t" << maxIter << "\t" << duration << "\n";
    // outputFile.close();
    // goto reiter;

    return 0;
}