#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>
#include <chrono>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

void dfs(int node, bool parallel = false) {
    stack<int> s;
    s.push(node);

    auto start = chrono::high_resolution_clock::now(); // Start the timer

    while (!s.empty()) {
        int curr_node = s.top();
        s.pop();

        if (!visited[curr_node]) {
            visited[curr_node] = true;

            cout << curr_node << " ";

            if (parallel) {
                #pragma omp parallel for
                for (int i = 0; i < graph[curr_node].size(); i++) {
                    int adj_node = graph[curr_node][i];
                    if (!visited[adj_node]) {
                        s.push(adj_node);
                    }
                }
            } else {
                for (int i = 0; i < graph[curr_node].size(); i++) {
                    int adj_node = graph[curr_node][i];
                    if (!visited[adj_node]) {
                        s.push(adj_node);
                    }
                }
            }
        }
    }

    auto end = chrono::high_resolution_clock::now(); // End the timer
    chrono::duration<double> duration = end - start; // Calculate the duration

    // Print the duration
    if (parallel) {
        cout << endl << "Time taken by Parallel DFS: " << duration.count() << " seconds" << endl;
    } else {
        cout << endl << "Time taken by Sequential DFS traversal: " << duration.count() << " seconds" << endl;
    }
}

int main() {
    int n, m, start_node;
    cout << "Enter No of Node, Edges, and start node:" ;
    cin >> n >> m >> start_node;

    cout << "Enter Pair of edges:" ;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Sequential DFS traversal
    cout << "Sequential DFS traversal:";
    dfs(start_node);

    // Reset visited array
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Parallel DFS traversal
    cout << "Parallel DFS traversal:";
    dfs(start_node, true);

    return 0;
}

