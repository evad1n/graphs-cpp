#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <time.h>
#include <cmath>
#include "graph.h"
#include "min_heap.h"

void printOptions(unsigned char options) {
    std::ostringstream ss;
    ss << "MODE: ";
    ss << (options & Directed ? "DIRECTED " : "UNDIRECTED ");
    ss << (options & Weighted ? "WEIGHTED " : "UNWEIGHTED ");
    ss << (options & PrintEdges ? "PRINT_EDGES " : "");
    ss << (options & UseList ? "ADJACENCY LIST " : "ADJACENCY MATRIX ");
    std::cout << ss.str() << std::endl;
}

void processGraph(std::string fileName, unsigned char options) {
    std::cout << fileName << std::endl;
    printOptions(options);

    Graph g = Graph(fileName, options);

    // Get filename, remove .txt and add .csv
    int endIdx = fileName.rfind('-');
    int beginIdx = fileName.rfind('-', endIdx - 1);
    std::string size = fileName.substr(beginIdx + 1, endIdx - beginIdx - 1);
    std::cout << size << std::endl;

    // std::string outFile = "out/prim-times.csv";
    // std::ofstream f;
    // f.open(outFile, std::ofstream::out | std::ofstream::app);
    // int cost = g.Prim();
    // // f << fileName <<
}

void printGraph(std::string fileName, unsigned char options) {
    std::cout << fileName << std::endl;
    printOptions(options);

    Graph g = Graph(fileName, options);
    // g.DFS();
    // g.BFS(1);
    // g.Dijkstra(1);
    int cost = g.Prim();
    std::cout << cost << std::endl;

    // Temp for 5b
    //std::ofstream out;
    //out.open("out/5b.csv", std::ios_base::app);
    //out << fileName << ", " << cost << "\n";
    //out.close();

    //std::cout << g << std::endl;
}

// makeheap(), deletemin(), insert() and decreasekey()
void timeBinaryHeap() {
    srand(time(NULL));
    std::ofstream f("min-heap.csv");

    for (int i = 4; i <= 28; i++) {
        double avgMakeHeap, avgDeleteMin, avgInsert, avgDecreaseKey = 0;
        int size = 2 << i - 1;
        int reps = 10;
        int innerReps = 100000;
        if (i > 22) {
            reps = 4;
        }
        if (i > 24) {
            reps = 2;
        }
        for (int j = 0; j < reps; j++) {
            // Create vector
            std::vector<Element> S(size);
            for (int el = 0; el < size; el++) {
                S[el] = Element{ el, (rand() % 100) - 100 };
            }

            double start, elapsed;

            // n log n
            start = clock();
            MinHeap h(S);
            elapsed = (clock() - start) / CLOCKS_PER_SEC;
            avgMakeHeap += elapsed;

            for (int k = 0; k < innerReps; k++) {
                // Sift
                start = clock();
                h.DeleteMin();
                elapsed = (clock() - start) / CLOCKS_PER_SEC;
                avgDeleteMin += elapsed;

                // Bubble
                start = clock();
                h.Insert(Element{ size + 1, k });
                elapsed = (clock() - start) / CLOCKS_PER_SEC;
                avgInsert += elapsed;

                // Bubble
                start = clock();
                h.DecreaseKey(size / 2, S[size / 2].key - 10);
                elapsed = (clock() - start) / CLOCKS_PER_SEC;
                avgDecreaseKey += elapsed;
            }
        }

        std::ostringstream ss;
        // Calculate average and write to file
        ss << i << ", " << size << ", ";
        ss << avgMakeHeap / reps << ", ";
        ss << avgDeleteMin / (reps * innerReps) << ", ";
        ss << avgInsert / (reps * innerReps) << ", ";
        ss << avgDecreaseKey / (reps * innerReps);

        std::cout << ss.str() << std::endl;
        f << ss.str() << "\n";
    }
    f.close();
}

