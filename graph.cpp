#include <vector>
#include <queue>
#include <sstream>
#include "graph.h"
#include "min_heap.h"

Graph::Graph(std::string fileName, unsigned char options)
    : opts(options) {
    std::ifstream f;
    f.open(fileName);
    // Check if file exists
    if (!f.good()) {
        std::cerr << "Error reading file: " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    int lineNumber = 1;
    // Get first line: number of vertices
    while (getline(f, line)) {
        // Skip comments
        if (line.at(0) == '#')
            continue;
        else
            break;
    }
    this->numV = std::stoi(line);
    // Initialize vertex storage
    this->vertices = std::vector<Vertex>(numV);
    for (int i = 0; i < this->numV; i++) {
        this->vertices[i] = Vertex{
            "",
            i,
        false,
            0,
            0,
            0,
            std::vector<DistancePath>(this->numV, DistancePath{})
        };
    }

    // Read optional labels
    auto pos = f.tellg();
    getline(f, line);
    if (line.at(0) == '-') {
        this->labeled = true;
        while (getline(f, line)) {
            // Skip comments
            if (line.at(0) == '#')
                continue;
            // Termination of labels
            if (line.at(0) == '-')
                break;

            std::string label;
            int num;
            std::istringstream ss(line);
            ss >> label >> num;
            // 1 indexing
            num--;
            // Add to map
            this->labels.insert(std::pair<std::string, int>(label, num));
            this->vertices[num].label = label;
        }
    } else {
        // Go back a line
        f.seekg(pos);
    }


    // Initialize adjacency matrix to all 0s
    std::vector<std::vector<int>> matrix(this->numV, std::vector<int>(this->numV));

    while (getline(f, line)) {
        // Skip comments
        if (line.at(0) == '#')
            continue;

        lineNumber++;
        std::istringstream ss(line);

        int from, to;
        if (this->labeled) {
            std::string fromLabel, toLabel;
            ss >> fromLabel >> toLabel;
            from = this->labels.at(fromLabel);
            to = this->labels.at(toLabel);
        } else {
            // Get vertex numbers for the edge
            ss >> from >> to;
            // Account for 1-indexing
            from--;
            to--;
        }
        // Catch bad input errors
        if (from >= this->numV || to >= this->numV) {
            std::cerr << "Not enough vertices provided @ line " << lineNumber << std::endl;
            exit(EXIT_FAILURE);
        }
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

void Graph::Previsit(int vertexIndex) {
    this->vertices[vertexIndex].component = this->cc;
    this->vertices[vertexIndex].pre = this->clock;
    this->clock++;
}

void Graph::Postvisit(int vertexIndex) {
    this->vertices[vertexIndex].post = this->clock;
    this->clock++;
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

void Graph::Dijkstra(int startVertexIndex) {
    // Correct for 1 indexing
    startVertexIndex--;
    Vertex* start = &this->vertices[startVertexIndex];

    // Reset values
    for (auto p : start->distancePaths) {
        p.distance = INT32_MAX;
    }

    // Binary min heap
    std::vector<Element> els(this->numV);
    for (int i = 0; i < this->numV; i++) {
        els[i] = Element{ i, INT32_MAX };
    }
    MinHeap heap(els);

    start->distancePaths[startVertexIndex].distance = 0;
    heap.DecreaseKey(startVertexIndex, 0);

    while (heap.Size() > 0) {
        int currIdx = heap.DeleteMin();

        for (int to = 0; to < this->numV; to++) {
            // If there is an edge and we can get to it
            if (this->adjacencies[currIdx][to] != 0 && start->distancePaths[currIdx].distance != INT32_MAX) {
                int weight = this->adjacencies[currIdx][to];
                int* currDist = &start->distancePaths[to].distance;
                int newDist = start->distancePaths[currIdx].distance + weight;
                if (newDist < *currDist) {
                    *currDist = newDist;
                    start->distancePaths[to].prev = &this->vertices[currIdx];
                    heap.DecreaseKey(to, newDist);
                }
            }
        }
    }
}

int Graph::Prim() {
    // Reset
    this->mst = std::vector<MSTEdge>();

    int totalCost = 0;
    // Cost tracking
    std::vector<int> costs(this->numV, INT32_MAX);
    // Prev tracking
    std::vector<int> prev(this->numV, -1);

    // Binary min heap
    std::vector<Element> els(this->numV);
    for (int i = 0; i < this->numV; i++) {
        els[i] = Element{ i, INT32_MAX };
    }
    MinHeap heap(els);

    // Start at first vertex; doesn't matter
    int start = 0;
    costs[start] = start;
    heap.DecreaseKey(start, 0);

    while (heap.Size() > 0) {
        int currIdx = heap.DeleteMin();
        // Add to MST
        if (currIdx != start) {
            this->mst.push_back(
                MSTEdge{ prev[currIdx], currIdx, costs[currIdx] }
            );
        }
        totalCost += costs[currIdx];

        for (int to = 0; to < this->numV; to++) {
            // If there is an edge
            if (this->adjacencies[currIdx][to] != 0) {
                int weight = this->adjacencies[currIdx][to];
                if (costs[to] > weight) {
                    // The edge is better
                    costs[to] = weight;
                    prev[to] = currIdx;
                    heap.DecreaseKey(to, weight);
                }
            }
        }
    }

    this->mstCost = totalCost;
    return totalCost;
}

void Graph::Dump(std::string fileName) {
    std::ofstream f(fileName);

    // Pre/Post/CC
    for (int i = 0; i < this->numV; i++) {
        std::ostringstream ss;
        const Vertex* v = &this->vertices[i];
        ss << (v->label == "" ? std::to_string(i + 1) : v->label) << ", " << v->pre << ", " << v->post << ", " << v->component << "\n";
        f << ss.str();
    }

    f.close();
}

std::ostream& operator<<(std::ostream& out, const Graph& g) {
    out << g.GetVertices();
    if (g.mstCost != 0) {
        out << g.GetMST();
    }
    if (g.opts & PrintEdges) {
        out << g.GetEdges();
    }
    return out;
}

std::string Graph::GetVertices() const {
    std::ostringstream ss;
    ss << "Vertices:\n";
    if (this->labeled) {
        for (int i = 0; i < this->numV; i++) {
            const Vertex* v = &this->vertices[i];
            ss << "Vertex " << v->label << ": {visited: " << v->visited << ", component: " << v->component << ", pre: " << v->pre << ", post: " << v->post << "}\n";
            // Show distances if some form of BFS was performed on this vertex
            if (v->distancePaths[i].distance != INT32_MAX) {
                ss << this->GetDistances(i);
            }
        }
    } else {
        for (int i = 0; i < this->numV; i++) {
            const Vertex* v = &this->vertices[i];
            ss << "Vertex " << i + 1 << ": {visited: " << v->visited << ", component: " << v->component << ", pre: " << v->pre << ", post: " << v->post << "}\n";
            // Show distances if some form of BFS was performed on this vertex
            if (v->distancePaths[i].distance != INT32_MAX) {
                ss << this->GetDistances(i);
            }
        }
    }
    return ss.str();
}

std::string Graph::GetDistances(int vertexIdx) const {
    std::ostringstream ss;
    const Vertex* v = &this->vertices[vertexIdx];
    if (this->labeled) {
        ss << "Distances from Vertex " << v->label << " {\n";
        for (int i = 0; i < this->numV; i++) {
            ss << "\tTo Vertex " << this->vertices[i].label << " => Distance: ";
            if (v->distancePaths[i].distance == INT32_MAX)
                ss << "∞";
            else
                ss << v->distancePaths[i].distance;
            ss << ", Prev: ";
            if (v->distancePaths[i].prev != NULL)
                ss << v->distancePaths[i].prev->number + 1;
            else
                ss << "NULL";
            ss << "\n";
        }
        ss << "}\n";
    } else {
        ss << "Distances from Vertex " << vertexIdx << " {\n";
        for (int i = 0; i < this->numV; i++) {
            ss << "\tTo Vertex " << i + 1 << " => Distance: ";
            if (v->distancePaths[i].distance == INT32_MAX)
                ss << "∞";
            else
                ss << v->distancePaths[i].distance;
            ss << ", Prev: ";
            if (v->distancePaths[i].prev != NULL)
                ss << v->distancePaths[i].prev->number + 1;
            else
                ss << "NULL";
            ss << "\n";
        }
        ss << "}\n";
    }

    return ss.str();
}

std::string Graph::GetEdges() const {
    std::ostringstream ss;
    ss << "\nEdges:\n";
    if (this->labeled) {
        for (int fromIdx = 0; fromIdx < this->numV; fromIdx++) {
            const Vertex* from = &this->vertices[fromIdx];
            for (int toIdx = 0; toIdx < this->numV; toIdx++) {
                const Vertex* to = &this->vertices[toIdx];
                if (this->adjacencies[fromIdx][toIdx] > 0) {
                    ss << "from: " << from->label << ", to: " << to->label << ", weight: " << this->adjacencies[fromIdx][toIdx] << "\n";
                }
            }
        }
    } else {
        for (int from = 0; from < this->numV; from++) {
            for (int to = 0; to < this->numV; to++) {
                if (this->adjacencies[from][to] > 0) {
                    ss << "from: " << from + 1 << ", to: " << to + 1 << ", weight: " << this->adjacencies[from][to] << "\n";
                }
            }
        }
    }
    return ss.str();
}

std::string Graph::GetMST() const {
    std::ostringstream ss;
    ss << "\nMinimum Spanning Tree:\n";
    ss << "Cost: " << this->mstCost << "\n";
    if (this->labeled) {
        for (auto e : this->mst) {
            ss << "from: " << this->vertices[e.from].label << ", to: " << this->vertices[e.to].label << ", weight: " << e.weight << "\n";
        }
    } else {
        for (auto e : this->mst) {
            ss << "from: " << e.from << ", to: " << e.to << ", weight: " << e.weight << "\n";
        }
    }
    return ss.str();
}
