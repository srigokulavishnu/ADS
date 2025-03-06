#include<iostream>
using namespace std;
struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    Node(int k) {
        key = k;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};
int height(Node* N) {
    if (N == nullptr)
        return 0;
    return N->height;
}
void update(Node *N){
    N->height = 1 + max(height(N->left), height(N->right));
}
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    update(y);
    update(x);
    return x;
}
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    update(x);
    update(y);
    return y;
}
int getBalance(Node* N) {
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}
Node* insert(Node* node, int key) {
    if (node == nullptr)
        return new Node(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;
    update(node);
    int balance = getBalance(node);
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
Node* deleteNode(Node* root, int key) {
    if (root == nullptr)
        return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            Node* temp = root->right;
            while (temp->left != nullptr)
                temp = temp->left;
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    if (root == nullptr)
        return root;
    update(root);
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}
bool search(Node* root, int key) {
    if (root == nullptr)
        return false;
    if (root->key == key)
        return true;
    if (key < root->key)
        return search(root->left, key);
    return search(root->right, key);
}
void preOrder(Node* root) {
    if (root != nullptr) {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}
void printBalanceFactor(Node* node) {
    if (node != nullptr) {
        cout << "Balance Factor of node " << node->key << " : " << getBalance(node) << endl;
    }
}
int main() {
    Node* root = nullptr;
    int choice, value;
    Node* node = nullptr;
    cout << "\nMenu:\n";
    cout << "1. Insert\n";
    cout << "2. Delete\n";
    cout << "3. Search\n";
    cout << "4. Preorder Traversal\n";
    cout << "5. Print Balance Factor\n";
    cout << "6. Exit\n";
    while (true) {
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                cin >> value;
                root = insert(root, value);
                break;
            case 2:
                cout << "Enter value to delete: ";
                cin >> value;
                root = deleteNode(root, value);
                break;
            case 3:
                cout << "Enter value to search: ";
                cin >> value;
                if (search(root, value))
                    cout << "Value " << value << " found in the tree.\n";
                else
                    cout << "Value " << value << " not found in the tree.\n";
                break;
            case 4:
                cout << "Preorder traversal: ";
                preOrder(root);
                cout << endl;
                break;
            case 5:
                cout << "Enter node value to find balance factor: ";
                cin >> value;
                node = root;
                while (node != nullptr && node->key != value) {
                    if (value < node->key)
                        node = node->left;
                    else
                        node = node->right;
                }
                if (node != nullptr)
                    printBalanceFactor(node);
                else
                    cout << "Node with value " << value << " not found.\n";
                break;
            case 6:
                exit(0);
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}