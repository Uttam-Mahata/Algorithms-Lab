#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Structure to represent an edge
struct Edge {
    int dest;
    double weight;

    Edge(int d, double w) : dest(d), weight(w) {}
};

class Graph {
private:
    int V;
    vector<vector<Edge>> adjList;

public:
    Graph(int vertices) : V(vertices) {
        adjList.resize(V);
    }

    // Function to add an edge to the graph
    void addEdge(int src, int dest, double weight) {
        adjList[src].emplace_back(dest, weight);
        adjList[dest].emplace_back(src, weight); // For undirected graph
    }

    // Function to find the minimum spanning tree using Prim's algorithm
    vector<Edge> primMST(int startVertex) {
        vector<bool> visited(V, false);
        vector<Edge> MST;
        double totalWeight = 0.0;

        // Priority queue to store vertices with their corresponding weights
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        pq.push({0.0, startVertex}); // Start with the given starting vertex

        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            int currentVertex = top.second;
            double currentWeight = top.first;

            // Skip if the vertex is already visited
            if (visited[currentVertex]) continue;

            visited[currentVertex] = true;
            totalWeight += currentWeight;

            // Add edges of the current vertex to the priority queue
            for (const auto& edge : adjList[currentVertex]) {
                if (!visited[edge.dest]) {
                    pq.push({edge.weight, edge.dest});
                    MST.emplace_back(currentVertex, edge.weight);
                }
            }
        }

        return MST;
    }
};

int main() {
    // Example graph
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);

    // Prim's MST
    cout << "Prim's Minimum Spanning Tree:" << endl;
    vector<Edge> MST = g.primMST(0); // Start from vertex 0
    double totalWeight = 0.0;
    for (const auto& edge : MST) {
        totalWeight += edge.weight;
        cout << "To: " << edge.dest << ", Weight: " << edge.weight << endl;
    }
    cout << "Total weight of MST: " << totalWeight << endl;

    return 0;
}
