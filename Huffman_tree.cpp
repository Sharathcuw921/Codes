#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

struct Node {
    char data;
    int freq;
    Node *left, *right;
    Node(char data, int freq) {
        left = right = nullptr;
        this->data = data;
        this->freq = freq;
    }
};

struct compare {
    bool operator()(Node* l, Node* r) {
        return (l->freq > r->freq);
    }
};

void printCodes(Node* root, string str, unordered_map<char, string> &huffmanCode) {
    if (!root)
        return;

    if (!root->left && !root->right) {
        huffmanCode[root->data] = str;
    }

    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

void HuffmanCodes(char S[], int f[], int N) {
    Node *left, *right, *top;

    priority_queue<Node*, vector<Node*>, compare> minHeap;
    for (int i = 0; i < N; ++i)
        minHeap.push(new Node(S[i], f[i]));

    while (minHeap.size() != 1) {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();

        top = new Node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    unordered_map<char, string> huffmanCode;
    printCodes(minHeap.top(), "", huffmanCode);

    for (int i = 0; i < N; ++i) {
        cout << huffmanCode[S[i]] << " ";
    }
    cout << endl;

    for (const auto &pair : huffmanCode) {
        cout << pair.first << " : " << pair.second << endl;
    }
}

int main() {
    string S;
    cout << "Enter characters: ";
    cin >> S;
    int N = S.length();

    int f[N];
    cout << "Enter frequencies: ";
    for (int i = 0; i < N; ++i) {
        cin >> f[i];
    }

    HuffmanCodes(&S[0], f, N);

    return 0;
}
