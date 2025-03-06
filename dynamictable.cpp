#include <iostream>
using namespace std;

class dynamicTable {
    int* table;
    int capacity;
    int size;
    int insertionCost;
    int resizingCost;

    void resize() {
        int newCapacity = capacity * 2;
        int* newTable = new int[newCapacity];
        for (int i = 0; i < size; i++) {
            newTable[i] = table[i];
        }
        delete[] table;
        table = newTable;
        resizingCost += size;
        capacity = newCapacity;
    }

public:
    dynamicTable() {
        capacity = 1;
        size = 0;
        table = new int[1];
        insertionCost = 0;
        resizingCost = 0;
    }

    ~dynamicTable() {
        delete[] table;
    }

    void insert(int val) {
        insertionCost++;
        if (size == capacity) {
            resize();
        }
        table[size++] = val;
    }

    void display() {
        for (int j = 0; j < size; j++) {
            cout << table[j] << " ";
        }
        cout << "\n";
    }

    int rcapacity() {
        return capacity;
    }

    int rsize() {
        return size;
    }

    int getInsertionCost() {
        return insertionCost;
    }

    int getResizingCost() {
        return resizingCost;
    }

    int getTotalCost() {
        return insertionCost + resizingCost;
    }

    double getAmortizedCost() {
        return static_cast<double>(getTotalCost()) / size;
    }
};

int main() {
    dynamicTable t;
    int n, val;

    cout << "Enter the number of elements: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "\nEnter the element: ";
        cin >> val;

        t.insert(val);
        cout << "\nAfter inserting " << val << ":";
        cout << "\nTable: ";
        t.display();
        cout << "Size: " << t.rsize();
        cout << "\nCapacity: " << t.rcapacity();
        cout << "\nInsertion Cost: " << t.getInsertionCost();
        cout << "\nResizing Cost: " << t.getResizingCost();
        cout << "\nTotal Cost: " << t.getTotalCost();
        cout << "\nAmortized Cost: " << t.getAmortizedCost() << "\n";
    }

    return 0;
}
