#include <iostream>
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

// Traverse Huffman tree to decode encoded data
string decodeHuffman(Node* root, const string& encodedData) {
    string decodedData = "";
    Node* current = root;

    for (char bit : encodedData) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        // If leaf node is reached, append its data to decoded data and reset current node to root
        if (current->left == nullptr && current->right == nullptr) {
            decodedData += current->data;
            current = root;
        }
    }

    return decodedData;
}

// Function to delete the tree nodes recursively
void deleteTree(Node* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

int main() {
    // Example: Frequency of characters
    unordered_map<char, int> frequencies = {
        {'a', 5},
        {'b', 9},
        {'c', 12},
        {'d', 13},
        {'e', 16},
        {'f', 45}
    };

    // Build Huffman tree
    Node* root = buildHuffmanTree(frequencies);

    // Example: Encoded data
    string encodedData = "101100011101010010";

    // Decode encoded data using Huffman tree
    string decodedData = decodeHuffman(root, encodedData);

    cout << "Decoded Data: " << decodedData << endl;

    // Clean up memory
    deleteTree(root);

    return 0;
}
