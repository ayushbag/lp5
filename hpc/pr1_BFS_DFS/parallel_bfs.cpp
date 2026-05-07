#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>

using namespace std;

void parallelBFS(vector<vector<int>>& graph, int start) {

    int n = graph.size();

    vector<bool> visited(n, false);

    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {

        int node = q.front();
        q.pop();

        cout << node << " ";

        #pragma omp parallel for
        for(int i=0; i < graph[node].size(); i++) {
            
            int neighbour = graph[node][i];

            if(!visited[neighbour]) {
                #pragma omp critical
                {
                    if(!visited[neighbour]) {
                        visited[neighbour] = true;
                        q.push(neighbour);
                    }
                }
            }
        }
    }
}

void parallelBFS1 (vector<vector<int>>& graph, int start) {
    int n = graph.size();

    vector<bool> visited(n, false);

    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {

        int node = q.front();
        q.pop();

        cout << node << " ";

        #pragma omp parallel for
        for(int i=0; i<graph[node].size(); i++) {
            int neighbour = graph[node][i];

            if(!visited[neighbour]) {
                #pragma omp critical
                {
                    if(!visited[neighbour]) {
                        visited[neighbour] = true;
                        q.push(neighbour);
                    }
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

    cout << "Parallel BFS Traversal" << endl;

    parallelBFS1(graph, 0);
    
    return 0;
}