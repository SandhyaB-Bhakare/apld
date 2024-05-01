#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

void bfs(int start_node, bool parallel = false) {
    queue<int> q;
    q.push(start_node);

    auto start = chrono::high_resolution_clock::now(); // Start the timer

    while (!q.empty()) {
        int curr_node = q.front();
        q.pop();

        if (!visited[curr_node]) {
            visited[curr_node] = true;

            cout << curr_node << " ";

            if (parallel) {
                #pragma omp parallel for
                for (int i = 0; i < graph[curr_node].size(); i++) {
                    int adj_node = graph[curr_node][i];
                    if (!visited[adj_node]) {
                        q.push(adj_node);
                    }
                }
            } else {
                for (int i = 0; i < graph[curr_node].size(); i++) {
                    int adj_node = graph[curr_node][i];
                    if (!visited[adj_node]) {
                        q.push(adj_node);
                    }
                }
            }
        }
    }

    auto end = chrono::high_resolution_clock::now(); // End the timer
    chrono::duration<double> duration = end - start; // Calculate the duration

    // Print the duration
    if (parallel) {
        cout << endl << "Time taken by Parallel BFS: " << duration.count() << " seconds" << endl;
    } else {
        cout << endl << "Time taken by Sequential BFS traversal: " << duration.count() << " seconds" << endl;
    }
}

int main() {
    int n, m, start_node;
    cout << "Enter No of Nodes, Edges, and start node: ";
    cin >> n >> m >> start_node;

    cout << "Enter Pair of edges:" ;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Sequential BFS traversal
    cout << "Sequential BFS traversal:";
    bfs(start_node);

    // Reset visited array
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Parallel BFS traversal
    cout << "Parallel BFS traversal:";
    bfs(start_node, true);

    return 0;
}

