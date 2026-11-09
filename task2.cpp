#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;

    Node(int x) {
        data = x;
        next = NULL;}
};

Node* heap[1000];
int heapSize = 0;

//inserting into Min Heap
void insert(Node* node) {
    int i = heapSize++;
    heap[i] = node;

    while (i > 0) {
        int p = (i - 1) / 2;

        if (heap[p]->data <= heap[i]->data)
            break;

        swap(heap[p], heap[i]);
        i = p;
    }
}

//removing minimum element
Node* removeMin() {
    Node* minNode = heap[0];
    heap[0] = heap[--heapSize];

    int i = 0;

    while (true) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;

        if (l < heapSize && heap[l]->data < heap[smallest]->data)
            smallest = l;
        if (r < heapSize && heap[r]->data < heap[smallest]->data)
            smallest = r;
        if (smallest == i)
            break;
        swap(heap[i], heap[smallest]);
        i = smallest;
    }

    return minNode;
}

//merge K Sorted Linked Lists
Node* mergeKLists(Node* lists[], int k) {
    heapSize = 0;

    //insert first node of every list
    for (int i = 0; i < k; i++) {
        if (lists[i] != NULL)
            insert(lists[i]);
    }

    Node* dummy = new Node(0);
    Node* tail = dummy;
    while (heapSize > 0) {
        Node* minNode = removeMin();
        tail->next = minNode;
        tail = tail->next;

        if (minNode->next != NULL)
            insert(minNode->next);
    }

    tail->next = NULL;
    return dummy->next;
}

// Display the list
void display(Node* head) {
    while (head != NULL) {
        cout << head->data << " ";
        head = head->next;}
    cout << endl;
}

//creating linked list
Node* createList(int n) {
    Node* head = NULL;
    Node* tail = NULL;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;

        Node* newNode = new Node(x);

        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;}
    }
    return head;
}

int main() {
    int k;
    cout << "Enter number of linked lists: ";
    cin >> k;

    Node* lists[100];
    for (int i = 0; i < k; i++) {
        int n;
        cout << "Enter size of list " << i + 1 << ": ";
        cin >> n;
        cout << "Enter sorted elements: ";
        lists[i] = createList(n);
    }

    Node* result = mergeKLists(lists, k);
    cout << "\nMerged Sorted Linked List: ";
    display(result);

    return 0;
}