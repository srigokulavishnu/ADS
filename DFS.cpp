#include <iostream>
#include <vector>
using namespace std;

class Graph {
public:
    int vertices;
    vector<vector<int>> adj;

    Graph(int v) {
        vertices = v;
        adj.resize(v);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);  // Add edge from u to v
        adj[v].push_back(u);  // Since the graph is undirected
    }

    void DFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";

        for (int v : adj[u]) {
            if (!visited[v])
                DFSUtil(v, visited);
        }
    }

    void DFS(int start) {
        vector<bool> visited(vertices, false);
        cout << "DFS starting from vertex " << start << ": ";
        DFSUtil(start, visited);
        cout << endl;
    }
};

int main() {
    int vertices, edges;
    cout << "Enter number of vertices: ";
    cin >> vertices;
    Graph g(vertices);

    cout << "Enter number of edges: ";
    cin >> edges;
    cout << "Enter edges (u v) one by one (0-based indexing):\n";
    for (int i = 0; i < edges; i++) {
        int u, v;
        cout << "Edge " << i + 1 << ": ";
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter starting vertex for DFS: ";
    cin >> start;
    g.DFS(start);

    return 0;
}
