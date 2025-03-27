include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <limits.h>

using namespace std;

// Node structure for Fibonacci Heap
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
    unordered_map<int, FibonacciNode*> nodeMap;

    void consolidate() {
        if (!minNode) return;
        int maxDegree = log2(nNodes) + 1;
        vector<FibonacciNode*> degreeTable(maxDegree, nullptr);

        vector<FibonacciNode*> rootList;
        FibonacciNode* curr = minNode;
        do {
            rootList.push_back(curr);
            curr = curr->right;
        } while (curr != minNode);

        for (FibonacciNode* node : rootList) {
            int d = node->degree;
            while (degreeTable[d]) {
                FibonacciNode* other = degreeTable[d];
                if (node->key > other->key) swap(node, other);

                other->right->left = other->left;
                other->left->right = other->right;
                other->parent = node;
                other->right = other->left = other;

                if (!node->child) {
                    node->child = other;
                } else {
                    other->right = node->child->right;
                    other->left = node->child;
                    node->child->right->left = other;
                    node->child->right = other;
                }

                node->degree++;
                other->mark = false;
                degreeTable[d] = nullptr;
                d++;
            }
            degreeTable[d] = node;
        }

        minNode = nullptr;
        for (auto node : degreeTable) {
            if (node && (!minNode || node->key < minNode->key)) {
                minNode = node;
            }
        }
    }

    void cut(FibonacciNode* x, FibonacciNode* y) {
        if (x->right == x) {
            y->child = nullptr;
        } else {
            x->left->right = x->right;
            x->right->left = x->left;
            if (y->child == x) {
                y->child = x->right;
            }
        }
        y->degree--;

        x->parent = nullptr;
        x->mark = false;
        x->left = minNode;
        x->right = minNode->right;
        minNode->right->left = x;
        minNode->right = x;
    }

    void cascadingCut(FibonacciNode* y) {
        FibonacciNode* z = y->parent;
        if (z) {
            if (!y->mark) {
                y->mark = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

public:
    FibonacciHeap() : minNode(nullptr), nNodes(0) {}

    void insert(int key) {
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

    FibonacciNode* findMin() {
        return minNode;
    }

    FibonacciNode* deleteMin() {
        if (!minNode) return nullptr;

        FibonacciNode* z = minNode;
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
        nodeMap.erase(z->key);
        delete z;
        nNodes--;
        return minNode;
    }

    void decreaseKey(int oldKey, int newKey) {
        if (nodeMap.find(oldKey) == nodeMap.end()) return;
        FibonacciNode* x = nodeMap[oldKey];

        if (newKey > x->key) return;

        nodeMap.erase(oldKey);
        nodeMap[newKey] = x;
        x->key = newKey;
        FibonacciNode* y = x->parent;
        if (y && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->key < minNode->key) {
            minNode = x;
        }
    }

    void meld(FibonacciHeap& other) {
        if (!other.minNode) return;

        if (!minNode) {
            minNode = other.minNode;
        } else {
            FibonacciNode* temp = minNode->right;
            minNode->right = other.minNode->right;
            other.minNode->right->left = minNode;
            other.minNode->right = temp;
            temp->left = other.minNode;

            if (other.minNode->key < minNode->key) {
                minNode = other.minNode;
            }
        }
        nNodes += other.nNodes;
        other.minNode = nullptr;
        other.nNodes = 0;
    }
    void printHeap() {
    if (!minNode) {
        cout << "Heap is empty" << endl;
 return;
    }
    cout << "Root nodes: ";
    FibonacciNode* temp = minNode;
    do {
        cout << temp->key << " ";
        temp = temp->right;
    } while (temp != minNode);
    cout << endl;
}

};

int main() {
    FibonacciHeap heap, newHeap;
    int choice, value, oldValue, newValue;

    while (true) {
        cout << "1. Insert\n2. Find Min\n3. Delete Min\n4. Decrease Key\n5. Meld Heaps\n6.Print Heap\n7. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                cin >> value;
                heap.insert(value);
                break;
            case 2:
                cout << "Min: " << (heap.findMin() ? to_string(heap.findMin()->key) : "Heap is empty") << endl;
                break;
            case 3:
                heap.deleteMin();
                break;
            case 4:
                cout << "Enter current key and new key: ";
                cin >> oldValue >> newValue;
                heap.decreaseKey(oldValue, newValue);
                break;
            case 5:
                cout << "Enter values for new heap (-1 to stop): ";
                while (cin >> value && value != -1) newHeap.insert(value);
                heap.meld(newHeap);
                break;
            case 6:
                heap.printHeap();
                break;
            case 7:
                return 0;
        }
    }
}



