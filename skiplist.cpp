#include <bits/stdc++.h>
using namespace std;
template<typename Key>
class SkipList {
private:
    struct Node {
        Key key;
        vector<Node*> forward;
        Node(Key k, int level) : key(k), forward(level, nullptr) {}
    };
    Node* head;
    int maxLevel;
    int currentLevel;
    int getMaxLevel() const {
        return maxLevel;
    }
public:
    SkipList(int maxLevel) : maxLevel(maxLevel), currentLevel(0) {
        head = new Node(numeric_limits<Key>::min(), maxLevel);
    }
    ~SkipList() {
        Node* current = head;
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }
    void insert(const Key& key) {
        vector<Node*> update(maxLevel, nullptr);
        Node* current = head;
        for (int i = currentLevel; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (!current || current->key != key) {
            int newLevel = determineLevel();
            if (newLevel > currentLevel) {
                for (int i = currentLevel + 1; i <= newLevel; ++i)
                    update[i] = head;
                currentLevel = newLevel;
            }
            Node* newNode = new Node(key, newLevel + 1);
            for (int i = 0; i <= newLevel; ++i) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }
    bool search(const Key& key) const {
        Node* current = head;
        for (int i = currentLevel; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->key < key) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        return current && current->key == key;
    }
    void erase(const Key& key) {
        vector<Node*> update(maxLevel, nullptr);
        Node* current = head;
        for (int i = currentLevel; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (current && current->key == key) {
            for (int i = 0; i <= currentLevel; ++i) {
                if (update[i]->forward[i] != current)
                    break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            while (currentLevel > 0 && head->forward[currentLevel] == nullptr)
                --currentLevel;
        }
    }
    void print() const {
        for (int i = currentLevel; i >= 0; --i){
            Node* current = head->forward[i];
            cout << "Level " << i << ": ";
            while (current) {
                cout << current->key << " ";
                current = current->forward[i];
            }
            cout << endl;
        }
    }
private:
    int determineLevel() const {
        static int level = 0;
        return (level++ % maxLevel);
    }
};
int main() {
    SkipList<int> skiplist(4);
    int choice, value;
    cout << "Menu:\n";
    cout << "1. Insert\n";
    cout << "2. Search\n";
    cout << "3. Delete\n";
    cout << "4. Print\n";
    cout << "5. Exit\n";
    while (true) {
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                cin >> value;
                skiplist.insert(value);
                break;
            case 2:
                cout << "Enter value to search: ";
                cin >> value;
                if (skiplist.search(value))
                    cout << "Value found in the skiplist.\n";
                else 
                    cout << "Value not found in the skiplist.\n";
                break;
            case 3:
                cout << "Enter value to delete: ";
                cin >> value;
                skiplist.erase(value);
                break;
            case 4:
                skiplist.print();break;
            case 5:
                cout << "Exiting...\n";return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}