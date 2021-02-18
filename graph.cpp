#include <vector>
#include <queue>
#include <sstream>
#include "graph.h"
#include "min_heap.h"

Graph::Graph(std::string fileName, unsigned char options)
    : opts(options) {
    std::ifstream f;
    f.open(fileName);

    std::string line;
    int lineNumber = 1;
    // Get first line: number of vertices
    getline(f, line);
    this->numV = std::stoi(line);
    // Initialize vertex storage
    std::vector<Vertex> vertices(this->numV);
    for (int i = 0; i < this->numV; i++) {
        vertices[i] = Vertex{
            i,
        false,
            0,
            0,
            0,
            std::vector<DistancePath>(this->numV, DistancePath{})
        };
    }

    // Initialize adjacency matrix to all 0s
    std::vector<std::vector<int>> matrix(this->numV, std::vector<int>(this->numV));

    while (getline(f, line)) {
        lineNumber++;
        std::istringstream ss(line);
        int from, to;
        // Get vertex numbers for the edge
        ss >> from >> to;
        // Catch bad input errors
        if (from > this->numV || to > this->numV) {
            std::cerr << "Not enough vertices provided @ line " << lineNumber << std::endl;
            exit(EXIT_FAILURE);
        }
        // Account for 1-indexing
        from--;
        to--;
        // Set weight
        int weight = 1;
        if (options & Weighted) {
            ss >> weight;
        }
        // Set in matrix
        matrix[from][to] = weight;
        // Also do reverse 
        if (options ^ Directed) {
            matrix[to][from] = weight;
        }
    }

    this->vertices = vertices;
    this->adjacencies = matrix;

    f.close();
}

Graph::~Graph() {}

void Graph::Explore(int vertexIndex) {
    this->vertices[vertexIndex].visited = true;
    this->Previsit(vertexIndex);
    for (int i = 0; i < this->numV; i++) {
        if (this->adjacencies[vertexIndex][i] != 0 && !this->vertices[i].visited) {
            this->Explore(i);
        }
    }
    this->Postvisit(vertexIndex);
}

void Graph::DFS() {
    // Reset values
    for (auto v : this->vertices)
        v.visited = false;
    this->cc = 0;
    this->clock = 1;

    for (int i = 0; i < this->numV; i++) {
        if (!this->vertices[i].visited) {
            this->cc++;
            this->Explore(i);
        }
    }
}

void Graph::BFS(int startVertexIndex) {
    // Correct for 1 indexing
    startVertexIndex--;
    Vertex* start = &this->vertices[startVertexIndex];

    // Reset values
    for (auto v : this->vertices) {
        for (auto other : v.distancePaths) {
            other.distance = INT32_MAX;
        }
    }
    start->distancePaths[startVertexIndex].distance = 0;

    std::queue<int> q;
    q.push(startVertexIndex);

    while (q.size() > 0) {
        int currIdx = q.front();
        q.pop();

        for (int i = 0; i < this->numV; i++) {
            if (start->distancePaths[i].distance == INT32_MAX) {
                q.push(i);
                start->distancePaths[i].distance = start->distancePaths[currIdx].distance + 1;
            }
        }
    }
}

void Graph::Djikstra(int startVertexIndex) {
    // Correct for 1 indexing
    startVertexIndex--;
    Vertex* start = &this->vertices[startVertexIndex];

    // Binary min heap
    std::vector<Element> els(this->numV);
    for (int i = 0; i < this->numV; i++) {
        els[i] = Element{ i, INT32_MAX };
    }
    MinHeap heap(els);

    this->vertices[startVertexIndex].distancePaths[startVertexIndex].distance = 0;

    while (heap.Size() > 0) {
        int currIdx = heap.DeleteMin();

        // for (int i = 0; i < this->adjacencies[currIdx].size(); i++) {
        //     int weight = this->adjacencies[currIdx][i];
        //     int* currDist = &this->vertices[startVertexIndex].distances[i];
        //     int* newDist = &this->vertices[startVertexIndex].distances[currIdx];
        //     if (weight != 0 && *currDist > *newDist + weight) {
        //         *currDist = *newDist + weight;
        //         q.push(i);
        //     }
        // }
    }
}

void Graph::Previsit(int vertexIndex) {
    this->vertices[vertexIndex].component = this->cc;
    this->vertices[vertexIndex].pre = this->clock;
    this->clock++;
}

void Graph::Postvisit(int vertexIndex) {
    this->vertices[vertexIndex].post = this->clock;
    this->clock++;
}

void Graph::Dump(std::string fileName) {
    std::ofstream f(fileName);

    for (int i = 0; i < this->numV; i++) {
        std::ostringstream ss;
        const Vertex* v = &this->vertices[i];
        ss << i + 1 << ", " << v->pre << ", " << v->post << ", " << v->component << "\n";
        f << ss.str();
    }

    f.close();
}

std::ostream& operator<<(std::ostream& out, const Graph& g) {
    out << g.GetVertices();
    if (g.opts & PrintEdges) {
        out << g.GetEdges();
    }
    return out;
}

std::string Graph::GetVertices() const {
    std::ostringstream ss;
    ss << "Vertices:" << std::endl;
    for (int i = 0; i < this->numV; i++) {
        const Vertex* v = &this->vertices[i];
        ss << "Vertex " << i + 1 << ": {visited: " << v->visited << ", component: " << v->component << ", pre: " << v->pre << ", post: " << v->post << "}" << std::endl;
        // Show distances if some form of BFS was performed on this vertex
        if (v->distancePaths[i].distance != INT32_MAX) {
            ss << this->GetDistances(i);
        }
    }
    return ss.str();
}

std::string Graph::GetDistances(int vertexIdx) const {
    std::ostringstream ss;
    const Vertex* v = &this->vertices[vertexIdx];
    ss << "Distances from Vertex " << vertexIdx << " {" << std::endl;
    for (int i = 0; i < this->numV; i++) {
        ss << "\tTo Vertex " << i << ": ";
        if (v->distancePaths[i].distance == INT32_MAX)
            ss << "∞";
        else
            ss << v->distancePaths[i].distance;
        ss << ", Prev: ";
        if (v->distancePaths[i].prev != NULL)
            ss << v->distancePaths[i].prev->number;
        else
            ss << "NULL";
        ss << std::endl;
    }
    ss << "}" << std::endl;

    return ss.str();
}

std::string Graph::GetEdges() const {
    std::ostringstream ss;
    ss << "\nEdges:" << std::endl;
    for (int from = 0; from < this->numV; from++) {
        for (int to = 0; to < this->numV; to++) {
            if (this->adjacencies[from][to] > 0) {
                ss << "from: " << from + 1 << ", to: " << to + 1 << ", weight: " << this->adjacencies[from][to] << std::endl;
            }
        }
    }
    return ss.str();
}
