#if !defined(GRAPH_H)
#define GRAPH_H

#include <vector>
#include <fstream>
#include <iostream>

enum ReadFlags {
    Directed = 0x01,
    Weighted = 0x02
};

struct Vertex {
    bool visited; // Temporary for exploring/dfs
    int pre; // The pre number
    int post; // The post number
    int component; // The connected component number
    std::vector<int> distances; // The distances to other vertices in the graph
};

class Graph {
private:
    std::vector<Vertex> vertices; // List of vertices
    std::vector<std::vector<int>> adjacencies; // The adjancency matrix
    int cc; // Connected component number
    int clock; // Previsit/postvisit counter

    void Previsit(int vertexIndex);
    void Postvisit(int vertexIndex);

    std::string GetVertices() const;
    std::string GetDistances(int vertexIdx) const;
    std::string GetEdges() const;

public:
    // Reads graph in from a file
    Graph(std::string fileName, unsigned char readFlags);
    ~Graph();

    // Visits all vertices in the connected component containing vertex v
    void Explore(int vertexIndex);
    // Visits all vertices in the graph
    void DFS();
    // Sets distances to each vertex from the starting vertex
    void BFS(int startVertexIndex);

    void Djikstra(int startVertexIndex);

    // Writes vertex data out to file in csv format
    void Dump(std::string fileName);

    // Prints vertices and edges
    friend std::ostream& operator<<(std::ostream& out, const Graph& g);
};


#endif // GRAPH_H