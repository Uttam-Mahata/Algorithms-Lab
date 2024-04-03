#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class DisjointSet {
private:
    vector<int> parent, rank;

public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int x) {
        if (x != parent[x])
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return;

        if (rank[rootX] < rank[rootY])
            parent[rootX] = rootY;
        else if (rank[rootX] > rank[rootY])
            parent[rootY] = rootX;
        else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
};

struct Edge {
    int src, dest;
    double weight;

    Edge(int s, int d, double w) : src(s), dest(d), weight(w) {}
};

class Graph {
private:
    int V;
    vector<Edge> edges;

public:
    Graph(int vertices) : V(vertices) {}

    void addEdge(int src, int dest, double weight) {
        edges.emplace_back(src, dest, weight);
    }

    void sortEdgesByWeight() {
        sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        });
    }

    vector<Edge> kruskalMST() {
        DisjointSet ds(V);
        vector<Edge> MST;
        double totalWeight = 0.0;

        sortEdgesByWeight();

        for (const auto& edge : edges) {
            int src = edge.src;
            int dest = edge.dest;

            int srcRoot = ds.find(src);
            int destRoot = ds.find(dest);

            if (srcRoot != destRoot) {
                MST.push_back(edge);
                totalWeight += edge.weight;
                ds.unionSets(srcRoot, destRoot);
            }
        }

        cout << "Total weight of MST: " << totalWeight << endl;
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

    // Kruskal's MST
    cout << "Kruskal's Minimum Spanning Tree:" << endl;
    vector<Edge> MST = g.kruskalMST();
    for (const auto& edge : MST) {
        cout << edge.src << " - " << edge.dest << " : " << edge.weight << endl;
    }

    return 0;
}
