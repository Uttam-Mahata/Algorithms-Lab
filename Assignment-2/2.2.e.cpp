#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// Node structure for the Huffman tree
struct Node {
    char data;
    int freq;
    Node* left;
    Node* right;

    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// Comparison function for priority queue
struct CompareNodes {
    bool operator()(Node* left, Node* right) const {
        return left->freq > right->freq;
    }
};

// Function to build Huffman tree
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, CompareNodes> minHeap;

    // Create a leaf node for each character and add it to the minHeap
    for (auto& pair : freqMap) {
        Node* newNode = new Node(pair.first, pair.second);
        minHeap.push(newNode);
    }

    // Build the Huffman tree
    while (minHeap.size() > 1) {
        Node* left = minHeap.top();
        minHeap.pop();
        Node* right = minHeap.top();
        minHeap.pop();

        Node* parent = new Node('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        minHeap.push(parent);
    }

    // Return the root of the Huffman tree
    return minHeap.top();
}

// Function to generate Huffman codes
void generateHuffmanCodes(Node* root, const string& code, unordered_map<char, string>& huffmanCodes) {
    if (!root)
        return;

    if (root->data != '$') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Function to compress text using Huffman codes
string compressText(const string& text, const unordered_map<char, string>& huffmanCodes) {
    string compressedText;
    for (char ch : text) {
        if (huffmanCodes.find(ch) != huffmanCodes.end()) {
            compressedText += huffmanCodes.at(ch);
        } else {
            cerr << "Huffman code not found for character: " << ch << endl;
            // Handle error or continue without compressing this character
        }
    }
    return compressedText;
}

// Function to calculate compression ratio
double calculateCompressionRatio(int originalSize, int compressedSize) {
    return static_cast<double>(originalSize) / compressedSize;
}

int main() {
    // Read and concatenate all known documents into a single large document
    string combinedDocument;
    ifstream knownFile("document.txt");
    if (knownFile.is_open()) {
        string line;
        while (getline(knownFile, line)) {
            combinedDocument += line;
        }
        knownFile.close();
    } else {
        cerr << "Unable to open file known_document.txt" << endl;
        return 1;
    }

    // Calculate frequency of each character in the combined document
    unordered_map<char, int> freqMap;
    for (char ch : combinedDocument) {
        freqMap[ch]++;
    }

    // Build Huffman tree
    Node* root = buildHuffmanTree(freqMap);

    // Generate Huffman codes
    unordered_map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // Read the unknown document
    string unknownDocument;
    ifstream unknownFile("unknown_document.txt");
    if (unknownFile.is_open()) {
        string line;
        while (getline(unknownFile, line)) {
            unknownDocument += line;
        }
        unknownFile.close();
    } else {
        cerr << "Unable to open file unknown_document.txt" << endl;
        return 1;
    }

    // Compress the unknown document using Huffman codes
    string compressedUnknownDocument = compressText(unknownDocument, huffmanCodes);

    // Save the compressed unknown document to file
    ofstream compressedUnknownFile("compressed_unknown_document.txt");
    if (compressedUnknownFile.is_open()) {
        compressedUnknownFile << compressedUnknownDocument;
        compressedUnknownFile.close();
    } else {
        cerr << "Unable to open file compressed_unknown_document.txt" << endl;
        return 1;
    }

    // Calculate compression ratio
    int originalSize = unknownDocument.size() * 8; // Assuming 8 bits per character
    int compressedSize = compressedUnknownDocument.size();
    double compressionRatio = calculateCompressionRatio(originalSize, compressedSize);

    // Output compression ratio
    cout << "Compression Ratio: " << compressionRatio << endl;

    cout << "Compression process completed successfully." << endl;

    return 0;
}
