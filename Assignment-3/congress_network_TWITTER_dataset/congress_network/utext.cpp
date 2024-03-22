#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

using namespace std;

// Define the adjacency list using unordered_map<int, vector<pair<int, double>>>
unordered_map<int, vector<pair<int, double>>> adjacency_list;

// Function to add an undirected edge to the adjacency list
void addUndirectedEdge(int src, int dest, double weight) {
    adjacency_list[src].push_back(make_pair(dest, weight));
    adjacency_list[dest].push_back(make_pair(src, weight));
}

int main() {
    // Open the text file
    ifstream infile("congress.txt");

    // Check if the file opened successfully
    if (!infile.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return 1; // Exit with error
    }

    // Read the file line by line
    string line;
    while (getline(infile, line)) {
        // Use stringstream to extract data from the line
        stringstream ss(line);
        int src, dest;
        char open_brace, comma;
        string weight_label;
        double weight;
        // Extract src, dest, and weight from the line
        if (!(ss >> src >> dest >> open_brace >> weight_label >> comma >> weight)) {
            cerr << "Error: Invalid format in line: " << line << endl;
            continue; // Skip invalid lines
        }
        // Add the edge to the adjacency list (undirected)
        addUndirectedEdge(src, dest, weight);
    }

    // Close the file
    infile.close();

    // Print the adjacency list
    cout << "Adjacency List:" << endl;
    for (const auto& entry : adjacency_list) {
        cout << entry.first << " -> ";
        for (const auto& edge : entry.second) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
    }

    return 0;
}
