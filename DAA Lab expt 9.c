//Name : Vijyant
//Roll no. : 25/DA/063
#include <stdio.h>
#include <stdbool.h>

#define INF 99999   // Define a large value to represent infinity
#define V 5         // Number of vertices in the graph

// Function to find the vertex with the minimum distance value, from
// the set of vertices not yet included in the shortest path tree
int minDistance(int dist[], bool sptSet[]) {
    int min = INF, min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == false && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Function to print the constructed distance array
void printSolution(int dist[]) {
    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF) {
            printf("%d \t INF\n", i);
        } else {
            printf("%d \t\t %d\n", i, dist[i]);
        }
    }
}

// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using an adjacency matrix
void dijkstra(int graph[V][V], int src) {
    int dist[V];      // dist[i] will hold the shortest distance from src to i
    bool sptSet[V];   // sptSet[i] will be true if vertex i is included in the
                      // shortest path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITY and sptSet[] as false
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        sptSet[i] = false;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++) {
            // Update dist[v] only if:
            // 1. It is not in sptSet.
            // 2. There is an edge from u to v.
            // 3. Total weight of path from src to v through u is smaller than current value of dist[v].
            if (!sptSet[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Print the calculated shortest distances
    printSolution(dist);
}

int main() {
    /* Example Graph representation using an Adjacency Matrix
       0 means no direct edge between the vertices */
    int graph[V][V] = {
        {0, 4, 2, 0, 0},
        {4, 0, 1, 5, 0},
        {2, 1, 0, 8, 10},
        {0, 5, 8, 0, 2},
        {0, 0, 10, 2, 0}
    };

    int source = 0; // Starting vertex
    printf("Source Vertex: %d\n\n", source);
    
    dijkstra(graph, source);

    return 0;
}
