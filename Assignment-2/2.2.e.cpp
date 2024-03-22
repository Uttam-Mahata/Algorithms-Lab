#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

// Node structure for Huffman tree
struct Node {
    char data;
    int freq;
    Node *left, *right;

    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// Comparator for priority queue
struct CompareNodes {
    bool operator()(Node* const& n1, Node* const& n2) {
        return n1->freq > n2->freq;
    }
};

// Function to generate Huffman tree
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;

    for (auto& entry : freqMap) {
        pq.push(new Node(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* combined = new Node('$', left->freq + right->freq);
        combined->left = left;
        combined->right = right;
        pq.push(combined);
    }

    return pq.top();
}

// Recursive function to generate Huffman codes and save tree details
void generateCodesAndSaveDetails(Node* root, string code, unordered_map<char, string>& codes, ofstream& file) {
    if (!root) return;

    if (root->data != '$') {
        file << root->data << ":" << root->freq << ":" << code << endl;
        codes[root->data] = code;
    }

    generateCodesAndSaveDetails(root->left, code + "0", codes, file);
    generateCodesAndSaveDetails(root->right, code + "1", codes, file);
}

// Function to encode text using Huffman codes
string encodeText(const string& text, const unordered_map<char, string>& codes) {
    string encoded = "";
    for (char c : text) {
        encoded += codes.at(c);
    }
    return encoded;
}

// Function to calculate average code length
double calculateAverageCodeLength(const string& text, const unordered_map<char, string>& codes) {
    double totalBits = 0;
    for (char c : text) {
        totalBits += codes.at(c).length();
    }
    return totalBits / text.length();
}

int main() {
    // File paths
    string knownDocument1Path = "student1_answer.txt";
    string knownDocument2Path = "student2_answer.txt";
    string unknownDocumentPath = "student3_answer.txt";
    string huffmanTreeFile = "huffman_tree.txt";

    // Read known documents
    string knownDocument1, knownDocument2, unknownDocument;
    ifstream file;

    // Read known document 1
    file.open(knownDocument1Path);
    if (file.is_open()) {
        getline(file, knownDocument1);
        file.close();
    } else {
        cerr << "Error: Unable to open known document 1 file." << endl;
        return 1;
    }

    // Read known document 2
    file.open(knownDocument2Path);
    if (file.is_open()) {
        getline(file, knownDocument2);
        file.close();
    } else {
        cerr << "Error: Unable to open known document 2 file." << endl;
        return 1;
    }

    // Read unknown document
    file.open(unknownDocumentPath);
    if (file.is_open()) {
        getline(file, unknownDocument);
        file.close();
    } else {
        cerr << "Error: Unable to open unknown document file." << endl;
        return 1;
    }

    // Calculate frequencies
    unordered_map<char, int> freqMap;
    for (char c : knownDocument1 + knownDocument2 + unknownDocument) {
        freqMap[c]++;
    }

    // Build Huffman tree
    Node* root = buildHuffmanTree(freqMap);

    // Generate Huffman codes and save tree details to file
    ofstream huffmanTreeFileStream(huffmanTreeFile);
    if (huffmanTreeFileStream.is_open()) {
        unordered_map<char, string> codes;
        generateCodesAndSaveDetails(root, "", codes, huffmanTreeFileStream);
        huffmanTreeFileStream.close();

        // Calculate average code lengths
        double avgCodeLenDoc1 = calculateAverageCodeLength(knownDocument1, codes);
        double avgCodeLenDoc2 = calculateAverageCodeLength(knownDocument2, codes);
        double avgCodeLenUnknown = calculateAverageCodeLength(unknownDocument, codes);

        // Display results
        cout << "Average Code Length for Known Document 1: " << avgCodeLenDoc1 << endl;
        cout << "Average Code Length for Known Document 2: " << avgCodeLenDoc2 << endl;
        cout << "Average Code Length for Unknown Document: " << avgCodeLenUnknown << endl;

        // Check for optimality loss
        if (avgCodeLenUnknown > avgCodeLenDoc1 && avgCodeLenUnknown > avgCodeLenDoc2) {
            cout << "Optimality Loss: The average code length for the unknown document is longer than that of the known documents." << endl;
        } else {
            cout << "No Optimality Loss: The average code length for the unknown document is not longer than that of the known documents." << endl;
        }
    } else {
        cerr << "Error: Unable to open file to save Huffman tree details." << endl;
        return 1;
    }
   

    return 0;
    


}
