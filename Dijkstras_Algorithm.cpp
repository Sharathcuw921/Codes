#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

const int INF = numeric_limits<int>::max(); 

vector<int> dijkstra(int start, const vector<vector<pair<int, int>>>& graph, vector<int>& parent) {
    int n = graph.size();
    vector<int> dist(n, INF); 
    parent.assign(n, -1); 
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // min-heap priority queue

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (const auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

vector<int> get_path(int end, const vector<int>& parent) {
    vector<int> path;
    for (int v = end; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}

void display_graph(const vector<vector<pair<int, int>>>& graph) {
    cout << "Graph Representation (Adjacency List):\n";
    for (int i = 0; i < graph.size(); ++i) {
        cout << "Vertex " << i << ":";
        for (const auto& edge : graph[i]) {
            cout << " -> (" << edge.first << ", " << edge.second << ")";
        }
        cout << endl;
    }
}

int main() {
    int vertices = 6; 

    vector<vector<pair<int, int>>> graph(vertices);

    graph[0].emplace_back(1, 10);
    graph[0].emplace_back(4, 3);
    graph[1].emplace_back(0, 10);
    graph[1].emplace_back(2, 2);
    graph[1].emplace_back(4, 4);
    graph[2].emplace_back(1, 2);
    graph[2].emplace_back(3, 9);
    graph[3].emplace_back(2, 9);
    graph[3].emplace_back(4, 7);
    graph[4].emplace_back(0, 3);
    graph[4].emplace_back(1, 4);
    graph[4].emplace_back(3, 7);

    display_graph(graph);

    int start, end;
    cout << "Enter the starting vertex: ";
    cin >> start;
    cout << "Enter the ending vertex: ";
    cin >> end;

    if (start < 0 || start >= vertices || end < 0 || end >= vertices) {
        cout << "Invalid vertex. Please run the program again with valid vertices." << endl;
        return 1;
    }

    vector<int> parent(vertices);
    vector<int> dist = dijkstra(start, graph, parent);
    vector<int> path = get_path(end, parent);

    cout << "Shortest path from " << start << " to " << end << " is:\n";
    for (int v : path) {
        cout << v << " ";
    }
    cout << "\nDistance: " << dist[end] << endl;

    return 0;
}
