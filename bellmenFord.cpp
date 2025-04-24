#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Edge {
public:
    int u, v, weight;

    Edge(int u, int v, int weight) {
        this->u = u;
        this->v = v;
        this->weight = weight;
    }
};

class Graph {
public:
    int V, E;
    vector<Edge> edges;

    Graph(int V, int E) {
        this->V = V;
        this->E = E;
    }

    void addEdge(int u, int v, int weight) {
        edges.push_back(Edge(u, v, weight));
    }

    void BellmanFord(int src) {
        vector<int> dist(V, INT_MAX);
        dist[src] = 0;

        for (int i = 1; i <= V - 1; i++) {
            for (const auto& edge : edges) {
                if (dist[edge.u] != INT_MAX && dist[edge.u] + edge.weight < dist[edge.v]) {
                    dist[edge.v] = dist[edge.u] + edge.weight;
                }
            }
        }

        for (const auto& edge : edges) {
            if (dist[edge.u] != INT_MAX && dist[edge.u] + edge.weight < dist[edge.v]) {
                cout << "Graph contains negative weight cycle" << endl;
                return;
            }
        }

        cout << "Vertex Distance from Source " << src << endl;
        for (int i = 0; i < V; i++) {
            if (dist[i] == INT_MAX) {
                cout << "Vertex " << i << ": INF" << endl;
            } else {
                cout << "Vertex " << i << ": " << dist[i] << endl;
            }
        }
    }
};

int main() {
    int V, E;

    cout << "Enter the number of vertices: ";
    cin >> V;
    cout << "Enter the number of edges: ";
    cin >> E;

    Graph g(V, E);

    cout << "Enter the edges (u, v, weight): " << endl;
    for (int i = 0; i < E; i++) {
        int u, v, weight;
        cin >> u >> v >> weight;
        g.addEdge(u, v, weight);
    }

    int src;
    cout << "Enter the source vertex: ";
    cin >> src;

    g.BellmanFord(src);

    return 0;
}

