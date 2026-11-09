#include <iostream>
#include <vector>
#include <limits>
using namespace std;

struct Node {
    int data, height, count;                       // count the times data occurs
    Node *left, *right;
    Node(int x) { data = x; height = 1; count = 1; left = right = NULL; }
};

int height(Node* n) { return n ? n->height : 0; }
int balanceOf(Node* n) { return n ? height(n->left) - height(n->right) : 0; }
void update(Node* n) { n->height = 1 + max(height(n->left), height(n->right)); }

Node* rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    update(y);
    update(x);
    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    update(x);
    update(y);
    return y;
}

// Rebalances a node (for after every insertion and deletion)
Node* rebalance(Node* root) {
    update(root);
    int b = balanceOf(root);

    if (b > 1) {                                   // left heavy
        if (balanceOf(root->left) < 0) {           // LR case
            cout << "   [Rebalance] Left-Right rotation at " << root->data << endl;
            root->left = leftRotate(root->left);
        } else
            cout << "   [Rebalance] Right (LL) rotation at " << root->data << endl;
        return rightRotate(root);
    }
    if (b < -1) {                                  // right heavy
        if (balanceOf(root->right) > 0) {          // RL case
            cout << "   [Rebalance] Right-Left rotation at " << root->data << endl;
            root->right = rightRotate(root->right);
        } else
            cout << "   [Rebalance] Left (RR) rotation at " << root->data << endl;
        return leftRotate(root);
    }
    return root;
}

Node* insert(Node* root, int key) {
    if (!root) return new Node(key);
    if (key < root->data) root->left = insert(root->left, key);
    else if (key > root->data) root->right = insert(root->right, key);
    else { root->count++; return root; }          // duplicates so just increase count
    return rebalance(root);
}

Node* deleteNode(Node* root, int key) {
    if (!root) return NULL;
    if (key < root->data) root->left = deleteNode(root->left, key);
    else if (key > root->data) root->right = deleteNode(root->right, key);
    else {
        if (root->count > 1) { root->count--; return root; }   // remove one occurrence only
        if (!root->left || !root->right) {         // 0 or 1 child
            Node* child = root->left ? root->left : root->right;
            delete root;
            return child;
        }
        Node* succ = root->right;                  // 2 children: inorder successor
        while (succ->left) succ = succ->left;
        root->data = succ->data;
        root->count = succ->count;
        succ->count = 1;                           // so the node is removed completely
        root->right = deleteNode(root->right, succ->data);
    }
    return rebalance(root);
}

void printTree(Node* root, int space = 0) {
    if (!root) return;
    space += 5;
    printTree(root->right, space);
    cout << string(space - 5, ' ') << root->data;
    if (root->count > 1) cout << "(x" << root->count << ")";
    cout << endl;
    printTree(root->left, space);
}

void getMax(Node* root, int k, vector<int>& v) {   // k largest
    if (!root || (int)v.size() >= k) return;
    getMax(root->right, k, v);
    for (int i = 0; i < root->count && (int)v.size() < k; i++) v.push_back(root->data);
    getMax(root->left, k, v);
}

void getMin(Node* root, int k, vector<int>& v) {   // k smallest
    if (!root || (int)v.size() >= k) return;
    getMin(root->left, k, v);
    for (int i = 0; i < root->count && (int)v.size() < k; i++) v.push_back(root->data);
    getMin(root->right, k, v);
}

vector<int> topK(Node* root, int k, int criteria) {
    vector<int> v;
    if (criteria == 2) getMin(root, k, v);
    else getMax(root, k, v);
    return v;
}

//asks again on invalid input
int readInt(const char* prompt, int lo, int hi) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= lo && x <= hi) return x;
        cout << "Invalid input, try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    Node* root = NULL;
    const int MIN_INT = numeric_limits<int>::min(), MAX_INT = numeric_limits<int>::max();

    // Insert multiple elements (rebalance + print after each)
    int n = readInt("Enter number of elements: ", 1, MAX_INT);
    for (int i = 0; i < n; i++) {
        int x = readInt("Enter element: ", MIN_INT, MAX_INT);
        cout << "Inserting " << x << endl;
        root = insert(root, x);
        cout << "Tree after insertion:\n";
        printTree(root);
        cout << "----------------\n";
    }

    // Query top k elements
    int k = readInt("\nEnter k: ", 1, MAX_INT);
    int c = readInt("Criteria (1 = Max, 2 = Min, 3 = Sum of k largest): ", 1, 3);

    vector<int> top = topK(root, k, c);
    int sum = 0;
    cout << "Top " << top.size() << " elements: ";
    for (int x : top) { cout << x << " "; sum += x; }
    if (c == 3) cout << "| Sum = " << sum;
    cout << endl;

    // Delete the top k elements (rebalance + print after each deletion)
    for (int x : top) {
        cout << "\nDeleting " << x << endl;
        root = deleteNode(root, x);
        cout << "Tree after deletion:\n";
        if (root) printTree(root); else cout << "(empty tree)\n";
        cout << "----------------\n";
    }
    return 0;
}