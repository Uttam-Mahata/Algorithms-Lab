#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Define a structure to represent an edge in the graph
struct Edge {
    int u, v;
    double weight;
};

// Define a structure to represent a graph
struct Graph {
    int V; // Number of vertices
    vector<vector<pair<int, double>>> adj; // Adjacency list

    Graph(int V) : V(V) {
        adj.resize(V);
    }

    // Function to add an edge to the graph
    void addEdge(int u, int v, double weight) {
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }
};

// Function to generate a random graph with given number of vertices and density
Graph generateRandomGraph(int numVertices, double density) {
    Graph graph(numVertices);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (int u = 0; u < numVertices; ++u) {
        for (int v = u + 1; v < numVertices; ++v) {
            if (dis(gen) < density) {
                double weight = dis(gen); // Random weight between 0 and 1
                graph.addEdge(u, v, weight);
            }
        }
    }
    return graph;
}

// Function to perform Prim's algorithm to find the Minimum Spanning Tree (MST)
double primMST(const Graph& graph) {
    int V = graph.V;
    vector<bool> visited(V, false);
    vector<double> key(V, numeric_limits<double>::max());
    key[0] = 0; // Start from vertex 0

    auto start = high_resolution_clock::now();

    double mstWeight = 0;

    for (int count = 0; count < V; ++count) {
        int u = -1;
        // Find the vertex with the minimum key value among vertices not yet included in MST
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && (u == -1 || key[v] < key[u])) {
                u = v;
            }
        }

        visited[u] = true;
        mstWeight += key[u];

        // Update key values of adjacent vertices of the picked vertex
        for (const auto& neighbor : graph.adj[u]) {
            int v = neighbor.first;
            double weight = neighbor.second;
            if (!visited[v] && weight < key[v]) {
                key[v] = weight;
            }
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    return duration.count();
}

// Function to evaluate performance by testing with different graph sizes and densities
void evaluatePerformance() {
    ofstream outputFile("performance_results.csv");
    if (!outputFile.is_open()) {
        cout << "Unable to open output file!" << endl;
        return;
    }

    // Header for CSV file
    outputFile << "NumVertices,Density,TimeTaken(ms)" << endl;

    vector<int> numVerticesList = {10, 50, 100}; // Different graph sizes
    vector<double> densityList = {0.2, 0.5, 0.8}; // Different densities

    for (int numVertices : numVerticesList) {
        for (double density : densityList) {
            // Perform multiple runs and take average time
            const int numRuns = 5;
            double totalTime = 0;

            for (int i = 0; i < numRuns; ++i) {
                Graph graph = generateRandomGraph(numVertices, density);
                totalTime += primMST(graph);
            }

            double averageTime = totalTime / numRuns;
            outputFile << numVertices << "," << density << "," << averageTime << endl;
        }
    }

    outputFile.close();
}

int main() {
    // print the time taken to run the primMST function
    // print the minimum spanning tree edges
    // print the total weight of the minimum spanning tree

    Graph graph(5);
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 3, 6);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 8);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 4, 7);
    graph.addEdge(3, 4, 9);

    auto start = high_resolution_clock::now();
    double timeTaken = primMST(graph);

    cout << "Time taken to find MST: " << timeTaken << "ms" << endl;

    

    return 0;
}
