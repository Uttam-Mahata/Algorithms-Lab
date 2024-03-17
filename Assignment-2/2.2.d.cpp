#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

// Node structure for Huffman tree
struct Node {
    char data;
    int frequency;
    Node* left;
    Node* right;

    Node(char d, int freq) : data(d), frequency(freq), left(nullptr), right(nullptr) {}
};

// Comparator for priority queue
struct CompareNodes {
    bool operator()(Node* const& lhs, Node* const& rhs) const {
        return lhs->frequency > rhs->frequency;
    }
};

// Build Huffman tree
Node* buildHuffmanTree(const unordered_map<char, int>& frequencies) {
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;

    // Create leaf nodes for each character and add them to priority queue
    for (const auto& pair : frequencies) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Build Huffman tree by merging nodes
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node('$', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    return pq.top();
}

// Traverse Huffman tree to generate codes
void generateCodes(Node* root, const string& code, unordered_map<char, string>& codes) {
    if (root->left == nullptr && root->right == nullptr) {
        codes[root->data] = code;
        return;
    }
    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

// Encode text using Huffman codes
string encodeText(const string& text, const unordered_map<char, string>& codes) {
    string encodedText = "";
    for (char ch : text) {
        encodedText += codes.at(ch);
    }
    return encodedText;
}

int main() {
    // Read text document
    ifstream file("document.txt");
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Calculate character frequencies
    unordered_map<char, int> frequencies;
    for (char ch : text) {
        frequencies[ch]++;
    }

    // Build Huffman tree
    Node* root = buildHuffmanTree(frequencies);

    // Generate Huffman codes
    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    // Encode text using Huffman codes
    string encodedText = encodeText(text, codes);

    // Write encoded text to file
    ofstream outFile("encoded.txt");
    outFile << encodedText;
    outFile.close();

    // Calculate compression ratio
    double originalSize = text.size();
    // Write a csv file for Symbol, Frequency, Code
    ofstream csvFile("codes.csv");
    csvFile << "Symbol,Frequency,Code" << endl;
    for (const auto& pair : frequencies) {
        csvFile << pair.first << "," << pair.second << "," << codes[pair.first] << endl;
    }
    csvFile.close();

    
    double encodedSize = encodedText.size();
    double compressionRatio = originalSize / encodedSize;
    cout << "Compression Ratio: " << compressionRatio << endl;

   

    return 0;
}
