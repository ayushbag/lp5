#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Class representing an undirected graph using adjacency list
class Graph {
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Function to add an undirected edge
    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v); 
    }

    // --- PARALLEL BREADTH-FIRST SEARCH ---
    void parallelBFS(int startNode) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startNode] = true;
        q.push(startNode);

        while (!q.empty()) {
            int qSize = q.size(); // Get number of nodes at the current level

            // Process all nodes at the current level in parallel
            #pragma omp parallel for
            for (int i = 0; i < qSize; i++) {
                int u;

                // Critical section to safely pop from the shared queue
                #pragma omp critical
                {
                    u = q.front();
                    q.pop();
                    cout << u << " "; 
                }

                // Explore all neighbors of the current node
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        // Critical section to safely check and update visited array & queue
                        #pragma omp critical
                        {
                            if (!visited[v]) { // Double-check inside critical section
                                visited[v] = true;
                                q.push(v);
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    // --- PARALLEL DEPTH-FIRST SEARCH ---
    // Recursive utility function for DFS tasks
    void parallelDFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";

        for (int v : adj[u]) {
            if (!visited[v]) {
                // Create a new parallel task for each unvisited neighbor
                #pragma omp task
                parallelDFSUtil(v, visited);
            }
        }
    }

    void parallelDFS(int startNode) {
        vector<bool> visited(V, false);

        // Create a parallel region
        #pragma omp parallel
        {
            // Ensure the initial DFS call is executed by only one thread
            #pragma omp single
            {
                parallelDFSUtil(startNode, visited);
            }
        }
        cout << endl;
    }
};

int main() {
    // Creating an undirected tree graph
    //        0
    //       / \
    //      1   2
    //     / \   \
    //    3   4   5
    
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    cout << "Tree Structure:" << endl;
    cout << "       0     " << endl;
    cout << "      / \\    " << endl;
    cout << "     1   2   " << endl;
    cout << "    / \\   \\  " << endl;
    cout << "   3   4   5 \n" << endl;

    cout << "Parallel BFS Traversal starting from node 0:" << endl;
    g.parallelBFS(0);
    
    cout << "\nParallel DFS Traversal starting from node 0:" << endl;
    g.parallelDFS(0);

    return 0;
}