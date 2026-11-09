#include <iostream>
#include <string>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = NULL;}
};

Node* heap[100];
int heapSize = 0;

//inserting into Min Heap
void insert(Node* node) {
    int i = heapSize++;
    heap[i] = node;

    while (i > 0) {
        int p = (i - 1) / 2;

        if (heap[p]->freq <= heap[i]->freq)
            break;
        swap(heap[p], heap[i]);
        i = p;
    }
}

//removing the minimum element
Node* removeMin() {
    Node* minNode = heap[0];
    heap[0] = heap[--heapSize];

    int i = 0;
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < heapSize &&
            heap[left]->freq < heap[smallest]->freq)
            smallest = left;
        if (right < heapSize &&
            heap[right]->freq < heap[smallest]->freq)
            smallest = right;
        if (smallest == i)
            break;

        swap(heap[i], heap[smallest]);
        i = smallest;
    }

    return minNode;}

//build the Huffman Tree
Node* buildHuffman(char ch[], int freq[], int n) {
    for (int i = 0; i < n; i++)
        insert(new Node(ch[i], freq[i]));

    //EDGE CASE: if only one character so give it code "0"
    if (heapSize == 1) {
        Node* only = removeMin();
        Node* root = new Node('$', only->freq);
        root->left = only;
        return root;
    }

    while (heapSize > 1) {
        Node* left = removeMin();
        Node* right = removeMin();
        Node* parent = new Node('$', left->freq + right->freq);

        parent->left = left;
        parent->right = right;
        insert(parent);
    }
    return removeMin();
}

//traversal (preorder) to print the codes
void printCodes(Node* root, string code) {
    if (root == NULL)
        return;

    if (root->left == NULL && root->right == NULL) {
        cout << root->ch << " : " << code << endl;
        return;
    }
    printCodes(root->left, code + "0");
    printCodes(root->right, code + "1");
}

int main() {
    int n;
    char ch[100];
    int freq[100];

    cout << "Enter number of characters: ";
    cin >> n;

    cout << "Enter characters: ";
    for (int i = 0; i < n; i++)
        cin >> ch[i];

    cout << "Enter frequencies: ";
    for (int i = 0; i < n; i++)
        cin >> freq[i];

    Node* root = buildHuffman(ch, freq, n);

    cout << "\nHuffman Codes:\n";
    printCodes(root, "");
    return 0;
}