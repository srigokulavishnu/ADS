#include <bits/stdc++.h>
using namespace std;
struct TreapNode {
    int key, priority;
    TreapNode *left, *right;
    TreapNode(int key, int priority) {
        this->key = key;
        this->priority = priority;
        left = right = nullptr;
    }
};
TreapNode* rotateRight(TreapNode* y) {
    TreapNode* x = y->left;
    TreapNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
}
TreapNode* rotateLeft(TreapNode* x) {
    TreapNode* y = x->right;
    TreapNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
}
TreapNode* insert(TreapNode* root, int key, int priority) {
    if (!root) return new TreapNode(key, priority);
    if (key < root->key) {
        root->left = insert(root->left, key, priority);
        if (root->left->priority > root->priority)
            root = rotateRight(root);
    } else {
        root->right = insert(root->right, key, priority);
        if (root->right->priority > root->priority)
            root = rotateLeft(root);
    }
    return root;
}
TreapNode* deleteNode(TreapNode* root, int key) {
    if (!root) return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if (!root->left) return root->right;
        else if (!root->right) return root->left;
        if (root->left->priority > root->right->priority) {
            root = rotateRight(root);
            root->right = deleteNode(root->right, key);
        } else {
            root = rotateLeft(root);
            root->left = deleteNode(root->left, key);
        }
    }
    return root;
}
void inorder(TreapNode* root) {
    if (root) {
        inorder(root->left);
        cout << "(Key: " << root->key << ", Priority: " << root->priority << ") ";
        inorder(root->right);
    }
}
int main() {
    TreapNode* root = nullptr;
    int choice, key, priority;
  cout << "\nTreap Menu:\n";
  cout << "1. Insert\n2. Delete\n3. Display Inorder\n4. Exit\n";
    while(true)
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter key to insert: ";
                cin >> key;
                cout << "Enter priority: ";
                cin >> priority;
                root = insert(root, key, priority);break;
            case 2:
                cout << "Enter key to delete: ";
                cin >> key;
                root = deleteNode(root, key);break;
            case 3:
                cout << "Inorder Traversal: ";
                inorder(root);
                cout << endl;break;
            case 4:
                cout << "Exiting...\n";break;
            default:
                cout << "Invalid choice!\n";
        }
    }  
    return 0;
}
