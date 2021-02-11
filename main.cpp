#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "graph.h"

void processGraph(std::string fileName, unsigned char readFlags) {
    std::cout << "processing " << fileName << std::endl;
    Graph g = Graph(fileName, readFlags);
    g.DFS();

    // Get filename, remove .txt and add .csv
    int beginIdx = fileName.rfind('/');
    std::string outFile = fileName.substr(beginIdx + 1);
    outFile = "out/" + outFile.substr(0, outFile.length() - 4) + ".csv";
    std::cout << "writing to " << outFile << std::endl;
    g.Dump(outFile);
}

void printGraph(std::string fileName, unsigned char readFlags) {
    std::cout << fileName << std::endl;
    std::cout << "MODE: ";
    std::cout << (readFlags & Directed ? "DIRECTED" : "UNDIRECTED") << " ";
    std::cout << (readFlags & Weighted ? "WEIGHTED" : "UNWEIGHTED") << std::endl;

    Graph g = Graph(fileName, readFlags);
    // std::cout << g << std::endl;
    g.DFS();
    // g.BFS(1);
    // g.BFS(2);
    std::cout << g << std::endl;
}

int main(int argc, char const* argv[]) {
    // Default to undirected and unweighted
    unsigned char readFlags = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            readFlags |= Directed;
        } else if (strcmp(argv[i], "-w") == 0) {
            readFlags |= Weighted;
        } else if (argv[i][0] != '-') {
            printGraph(argv[i], readFlags);
            // processGraph(argv[i], readFlags);
        } else {
            std::cerr << "Ignoring invalid argument: " << argv[i] << std::endl;
        }
    }

    return 0;
}
