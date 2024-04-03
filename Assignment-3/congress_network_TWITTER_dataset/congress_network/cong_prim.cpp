#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <map>

using namespace std;

class Graph {
public:
    map<int, vector<pair<int, double>>> adjList;

    void addEdge(int src, int dest, double weight) {
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
        priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<pair<double, pair<int, int>>>> pq;
        unordered_set<int> visited;
        int startNode = adjList.begin()->first;
        visited.insert(startNode);
        for (const auto& neighbor : adjList[startNode]) {
            pq.push({neighbor.second, {startNode, neighbor.first}});
        }

        double totalWeight = 0.0;
        vector<pair<int, int>> mstEdges;
        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            double weight = top.first;
            int from = top.second.first;
            int to = top.second.second;

            if (visited.find(to) != visited.end()) {
                continue;
            }

            visited.insert(to);
            mstEdges.push_back({from, to});
            totalWeight += weight;

            for (const auto& neighbor : adjList[to]) {
                if (visited.find(neighbor.first) == visited.end()) {
                    pq.push({neighbor.second, {to, neighbor.first}});
                }
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
        ofstream outfile("connected_components_prim.txt");
        if (outfile.is_open()) {
            for (const auto& component : components) {
                for (int node : component) {
                    outfile << node << " ";
                }
                outfile << "\n";
            }
            outfile.close();
            cout << "Connected components data saved to connected_components.txt\n";
        } else {
            cout << "Unable to save connected components data to file\n";
        }
    }

    void saveMST(const vector<pair<int, int>>& mstEdges) {
        ofstream outfile("minimum_spanning_tree.txt");
        if (outfile.is_open()) {
            for (const auto& edge : mstEdges) {
                outfile << edge.first << " - " << edge.second << "\n";
            }
            outfile.close();
            cout << "Minimum spanning tree data saved to minimum_spanning_tree.txt\n";
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
