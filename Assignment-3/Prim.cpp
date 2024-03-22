#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max(); // Infinity value

// Structure to represent a node in the graph
struct Node {
    int vertex;
    int weight;
};

// Comparator function for priority queue
struct CompareNode {
    bool operator()(const Node& n1, const Node& n2) {
        return n1.weight > n2.weight; // Min-heap based on weight
    }
};

// Function to perform Prim's algorithm for Minimum Spanning Tree
vector<vector<pair<int, int>>> primMST(vector<vector<pair<int, int>>>& graph) {
    int V = graph.size();
    vector<bool> visited(V, false);
    vector<int> parent(V, -1);
    vector<int> key(V, INF);

    // Priority queue to select the next edge to include in MST
    priority_queue<Node, vector<Node>, CompareNode> pq;

    // Start with vertex 0
    key[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        int u = pq.top().vertex;
        pq.pop();

        // Mark current node as visited
        visited[u] = true;

        // Update key values and enqueue adjacent vertices
        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (!visited[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
                pq.push({v, key[v]});
            }
        }
    }

    // Construct MST based on parent array
    vector<vector<pair<int, int>>> MST(V);
    for (int i = 1; i < V; ++i) {
        int u = parent[i];
        int v = i;
        int weight = key[i];
        MST[u].push_back({v, weight});
        MST[v].push_back({u, weight});
    }

    return MST;
}

int main() {
    int V, E;
    cout << "Enter the number of vertices and edges: ";
    cin >> V >> E;

    // Initialize graph as adjacency list
    vector<vector<pair<int, int>>> graph(V);

    cout << "Enter the source, destination, and weight of each edge:" << endl;
    for (int i = 0; i < E; ++i) {
        int src, dest, weight;
        cin >> src >> dest >> weight;
        graph[src].push_back({dest, weight});
        graph[dest].push_back({src, weight}); // Undirected graph
    }

    // Compute MST using Prim's algorithm
    vector<vector<pair<int, int>>> MST = primMST(graph);

    // Output MST
    cout << "Edges in the Minimum Spanning Tree:" << endl;
    for (int u = 0; u < MST.size(); ++u) {
        for (const auto& edge : MST[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (u < v) // Print each edge only once
                cout << u << " - " << v << " : " << weight << endl;
        }
    }

    return 0;
}
