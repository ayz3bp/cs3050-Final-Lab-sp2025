#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// Define an edge in the adjacency list
typedef struct Edge {
    int to;
    int weight;
    struct Edge* next;
} Edge;

// Define a graph structure
typedef struct Graph {
    int V;          // Number of vertices
    Edge** adjList; // Adjacency list
} Graph;

// Create a new graph with V vertices
Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->adjList = (Edge**)malloc(V * sizeof(Edge*));
    
    // Initialize adjacency lists as empty
    for (int i = 0; i < V; i++) {
        graph->adjList[i] = NULL;
    }
    
    return graph;
}

// Add an edge to the graph (undirected)
void addEdge(Graph* graph, int src, int dest, int weight) {
    // Add edge from src to dest
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->to = dest;
    newEdge->weight = weight;
    newEdge->next = graph->adjList[src];
    graph->adjList[src] = newEdge;
    
    // Add edge from dest to src (since the graph is undirected)
    newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->to = src;
    newEdge->weight = weight;
    newEdge->next = graph->adjList[dest];
    graph->adjList[dest] = newEdge;
}

// Print the graph adjacency list
void printGraph(Graph* graph) {
    for (int v = 0; v < graph->V; v++) {
        Edge* temp = graph->adjList[v];
        printf("Vertex %d: ", v);
        while (temp) {
            printf("-> %d (%d) ", temp->to, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Find if an edge exists in the MST
bool edgeExists(Graph* mst, int src, int dest) {
    Edge* temp = mst->adjList[src];
    while (temp) {
        if (temp->to == dest) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// Structure to represent a new edge for consideration
typedef struct {
    int src;
    int dest;
    int weight;
} NewEdge;

// Function to update MST when a new vertex and its incident edges are added
Graph* updateMST(Graph* mst, int newVertex, NewEdge* newEdges, int numNewEdges) {
    // Create a new MST with one more vertex
    Graph* newMST = createGraph(mst->V + 1);
    
    // Copy the existing MST
    for (int v = 0; v < mst->V; v++) {
        Edge* temp = mst->adjList[v];
        while (temp) {
            if (v < temp->to) { // Avoid adding edges twice
                addEdge(newMST, v, temp->to, temp->weight);
            }
            temp = temp->next;
        }
    }
    
    // Special case: If the new vertex has no edges
    if (numNewEdges == 0) {
        // New vertex is isolated, just add it to the MST (no edges)
        return newMST;
    }
    
    // Find the minimum weight edge connecting the new vertex to the existing MST
    int minWeight = INT_MAX;
    int minEdgeIndex = -1;
    
    for (int i = 0; i < numNewEdges; i++) {
        if (newEdges[i].src == newVertex && newEdges[i].dest < mst->V && newEdges[i].weight < minWeight) {
            minWeight = newEdges[i].weight;
            minEdgeIndex = i;
        }
    }
    
    // Special case: If the new vertex is not connected to any existing vertex
    if (minEdgeIndex == -1) {
        // New vertex is disconnected from existing MST
        return newMST;
    }
    
    // Add the minimum weight edge to the MST
    addEdge(newMST, newVertex, newEdges[minEdgeIndex].dest, newEdges[minEdgeIndex].weight);
    
    return newMST;
}
