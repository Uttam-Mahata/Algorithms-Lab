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

// Decode text using Huffman codes
string decodeText(const string& encodedText, Node* root) {
    string decodedText = "";
    Node* current = root;

    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current->left == nullptr && current->right == nullptr) {
            // Found a leaf node, append its character to decoded text
            decodedText += current->data;
            // Reset current node to root for next character decoding
            current = root;
        }
    }

    return decodedText;
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
    ofstream encodedFile("encoded.txt");
    encodedFile << encodedText;
    encodedFile.close();

    // Decode text using Huffman codes
    string decodedText = decodeText(encodedText, root);

    // Write decoded text to file
    ofstream decodedFile("decoded.txt");
    decodedFile << decodedText;
    decodedFile.close();

    return 0;
}
