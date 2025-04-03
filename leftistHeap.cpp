

#include <iostream>
#include <algorithm>
using namespace std;
template <typename T>
struct Node {
    T key;
    Node* left;
    Node* right;
    int dist;
    Node(T k) : key(k), left(nullptr), right(nullptr), dist(1) {}
};
template <typename T>
Node<T>* merge(Node<T>* h1, Node<T>* h2) {
    if (!h1) return h2;
    if (!h2) return h1;
    if (h1->key > h2->key)
        swap(h1, h2);
    h1->right = merge(h1->right, h2);
    if (!h1->left || h1->left->dist < h1->right->dist) 
        swap(h1->left, h1->right);
    h1->dist = (h1->right ? h1->right->dist : 0) + 1;
    return h1;
}
template <typename T>
Node<T>* insert(Node<T>* root, T key) {
    Node<T>* newNode = new Node<T>(key);
    return merge(root, newNode);
}
template <typename T>
Node<T>* findMin(Node<T>* root) {
    return root;
}
template <typename T>
Node<T>* deleteMin(Node<T>* root) {
    if (!root) return nullptr;
    Node<T>* newRoot = merge(root->left, root->right);
    delete root;
    return newRoot;
}
template <typename T>
void inorder(Node<T>* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->key << " ";
    inorder(root->right);
}
int main() {
    Node<int>* root = nullptr;
    int choice, value;
    cout << "\nMenu:\n";
    cout << "1. Insert Element\n";
    cout << "2. Delete Minimum Element\n";
    cout << "3. Display Heap\n";
    cout << "4. Find min element\n";
    cout << "5. Exit\n";
    do {
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: 
                cout << "Enter the value to insert: ";
                cin >> value;
                root = insert(root, value);
                break;
            case 2:
                if (root) {
                    root = deleteMin(root);
                    cout << "Minimum element has been deleted\n";
                } else
                    cout << "Heap is empty. Cannot delete\n";
                break;
            case 3:
                cout << "Heap elements: ";
                if (root)
                    inorder(root);
                else
                    cout << "Heap is empty\n";
                cout << endl;break;
            case 4:
                if (root){
                    Node<int>* minNode = findMin(root);
                    cout << "Minimum element is "<<minNode->key<<endl;
                }
                else
                    cout << "Heap is empty\n";
                break;
            case 5:
                cout << "Exiting...\n";break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 5);
    return 0;
}