void testBinaryHeap() {

    std::vector<Element> S{};
    // std::vector<int> keys{ 21, 22, 25, 30, 31, 26, 25, 45 };
    // std::vector<int> IDs{ 3,8,2,4,6,1,5,7 };
    std::vector<int> keys{ 27,26, 25, 21, 30, 25, 31, 45, 22, 45 };
    std::vector<int> IDs{ 0, 1, 2,3,4,5,6,7,8, 9 };

    for (int i = 0; i < keys.size(); i++) {
        S.push_back(Element{ IDs[i], keys[i] });
    }

    MinHeap h(S);
    std::cout << h << std::endl;

    h.DecreaseKey(9, 30);
    std::cout << h << std::endl;

    for (int i = 0; i < S.size(); i++) {
        int ID = h.DeleteMin();
        std::cout << ID << ", " << keys[ID] << std::endl;
        std::cout << h << std::endl;
    }

}

void timePrims(std::vector<std::string> files, unsigned char opts) {
    std::ofstream f("out/prim-times.csv");
    printOptions(opts);


    for (auto fileName : files) {
        double avgTime = 0;
        double reps = 0;
        int cost;
        // Get size of graph from filename
        int endIdx = fileName.rfind('-');
        int beginIdx = fileName.rfind('-', endIdx - 1);
        int size = atoi(fileName.substr(beginIdx + 1, endIdx - beginIdx - 1).c_str());


        Graph g = Graph(fileName, opts);

        double innerReps = 1;
        if (size <= 100000) {
            innerReps = 4;
        }
        if (size <= 10000) {
            innerReps = 10;
        }
        if (size <= 1000) {
            innerReps = 100;
        }
        for (size_t i = 0; i < innerReps; i++) {
            // Time it
            reps++;
            double start, elapsed;
            start = clock();
            cost = g.Prim();
            elapsed = (clock() - start) / CLOCKS_PER_SEC;
            avgTime += elapsed;
        }

        std::cout << fileName << ", " << avgTime / reps << ", " << cost << std::endl;
        f << fileName << ", " << avgTime / reps << ", " << cost << "\n";

    }

    f.close();
}


void readInputs(int argc, char const* argv[]) {
    // Default to undirected and unweighted, matrix
    unsigned char options = 0;
    std::vector<std::string> files;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            options |= Directed;
        } else if (strcmp(argv[i], "-w") == 0) {
            options |= Weighted;
        } else if (strcmp(argv[i], "-e") == 0) {
            options |= PrintEdges;
        } else if (strcmp(argv[i], "-p") == 0) {
            options |= OnlyPrint;
        } else if (strcmp(argv[i], "-l") == 0) {
            options |= UseList;
        } else if (argv[i][0] == '-') {
            // Multi args
            if (strstr(argv[i], "d") != NULL)
                options |= Directed;
            if (strstr(argv[i], "w") != NULL)
                options |= Weighted;
            if (strstr(argv[i], "e") != NULL)
                options |= PrintEdges;
            if (strstr(argv[i], "p") != NULL)
                options |= OnlyPrint;
            if (strstr(argv[i], "l") != NULL)
                options |= UseList;
        } else if (argv[i][0] != '-') {
            files.push_back(argv[i]);
        } else {
            std::cerr << "Invalid argument: " << argv[i] << std::endl;
            std::ostringstream ss;
            ss << "\n";
            ss << "-d | Set DIRECTED mode for reading from a file\n";
            ss << "-w | Set WEIGHTED mode for reading from a file\n";
            ss << "-e | Print edges when printing graphs\n";
            ss << "-p | Just print graphs\n";
            ss << "-l | Use adjacency list instead of a matrix\n";
            std::cerr << ss.str() << std::endl;
        }
    }
    // for (auto f : files) {
    //     (options & OnlyPrint) ? printGraph(f, options) : processGraph(f, options);
    // }
    timePrims(files, options);

}

int main(int argc, char const* argv[]) {
    readInputs(argc, argv);
    // testBinaryHeap();
    //  timeBinaryHeap();

    return 0;
}
