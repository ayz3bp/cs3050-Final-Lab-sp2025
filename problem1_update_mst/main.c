#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Define an edge
typedef struct Edge {
    int to;
    int weight;
    struct Edge* next;
} Edge;

// Add edge to adjacency list
void addEdge(Edge** adjList, int from, int to, int weight) {
    Edge* newEdge = malloc(sizeof(Edge));
    newEdge->to = to;
    newEdge->weight = weight;
    newEdge->next = adjList[from];
    adjList[from] = newEdge;
}

// Find the best edge from new vertex to MST
int findBestEdge(int mstSize, Edge** adjList, int newVertex, int* toNode, int* bestWeight) {
    // Initialize best weight to maximum possible value
    *bestWeight = INT_MAX;
    *toNode = -1;
    
    // Look through all edges from the new vertex
    Edge* curr = adjList[newVertex];
    while (curr != NULL) {
        // Check if this edge connects to a vertex in the MST and has smaller weight
        if (curr->to < mstSize && curr->weight < *bestWeight) {
            *bestWeight = curr->weight;
            *toNode = curr->to;
        }
        curr = curr->next;
    }
    
    // Return true if we found an edge to the MST
    return *toNode != -1;
}

int main() {
    // Example: MST with 3 vertices
    int mstSize = 3;
    int newVertex = 3;
    Edge* adjList[10] = {NULL}; // Combined adjacency list for all vertices
    
    // Existing MST edges
    addEdge(adjList, 0, 1, 1);
    addEdge(adjList, 1, 0, 1);
    addEdge(adjList, 1, 2, 2);
    addEdge(adjList, 2, 1, 2);
    
    // Add edges for the new vertex
    addEdge(adjList, newVertex, 0, 4); // edge from 3 to 0 with weight 4
    addEdge(adjList, newVertex, 1, 3); // edge from 3 to 1 with weight 3
    addEdge(adjList, newVertex, 2, 6); // edge from 3 to 2 with weight 6
    
    // Find minimal edge to connect new vertex to MST
    int toNode, weight;
    if (findBestEdge(mstSize, adjList, newVertex, &toNode, &weight)) {
        printf("Connecting new vertex %d to MST using edge (%d - %d) with weight %d\n", 
               newVertex, toNode, newVertex, weight);
        
        // Add the edge in both directions to maintain undirected graph property
        addEdge(adjList, toNode, newVertex, weight);
        
        // Increment MST size as we've added a new vertex
        mstSize++;
        
        // Print updated MST
        printf("\nUpdated MST:\n");
        for (int i = 0; i < mstSize; i++) {
            printf("Vertex %d:", i);
            Edge* curr = adjList[i];
            while (curr != NULL) {
                // Only print edges that are part of the MST
                // For this simple example, we know the MST edges
                if ((i == 0 && curr->to == 1) || 
                    (i == 1 && curr->to == 0) || 
                    (i == 1 && curr->to == 2) || 
                    (i == 2 && curr->to == 1) ||
                    (i == toNode && curr->to == newVertex) ||
                    (i == newVertex && curr->to == toNode)) {
                    printf(" -> (%d, %d)", curr->to, curr->weight);
                }
                curr = curr->next;
            }
            printf("\n");
        }
    } else {
        printf("New vertex is not connected to any node in MST.\n");
    }
    
    return 0;
}
