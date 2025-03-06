#include <iostream>
using namespace std;
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) {
        data = val;
        left = right = nullptr;
    }
};
class SplayTree {
private:
    Node* root;
    int cost;
    Node* rightRotate(Node* node) {
        Node* leftChild = node->left;
        node->left = leftChild->right;
        leftChild->right = node;
        cost++;
        return leftChild;
    }
    Node* leftRotate(Node* node) {
        Node* rightChild = node->right;
        node->right = rightChild->left;
        rightChild->left = node;
        cost++;
        return rightChild;
    }
    Node* splay(Node* root, int key) {
        if (!root || root->data == key)
            return root;
        if (root->data > key) {
            if (!root->left) 
                return root;
            if (root->left->data > key) {
                root->left->left = splay(root->left->left, key);
                root = rightRotate(root);
            } 
            else if (root->left->data < key) {
                root->left->right = splay(root->left->right, key);
                if (root->left->right) root->left = leftRotate(root->left);
            }
            return (root->left == nullptr) ? root : rightRotate(root);
        } 
        else {
            if (!root->right) return root;

            if (root->right->data < key) { 
                root->right->right = splay(root->right->right, key);
                root = leftRotate(root);
            } 
            else if (root->right->data > key) { 
                root->right->left = splay(root->right->left, key);
                if (root->right->left) root->right = rightRotate(root->right);
            }
            return (root->right == nullptr) ? root : leftRotate(root);
        }
    }
    Node* insert(Node* root, int key) {
        if (root == nullptr)
            return new Node(key);
        cost = 0;
        root = splay(root, key);
        if (root->data == key)
            return root;
        Node* newNode = new Node(key);
        if (root->data > key) {
            newNode->right = root;
            newNode->left = root->left;
            root->left = nullptr;
        } 
        else {
            newNode->left = root;
            newNode->right = root->right;
            root->right = nullptr;
        }
        return newNode;
    }
    Node* search(Node* root, int key) {
        return splay(root, key);
    }
    Node* deleteNode(Node* root, int key) {
        if (!root)
            return nullptr;
        root = splay(root, key);
        if (root->data != key)
            return root;
        if (!root->left) {
            Node* temp = root;
            root = root->right;
            delete temp;
        } else {
            Node* temp = root;
            root = splay(root->left, key);
            root->right = temp->right;
            delete temp;
        }
        return root;
    }
    void preorder(Node* root) {
        if (root != nullptr) {
            cout << root->data << " ";
            preorder(root->left);
            preorder(root->right);
        }
    }
public:
    SplayTree() : root(nullptr), cost(0) {}
    void insert(int key) {
        cost = 0;
        root = insert(root, key);
    }
    void search(int key) {
        cost = 0;
        root = search(root, key);
        if (root == nullptr || root->data != key)
            cout << "Key " << key << " not found!" << endl;
        else
            cout << "Key " << key << " found!" << endl;
    }
    void deleteNode(int key) {
        cost = 0;
        root = deleteNode(root, key);
    }
    void preorder() {
        preorder(root);
        cout << endl;
    }
    int getCost() {
        return cost;
    }
};
int main() {
    SplayTree tree;
    int choice, key;
    cout << "\nSplay Tree Operations Menu\n";
    cout << "1. Insert\n";
    cout << "2. Search\n";
    cout << "3. Display\n";
    cout << "4. Delete\n";
    cout << "5. Get Amortized Cost\n";
    cout << "6. Exit\n";
    while (true) {
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter key to insert: ";
                cin >> key;
                tree.insert(key);
                break;
            case 2:
                cout << "Enter key to search: ";
                cin >> key;
                tree.search(key);
                break;
            case 3:
                cout << "Tree elements in Preorder: ";
                tree.preorder();
                break;
            case 4:
                cout << "Enter key to delete: ";
                cin >> key;
                tree.deleteNode(key);
                break;
            case 5:
                cout << "Cost of last operation: " << tree.getCost() << endl;
                break;
            case 6:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}
