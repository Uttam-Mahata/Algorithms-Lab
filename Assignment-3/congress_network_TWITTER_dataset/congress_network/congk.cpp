#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <queue>

using namespace std;

class DisjointSet {
private:
    vector<int> parent;
    vector<int> rank;

public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int u) {
        if (parent[u] != u)
            parent[u] = find(parent[u]);
        return parent[u];
    }

    void merge(int u, int v) {
        int pu = find(u);
        int pv = find(v);

        if (pu != pv) {
            if (rank[pu] < rank[pv])
                parent[pu] = pv;
            else if (rank[pu] > rank[pv])
                parent[pv] = pu;
            else {
                parent[pv] = pu;
                rank[pu]++;
            }
        }
    }
};

class Graph {
public:
    vector<pair<double, pair<int, int>>> edges;
    map<int, vector<pair<int, double>>> adjList;

    void addEdge(int src, int dest, double weight) {
        edges.push_back({weight, {src, dest}});
        adjList[src].push_back({dest, weight});
        adjList[dest].push_back({src, weight});
    }

    void connectedComponents() {
        unordered_set<int> visited;
        vector<unordered_set<int>> components;

        for (const auto& kvp : adjList) {
            int node = kvp.first;
            if (visited.find(node) == visited.end()) {
                unordered_set<int> component;
                dfs(node, visited, component);
                components.push_back(component);
            }
        }

        saveConnectedComponents(components);

        cout << "Number of connected components: " << components.size() << endl;
        int componentNumber = 1;
        for (const auto& component : components) {
            cout << "Component " << componentNumber++ << ": ";
            for (int node : component) {
                cout << node << " ";
            }
            cout << endl;
        }
    }

    void dfs(int node, unordered_set<int>& visited, unordered_set<int>& component) {
        visited.insert(node);
        component.insert(node);
        for (const auto& neighbor : adjList[node]) {
            if (visited.find(neighbor.first) == visited.end()) {
                dfs(neighbor.first, visited, component);
            }
        }
    }

    void minimumSpanningTree() {
        sort(edges.begin(), edges.end());
        int n = adjList.size();
        DisjointSet ds(n);

        double totalWeight = 0.0;
        vector<pair<int, int>> mstEdges;
        for (const auto& edge : edges) {
            double weight = edge.first;
            int u = edge.second.first;
            int v = edge.second.second;

            int pu = ds.find(u);
            int pv = ds.find(v);

            if (pu != pv) {
                ds.merge(pu, pv);
                mstEdges.push_back({u, v});
                totalWeight += weight;
            }
        }

        saveMST(mstEdges);

        cout << "Total weight of MST: " << totalWeight << endl;
        cout << "Minimum Spanning Tree Edges: " << endl;
        for (const auto& edge : mstEdges) {
            cout << edge.first << " - " << edge.second << endl;
        }
    }

    void saveConnectedComponents(const vector<unordered_set<int>>& components) {
        ofstream outfile("connected_components_kruskal.txt");
        if (outfile.is_open()) {
            for (const auto& component : components) {
                for (int node : component) {
                    outfile << node << " ";
                }
                outfile << "\n";
            }
            outfile.close();
            cout << "Connected components data saved to connected_components_kruskal.txt\n";
        } else {
            cout << "Unable to save connected components data to file\n";
        }
    }

    void saveMST(const vector<pair<int, int>>& mstEdges) {
        ofstream outfile("minimum_spanning_tree_kruskal.txt");
        if (outfile.is_open()) {
            for (const auto& edge : mstEdges) {
                outfile << edge.first << " - " << edge.second << "\n";
            }
            outfile.close();
            cout << "Minimum spanning tree data saved to minimum_spanning_tree_kruskal.txt\n";
        } else {
            cout << "Unable to save minimum spanning tree data to file\n";
        }
    }
};

int main() {
    Graph graph;
    ifstream infile("congress.txt");
    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            istringstream iss(line);
            int src, dest;
            double weight;
            if (iss >> src >> dest >> weight) {
                graph.addEdge(src, dest, weight);
            }
        }
        infile.close();
    } else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    cout << "Connected Components:" << endl;
    graph.connectedComponents();

    cout << "\nMinimum Spanning Tree:" << endl;
    graph.minimumSpanningTree();

    return 0;
}

// draw graph in python from minimum_spanning_tree_kruskal.txt
// import networkx as nx
// import matplotlib.pyplot as plt
//
// G = nx.Graph()
// with open('minimum_spanning_tree_kruskal.txt') as f:
//     for line in f:
//         u, v = map(int, line.strip().split(' - '))
//         G.add_edge(u, v)
//
// nx.draw(G, with_labels=True, node_size=1000, node_color='skyblue', font_size=10, font_weight='bold')
// plt.show()

