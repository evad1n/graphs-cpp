#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include "graph.h"

void processGraph(std::string fileName, unsigned char options) {
    std::ostringstream ss;
    ss << fileName << std::endl;
    ss << "MODE: ";
    ss << (options & Directed ? "DIRECTED " : "UNDIRECTED ");
    ss << (options & Weighted ? "WEIGHTED " : "UNWEIGHTED ");
    ss << (options & PrintEdges ? "PRINT_EDGES " : "");
    std::cout << ss.str() << std::endl;

    Graph g = Graph(fileName, options);
    // g.DFS();

    // Get filename, remove .txt and add .csv
    int beginIdx = fileName.rfind('/');
    std::string outFile = fileName.substr(beginIdx + 1);
    outFile = "out/" + outFile.substr(0, outFile.length() - 4) + ".csv";
    std::cout << "writing to " << outFile << std::endl;
    g.Dump(outFile);
}

void printGraph(std::string fileName, unsigned char options) {
    std::ostringstream ss;
    ss << fileName << std::endl;
    ss << "MODE: ";
    ss << (options & Directed ? "DIRECTED " : "UNDIRECTED ");
    ss << (options & Weighted ? "WEIGHTED " : "UNWEIGHTED ");
    ss << (options & PrintEdges ? "PRINT_EDGES " : "");
    std::cout << ss.str() << std::endl;

    Graph g = Graph(fileName, options);
    g.DFS();
    g.BFS(1);
    g.BFS(2);
    std::cout << g << std::endl;
}

int main(int argc, char const* argv[]) {
    // Default to undirected and unweighted
    unsigned char options = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            options |= Directed;
        } else if (strcmp(argv[i], "-w") == 0) {
            options |= Weighted;
        } else if (strcmp(argv[i], "-e") == 0) {
            options |= PrintEdges;
        } else if (strcmp(argv[i], "-p") == 0) {
            options |= OnlyPrint;
        } else if (argv[i][0] != '-') {
            options& OnlyPrint ? printGraph(argv[i], options) : processGraph(argv[i], options);
        } else {
            std::cerr << "Ignoring invalid argument: " << argv[i] << std::endl;
        }
    }

    return 0;
}
