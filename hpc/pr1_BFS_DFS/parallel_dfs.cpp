#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void parallelDFS(vector<vector<int>>& graph, int node, vector<bool>& visited) {
    visited[node] = true;
    
    cout << node << " ";

    #pragma omp parallel for
    for(int i=0; i<graph[node].size(); i++) {

        int neighbour = graph[node][i];

        if(!visited[neighbour]) {
            #pragma omp critical
            {
                if(!visited[neighbour]) {
                    #pragma omp task
                    parallelDFS(graph, neighbour, visited);
                }
            }
        }
    }
}

int main() {
    vector<vector<int>> graph = {
        {1, 2},
        {0, 3},
        {0, 3, 4},
        {1, 2},
        {2}
    };

    int n = graph.size();

    vector<bool> visited(n, false);

    cout << "Parallel DFS Traversal";

    parallelDFS(graph, 0, visited);

    return 0;
}