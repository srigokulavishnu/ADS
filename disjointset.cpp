#include <iostream>
#include <vector>
using namespace std;
class Edge {
public:
    int u, v;
    int index;
};
class Graph {
public:
    int vertices, edges;
    vector<Edge> edge;
};
Graph* constructGraph(int vertices, int edges) {
    Graph* graph = new Graph();
    graph->vertices = vertices;
    graph->edges = edges;
    return graph;
}
int find(int parent[], int u) {
    if (parent[u] != u)
        parent[u] = find(parent, parent[u]);
    return parent[u];
}
void unionNodes(int parent[], int rank[], int u, int v) {
    int rootU = find(parent, u);
    int rootV = find(parent, v);
    if (rootU == rootV) return;
    if (rank[rootU] < rank[rootV])
        parent[rootU] = rootV;
    else if (rank[rootU] > rank[rootV])
        parent[rootV] = rootU;
    else {
        parent[rootV] = rootU;
        rank[rootU]++;
    }
}
pair<bool, Edge> checkCycle(Graph* graph) {
    int* parent = new int[graph->vertices];
    int* rank = new int[graph->vertices]();
    for (int i = 0; i < graph->vertices; i++)
        parent[i] = i;
    for (Edge e : graph->edge) {
        int rootU = find(parent, e.u);
        int rootV = find(parent, e.v);
        if (rootU == rootV) {
            delete[] parent;
            delete[] rank;
            return {true, e}; // Return the cycle-forming edge
        }   
        unionNodes(parent, rank, e.u, e.v);
    }   
    delete[] parent;
    delete[] rank;
    return {false, {-1, -1, -1}}; // No cycle found
}
int main() {
    int vertices, edges;
    cout << "Enter the number of vertices: ";
    cin >> vertices;
    cout << "Enter the number of edges: ";
    cin >> edges;
    Graph* graph = constructGraph(vertices, edges);
    if(edges>0)
    	cout << "Enter edges (u v) one by one:" << endl;
    for (int i = 0; i < edges; i++) {
        Edge e;
        cout << "Edge " << i + 1 << ": ";
        cin >> e.u >> e.v;
        e.index = i;
        graph->edge.push_back(e);
    }
    auto result = checkCycle(graph);
    if (result.first) {
        cout << "Cycle exists in the graph!" << endl;
        cout << "To break the cycle, exclude edge between " << result.second.u << " and " << result.second.v << endl;
    }else
        cout << "No cycle detected in the graph." << endl;   
    delete graph;
    return 0;
}
