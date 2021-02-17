#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include "graph.h"
#include "min_heap.h"

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
    // std::vector<Element> S{};
    // for (int i = 20; i > 0; i--) {
    //     S.push_back(Element{ 2 * i, i });
    // }

    // MinHeap h(S);
    // std::cout << h << std::endl;

    // for (int i = 0; i < S.size(); i++) {
    //     std::cout << h.DeleteMin() << std::endl;
    // }

    std::vector<Element> S{};
    // std::vector<int> keys{ 21, 22, 25, 30, 31, 26, 25, 45 };
    // std::vector<int> IDs{ 3,8,2,4,6,1,5,7 };
    std::vector<int> keys{ 27,26, 25, 21, 30, 25, 31, 45, 22 };
    std::vector<int> IDs{ 0, 1, 2,3,4,5,6,7,8 };

    for (int i = 0; i < keys.size(); i++) {
        S.push_back(Element{ IDs[i], keys[i] });
    }

    MinHeap h(S);
    std::cout << h << std::endl;

    // for (int i = 0; i < S.size(); i++) {
    //     std::cout << h.DeleteMin() << std::endl;
    // }

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
