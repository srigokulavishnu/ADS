#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
class BinaryCounter { 
    vector<int> counter; 
    int bitFlippingCost; 
    int incrementCost;   
public:
    BinaryCounter(int n) {
        counter.resize(n, 0);
        bitFlippingCost = 0;
        incrementCost = 0;
    }
    void increment() {
        incrementCost++;
        int flips = 0;
        for (int i = 0; i < counter.size(); i++) {
            if (counter[i] == 0) {
                counter[i] = 1;
                flips++;
                break;
            } else {
                counter[i] = 0;
                flips++;
            }
        }
        bitFlippingCost += flips;
    }
    void displayCounter() {
        for (int i = counter.size() - 1; i >= 0; i--) {
            cout << counter[i];
        }
        cout << "\n";
    }
    int getBitFlippingCost() {
        return bitFlippingCost;
    }
    int getIncrementCost() {
        return incrementCost;
    }
    int getTotalCost() {
        return bitFlippingCost + incrementCost;
    }
    double getAmortizedCost(int operations) {
        return static_cast<double>(getTotalCost()) / operations;
    }
};

int main() {
    int n, operations;
    cout << "Enter the number of bits for the binary counter: ";
    cin >> n;
    cout << "Enter the number of increments to perform: ";
    cin >> operations;
    BinaryCounter counter(n);
    for (int i = 0; i < operations; i++) {
        cout<< "\nIncrement " << i + 1 << ": ";
        counter.increment();
        counter.displayCounter();
        cout << "Bit Flipping Cost: " << counter.getBitFlippingCost() << "\n";
        cout << "Increment Cost: " << counter.getIncrementCost() << "\n";
        cout << "Total Cost: " << counter.getTotalCost() << "\n";
        cout << "Amortized Cost: " << counter.getAmortizedCost(i + 1) << "\n";
    }
    return 0;
}
