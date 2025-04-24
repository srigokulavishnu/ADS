#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Graph {
public:
    int V;
    vector<vector<int>> dist;

    Graph(int V) {
        this->V = V;
        dist = vector<vector<int>>(V, vector<int>(V, INT_MAX));
        
        for (int i = 0; i < V; i++) {
            dist[i][i] = 0;
        }
    }

    void addEdge(int u, int v, int weight) {
        dist[u][v] = weight;
    }

    void FloydWarshall() {
        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }

        cout << "Shortest distances between every pair of vertices: " << endl;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][j] == INT_MAX) {
                    cout << "INF ";
                } else {
                    cout << dist[i][j] << "   ";
                }
            }
            cout << endl;
        }
    }
};

int main() {
    int V, E;

    cout << "Enter the number of vertices: ";
    cin >> V;

    cout << "Enter the number of edges: ";
    cin >> E;

    Graph g(V);

    cout << "Enter the edges in the format: <u> <v> <weight>" << endl;
    for (int i = 0; i < E; i++) {
        int u, v, weight;
        cout << "Edge " << i + 1 << ": ";
        cin >> u >> v >> weight;
        g.addEdge(u, v, weight);
    }

    g.FloydWarshall();

    return 0;
}

