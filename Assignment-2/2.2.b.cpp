// Created: 14th March 2024 10:27 PM
// By: Uttam

#include <iostream>
#include <queue>
#include <map>
#include <string>

using namespace std;

// Node structure for Huffman tree
struct HuffmanNode {
    char data;                  // Character stored in the node
    int frequency;              // Frequency of the character
    HuffmanNode *left, *right;  // Pointers to left and right children

    // Constructor
    HuffmanNode(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparator for the priority queue (min heap)
struct CompareNodes {
    bool operator()(HuffmanNode* const& lhs, HuffmanNode* const& rhs) {
        return lhs->frequency > rhs->frequency;
    }
};

// Traverse the Huffman tree and assign codes to characters
void assignHuffmanCodes(HuffmanNode* root, string code, map<char, string>& huffmanCodes) {
    if (root) {
        if (!root->left && !root->right) {
            huffmanCodes[root->data] = code;
        }
        assignHuffmanCodes(root->left, code + "0", huffmanCodes);
        assignHuffmanCodes(root->right, code + "1", huffmanCodes);
    }
}

// Build Huffman tree and return the root
HuffmanNode* buildHuffmanTree(map<char, int>& frequencies) {
    // Priority queue to store nodes (min heap)
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> pq;

    // Create nodes for each character and add them to the priority queue
    for (const auto& pair : frequencies) {
        HuffmanNode* newNode = new HuffmanNode(pair.first, pair.second);
        pq.push(newNode);
    }

    // Build Huffman tree by repeatedly merging two nodes with the lowest frequencies
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        // Create a new internal node with sum of frequencies
        HuffmanNode* internalNode = new HuffmanNode('$', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        // Add the new internal node back to the priority queue
        pq.push(internalNode);
    }

    // Return the root of the Huffman tree
    return pq.top();
}

// Encode the input text using Huffman codes
string huffmanEncode(string text, map<char, string>& huffmanCodes) {
    string encodedText;
    for (char c : text) {
        encodedText += huffmanCodes[c];
    }
    return encodedText;
}

int main() {
    string inputText;

    // Take inputText from user
    cout << "Enter the input text: ";
    getline(cin, inputText);


    // Step 1: Build frequency table
    map<char, int> frequencies;
    for (char c : inputText) {
        frequencies[c]++;
    }

    // Step 2: Build Huffman tree
    HuffmanNode* root = buildHuffmanTree(frequencies);

    // Step 3: Assign Huffman codes
    map<char, string> huffmanCodes;
    assignHuffmanCodes(root, "", huffmanCodes);

    // Step 4: Encode input text using Huffman codes
    string encodedText = huffmanEncode(inputText, huffmanCodes);

    // Display the Huffman codes
    cout << "Huffman Codes:\n";
    for (const auto& pair : huffmanCodes) {
        cout << "'" << pair.first << "' : " << pair.second << endl;
    }

    // Display the encoded text
    cout << "\nEncoded Text: " << encodedText << endl;

    return 0;
}
