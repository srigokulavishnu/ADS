#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <limits.h>

using namespace std;

// Node structure for Fibonacci Heap
struct FibonacciNode {
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

    FibonacciNode* extractMin() {
        if (!minNode) {
            cout << "Heap is empty!" << endl;
            return nullptr;
        }

        FibonacciNode* z = minNode;
        int minKey = z->key;  // Store min value before deletion

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

        cout << "Extracted Min: " << minKey << endl;  // Now safely printing minKey
        return minNode;
    }

    void decreaseKey(int oldKey, int newKey) {
        if (nodeMap.find(oldKey) == nodeMap.end()) {
            cout << "Key not found!" << endl;
            return;
        }
        FibonacciNode* x = nodeMap[oldKey];

        if (newKey > x->key) {
            cout << "New key is greater than current key!" << endl;
            return;
        }

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

    void deleteNode(int key) {
        decreaseKey(key, INT_MIN);
        extractMin();
    }

  void meld(FibonacciHeap* otherHeap) {
    if (!otherHeap || !otherHeap->minNode) return;

    if (!minNode) {
        minNode = otherHeap->minNode;  // If current heap is empty, just take the new heap
    } else {
        // Merge the circular doubly linked lists
        FibonacciNode* thisRight = minNode->right;
        FibonacciNode* otherLeft = otherHeap->minNode->left;

        minNode->right = otherHeap->minNode;
        otherHeap->minNode->left = minNode;

        thisRight->left = otherLeft;
        otherLeft->right = thisRight;

        // Update min pointer if necessary
        if (otherHeap->minNode->key < minNode->key) {
            minNode = otherHeap->minNode;
        }
    }

    // ✅ Step 1: Update nodeMap to include nodes from otherHeap
    FibonacciNode* start = otherHeap->minNode;
    if (start) {
        FibonacciNode* curr = start;
        do {
            nodeMap[curr->key] = curr;  // Store reference in nodeMap
            curr = curr->right;
        } while (curr != start);
    }

    // ✅ Step 2: Update node count
    nNodes += otherHeap->nNodes;

    // ✅ Step 3: Clear the other heap (since all nodes are now merged)
    otherHeap->minNode = nullptr;
    otherHeap->nNodes = 0;
}

void printHeap() {
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

};

// Menu-driven program
int main() {
    FibonacciHeap heap;
    int choice, value, oldValue, newValue;

    while (true) {
        cout << "\n===== Fibonacci Heap Menu =====" << endl;
        cout << "1. Insert\n2. Find Min\n3. Extract Min\n4. Meld (Merge with New Heap)\n5. Decrease Key\n6. Delete Node\n7. Print Heap\n8. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                cin >> value;
                heap.insert(value);
                break;

            case 2:
                cout << "Min Element: " << heap.findMin()->key << endl;
                break;

            case 3:
                heap.extractMin();
                break;

           case 4: {
    FibonacciHeap otherHeap;
    int numValues, tempValue;

    cout << "Enter number of values for new heap: ";
    cin >> numValues;
    
    cout << "Enter values: ";
    for (int i = 0; i < numValues; i++) {
        cin >> tempValue;
        otherHeap.insert(tempValue);
    }

    heap.meld(&otherHeap);
    cout << "Heaps merged successfully!" << endl;
    break;
}


            case 5:
                cout << "Enter current key and new key: ";
                cin >> oldValue >> newValue;
                heap.decreaseKey(oldValue, newValue);
                break;

            case 6:
                cout << "Enter key to delete: ";
                cin >> value;
                heap.deleteNode(value);
                break;

            case 7:
                heap.printHeap();
                break;

            case 8:
                return 0;
        }
    }
    return 0;
}

