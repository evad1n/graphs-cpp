#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include "graph.h"
#include "binary_heap.h"

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

void testBinaryHeap() {
    BinaryHeap h(11);

    h.Insert(3);
    std::cout << h << std::endl;
    h.Insert(2);
    std::cout << h << std::endl;
    h.Insert(1);
    std::cout << h << std::endl;
    h.Insert(6);
    std::cout << h << std::endl;
    h.Insert(5);
    std::cout << h << std::endl;
    h.Insert(4);
    std::cout << h.Index(4) << std::endl;
    std::cout << h << std::endl;
    h.Insert(7);
    std::cout << h << std::endl;
    for (int i = 0; i < 6; i++) {
        std::cout << h.DeleteMin() << std::endl;
        std::cout << h << std::endl;
    }
    std::cout << h << std::endl;
}

void readInputs(int argc, char const* argv[]) {
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
}

int main(int argc, char const* argv[]) {
    // readInputs(argc, argv);
    testBinaryHeap();

    return 0;
}
