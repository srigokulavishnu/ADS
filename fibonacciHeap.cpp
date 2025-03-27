#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <limits.h>

using namespace std;

// Node class for Fibonacci Heap
class FibonacciNode {
public:
    int key, degree;
    bool mark;
    FibonacciNode *parent, *child, *left, *right;

    FibonacciNode(int value) : key(value), degree(0), mark(false), parent(nullptr), child(nullptr) {
        left = right = this; // Circular doubly linked list
    }
};

// Fibonacci Heap class
class FibonacciHeap {
private:
    FibonacciNode* minNode;
    int nNodes;
    unordered_map<int, FibonacciNode*> nodeMap;  // To track inserted nodes

    void consolidate();
    void cut(FibonacciNode* x, FibonacciNode* y);
    void cascadingCut(FibonacciNode* y);

public:
    FibonacciHeap();
    void insert(int key);
    FibonacciNode* findMin();
    FibonacciNode* extractMin();
    void decreaseKey(int oldKey, int newKey);
    void deleteNode(int key);
    void meld(FibonacciHeap* otherHeap);
    void printHeap();
};

FibonacciHeap::FibonacciHeap() : minNode(nullptr), nNodes(0) {}

void FibonacciHeap::insert(int key) {
    FibonacciNode* newNode = new FibonacciNode(key);
    nodeMap[key] = newNode;

    if (!minNode) {
        minNode = newNode;
    } else {
        newNode->right = minNode->right;
        newNode->left = minNode;
        minNode->right->left = newNode;
        minNode->right = newNode;

        if (key < minNode->key) {
            minNode = newNode;
        }
    }
    nNodes++;
}

FibonacciNode* FibonacciHeap::findMin() {
    return minNode;
}

FibonacciNode* FibonacciHeap::extractMin() {
    if (!minNode) {
        cout << "Heap is empty!" << endl;
        return nullptr;
    }

    FibonacciNode* z = minNode;
    int minKey = z->key;

    if (z->child) {
        FibonacciNode* child = z->child;
        do {
            FibonacciNode* next = child->right;
            child->left = minNode;
            child->right = minNode->right;
            minNode->right->left = child;
            minNode->right = child;
            child->parent = nullptr;
            child = next;
        } while (child != z->child);
    }

    if (z->right == z) {
        minNode = nullptr;
    } else {
        minNode->left->right = minNode->right;
        minNode->right->left = minNode->left;
        minNode = minNode->right;
        consolidate();
    }

    nodeMap.erase(minKey);
    delete z;
    nNodes--;
    return minNode;
}

void FibonacciHeap::meld(FibonacciHeap* otherHeap) {
    if (!otherHeap || !otherHeap->minNode) return;
    if (!minNode) {
        minNode = otherHeap->minNode;
    } else {
        FibonacciNode* thisRight = minNode->right;
        FibonacciNode* otherLeft = otherHeap->minNode->left;

        minNode->right = otherHeap->minNode;
        otherHeap->minNode->left = minNode;
        thisRight->left = otherLeft;
        otherLeft->right = thisRight;

        if (otherHeap->minNode->key < minNode->key) {
            minNode = otherHeap->minNode;
        }
    }

    FibonacciNode* start = otherHeap->minNode;
    if (start) {
        FibonacciNode* curr = start;
        do {
            nodeMap[curr->key] = curr;
            curr = curr->right;
        } while (curr != start);
    }
    nNodes += otherHeap->nNodes;
    otherHeap->minNode = nullptr;
    otherHeap->nNodes = 0;
}

void FibonacciHeap::printHeap() {
    if (!minNode) {
        cout << "Heap is empty." << endl;
        return;
    }
    cout << "Root List: ";
    FibonacciNode* start = minNode;
    FibonacciNode* temp = start;
    do {
        cout << temp->key << " ";
        temp = temp->right;
    } while (temp != start);
    cout << endl;
}

// Menu-driven program
int main() {
    FibonacciHeap heap;
    int choice, value;

    while (true) {
        cout << "\n===== Fibonacci Heap Menu =====" << endl;
        cout << "1. Insert\n2. Find Min\n3. Extract Min\n4. Print Heap\n5. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                cin >> value;
                heap.insert(value);
                break;
            case 2:
                if (heap.findMin())
                    cout << "Min Element: " << heap.findMin()->key << endl;
                else
                    cout << "Heap is empty." << endl;
                break;
            case 3:
                heap.extractMin();
                break;
            case 4:
                heap.printHeap();
                break;
            case 5:
                return 0;
        }
    }
    return 0;
}
