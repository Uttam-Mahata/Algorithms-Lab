#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent an edge in the graph
struct Edge {
    int src, dest, weight;
};

// Structure to represent a disjoint set
struct DisjointSet {
    vector<int> parent, rank;

    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n);

        // Initialize each set as a singleton set with rank 0
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    // Find the parent of a node (with path compression)
    int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union two sets by rank
    void unionSets(int u, int v) {
        int uRoot = find(u);
        int vRoot = find(v);

        if (rank[uRoot] < rank[vRoot])
            parent[uRoot] = vRoot;
        else if (rank[uRoot] > rank[vRoot])
            parent[vRoot] = uRoot;
        else {
            parent[vRoot] = uRoot;
            rank[uRoot]++;
        }
    }
};

// Comparator function to sort edges by weight
bool compareEdges(const Edge& e1, const Edge& e2) {
    return e1.weight < e2.weight;
}

// Kruskal's algorithm to find Minimum Spanning Tree
vector<Edge> kruskalMST(vector<Edge>& edges, int V) {
    // Sort edges by weight
    sort(edges.begin(), edges.end(), compareEdges);

    DisjointSet ds(V);
    vector<Edge> MST;

    for (const auto& edge : edges) {
        int u = edge.src;
        int v = edge.dest;

        int uRoot = ds.find(u);
        int vRoot = ds.find(v);

        // If including this edge does not cause a cycle, include it in the MST
        if (uRoot != vRoot) {
            MST.push_back(edge);
            ds.unionSets(uRoot, vRoot);
        }
    }

    return MST;
}

int main() {
    int V, E;
    cout << "Enter the number of vertices and edges: ";
    cin >> V >> E;

    vector<Edge> edges(E);
    cout << "Enter the source, destination, and weight of each edge:" << endl;
    for (int i = 0; i < E; ++i) {
        cin >> edges[i].src >> edges[i].dest >> edges[i].weight;
    }

    vector<Edge> MST = kruskalMST(edges, V);

    cout << "Edges in the Minimum Spanning Tree:" << endl;
    for (const auto& edge : MST) {
        cout << edge.src << " - " << edge.dest << " : " << edge.weight << endl;
    }

    return 0;
}
