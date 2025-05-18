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
int findBestEdge(int mstSize, Edge* newEdges, int* toNode, int* bestWeight) {
    *bestWeight = INT_MAX;
    *toNode = -1;
    Edge* curr = newEdges;
    while (curr != NULL) {
        if (curr->to < mstSize && curr->weight < *bestWeight) {
            *bestWeight = curr->weight;
            *toNode = curr->to;
        }
        curr = curr->next;
    }
    return *toNode != -1;
}

int main() {
    // Example: MST with 3 vertices
    int mstSize = 3;
    Edge* mstAdjList[10] = {NULL};

    // Existing MST edges (example)
    addEdge(mstAdjList, 0, 1, 1);
    addEdge(mstAdjList, 1, 0, 1);
    addEdge(mstAdjList, 1, 2, 2);
    addEdge(mstAdjList, 2, 1, 2);

    // New vertex (vertex 3) and its edges
    Edge* newEdges = NULL;
    Edge* newAdjList[10] = {NULL}; // include new vertex
    addEdge(&newEdges, 0, 3, 4); // from 3 to 0
    addEdge(&newEdges, 1, 3, 3); // from 3 to 1
    addEdge(&newEdges, 2, 3, 6); // from 3 to 2

    // Find minimal edge to MST
    int toNode, weight;
    if (findBestEdge(mstSize, newEdges, &toNode, &weight)) {
        printf("Connecting new vertex 3 to MST using edge (%d - 3) with weight %d\n", toNode, weight);
        addEdge(mstAdjList, 3, toNode, weight);
        addEdge(mstAdjList, toNode, 3, weight);
        mstSize++; // updated MST size
    } else {
        printf("New vertex is not connected to any node in MST.\n");
    }

    // Print MST
    printf("Updated MST:\n");
    for (int i = 0; i < mstSize; i++) {
        printf("Vertex %d:", i);
        Edge* curr = mstAdjList[i];
        while (curr != NULL) {
            printf(" -> (%d, %d)", curr->to, curr->weight);
            curr = curr->next;
        }
        printf("\n");
    }

    return 0;
}
