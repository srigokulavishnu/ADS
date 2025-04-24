#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

bool bfs(const vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent) {
    int V = residualGraph.size();
    vector<bool> visited(V, false);
    queue<int> q;

    visited[source] = true;
    q.push(source);

 
    while (!q.empty()) {
        int u = q.front();
        q.pop();

     
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                visited[v] = true;
                parent[v] = u;

              
                if (v == sink) return true;
            }
        }
    }

    return false;
}


int fordFulkerson(vector<vector<int>>& graph, int source, int sink) {
    int V = graph.size();
    vector<vector<int>> residualGraph = graph; 
    vector<int> parent(V); 
    int maxFlow = 0;

    
    while (bfs(residualGraph, source, sink, parent)) {
    
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

   
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

     
        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main() {
    int V, E;

    /
    cout << "Enter number of vertices: ";
    cin >> V;

    cout << "Enter number of edges: ";
    cin >> E;

    vector<vector<int>> graph(V, vector<int>(V, 0));

  
    cout << "Enter the edges (u, v, capacity):\n";
    for (int i = 0; i < E; i++) {
        int u, v, capacity;
        cout << "Edge " << i + 1 << ": ";
        cin >> u >> v >> capacity;
        graph[u][v] = capacity;
    }

    int source, sink;
    cout << "Enter source vertex: ";
    cin >> source;

    cout << "Enter sink vertex: ";
    cin >> sink;

    int maxFlow = fordFulkerson(graph, source, sink);

    cout << "The maximum possible flow is " << maxFlow << endl;

    return 0;
}
