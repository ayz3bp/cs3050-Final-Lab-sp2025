#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Graph representation
typedef struct Edge {
    int src;
    int dest;
    int weight;
} Edge;

typedef struct Graph {
    int V;          // Number of vertices
    int E;          // Number of edges
    Edge* edges;    // Array of edges
} Graph;

// Adjacency list representation
typedef struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
} AdjListNode;

typedef struct AdjList {
    AdjListNode* head;
} AdjList;

typedef struct AdjListGraph {
    int V;
    AdjList* array;
} AdjListGraph;

// Structure to store MST edge order
typedef struct EdgeOrder {
    int src;
    int dest;
    int weight;
} EdgeOrder;

// Create a graph with V vertices and E edges
Graph* createGraph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edges = (Edge*)malloc(E * sizeof(Edge));
    return graph;
}

// Create an adjacency list graph
AdjListGraph* createAdjListGraph(int V) {
    AdjListGraph* graph = (AdjListGraph*)malloc(sizeof(AdjListGraph));
    graph->V = V;
    graph->array = (AdjList*)malloc(V * sizeof(AdjList));
    
    for (int i = 0; i < V; i++)
        graph->array[i].head = NULL;
    
    return graph;
}

// Add an edge to the adjacency list graph
void addEdge(AdjListGraph* graph, int src, int dest, int weight) {
    // Add edge from src to dest
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    
    // Add edge from dest to src (undirected graph)
    newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = src;
    newNode->weight = weight;
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Convert adjacency list to edge array
Graph* adjListToEdgeArray(AdjListGraph* adjGraph) {
    // Count edges first (we'll only count one direction since it's undirected)
    int edgeCount = 0;
    for (int i = 0; i < adjGraph->V; i++) {
        AdjListNode* temp = adjGraph->array[i].head;
        while (temp) {
            if (i < temp->dest) { // Only count edges once
                edgeCount++;
            }
            temp = temp->next;
        }
    }
    
    // Create edge array
    Graph* graph = createGraph(adjGraph->V, edgeCount);
    int edgeIndex = 0;
    
    for (int i = 0; i < adjGraph->V; i++) {
        AdjListNode* temp = adjGraph->array[i].head;
        while (temp) {
            if (i < temp->dest) { // Only add edges once
                graph->edges[edgeIndex].src = i;
                graph->edges[edgeIndex].dest = temp->dest;
                graph->edges[edgeIndex].weight = temp->weight;
                edgeIndex++;
            }
            temp = temp->next;
        }
    }
    
    return graph;
}

// A utility function to find the subset of an element i
int find(int parent[], int i) {
    if (parent[i] != i)
        parent[i] = find(parent, parent[i]);
    return parent[i];
}

// A utility function to do union of two subsets
void Union(int parent[], int rank[], int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);

    if (rootX == rootY) return;

    if (rank[rootX] < rank[rootY])
        parent[rootX] = rootY;
    else if (rank[rootX] > rank[rootY])
        parent[rootY] = rootX;
    else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

// Compare function for qsort to sort edges by weight
int compareEdges(const void* a, const void* b) {
    Edge* a1 = (Edge*)a;
    Edge* b1 = (Edge*)b;
    return a1->weight - b1->weight;
}

// Kruskal's algorithm to find MST
EdgeOrder* kruskalMST(Graph* graph, int* mstSize) {
    int V = graph->V;
    EdgeOrder* result = (EdgeOrder*)malloc((V-1) * sizeof(EdgeOrder));
    *mstSize = 0; // Number of edges in MST

    // Step 1: Sort all edges in non-decreasing order of their weight
    qsort(graph->edges, graph->E, sizeof(Edge), compareEdges);

    // Allocate memory for creating V subsets
    int* parent = (int*)malloc(V * sizeof(int));
    int* rank = (int*)malloc(V * sizeof(int));

    // Create V subsets with single elements
    for (int v = 0; v < V; v++) {
        parent[v] = v;
        rank[v] = 0;
    }

    // Number of edges to be taken is equal to V-1
    int e = 0; // Index used for sorted edges
    
    // Process edges one by one
    while (*mstSize < V - 1 && e < graph->E) {
        // Step 2: Pick the smallest edge
        Edge next_edge = graph->edges[e++];

        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);

        // If including this edge doesn't cause cycle, include it in result
        if (x != y) {
            result[*mstSize].src = next_edge.src;
            result[*mstSize].dest = next_edge.dest;
            result[*mstSize].weight = next_edge.weight;
            (*mstSize)++;
            Union(parent, rank, x, y);
        }
    }

    free(parent);
    free(rank);
    
    // If MST doesn't have V-1 edges, then graph is not connected
    if (*mstSize != V - 1) {
        printf("Graph is not connected. Kruskal's algorithm found %d edges for MST.\n", *mstSize);
    }
    
    return result;
}

// Utility function to find the vertex with minimum key value
int minKey(int key[], bool mstSet[], int V) {
    int min = INT_MAX, min_index;
    
    for (int v = 0; v < V; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
            
    return min_index;
}

// Prim's algorithm to find MST
EdgeOrder* primMST(AdjListGraph* graph, int* mstSize) {
    int V = graph->V;
    EdgeOrder* result = (EdgeOrder*)malloc((V-1) * sizeof(EdgeOrder));
    *mstSize = 0;
    
    // Array to store constructed MST
    int* parent = (int*)malloc(V * sizeof(int));
    // Key values used to pick minimum weight edge
    int* key = (int*)malloc(V * sizeof(int));
    // To represent set of vertices included in MST
    bool* mstSet = (bool*)malloc(V * sizeof(bool));
    
    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }
    
    // Always include first vertex in MST
    key[0] = 0;     // Make key 0 so that this vertex is picked as first
    parent[0] = -1; // First node is always root of MST
    
    // The MST will have V vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum key vertex from the set of vertices not yet included in MST
        int u = minKey(key, mstSet, V);
        
        // If no vertex is reachable, break
        if (key[u] == INT_MAX) {
            printf("Graph is not connected. Prim's algorithm found %d edges for MST.\n", *mstSize);
            break;
        }
        
        // Add the picked vertex to the MST Set
        mstSet[u] = true;
        
        // Add this edge to the result except for the first vertex
        if (parent[u] != -1) {
            result[*mstSize].src = parent[u];
            result[*mstSize].dest = u;
            
            // Find the weight of this edge
            AdjListNode* temp = graph->array[parent[u]].head;
            while (temp) {
                if (temp->dest == u) {
                    result[*mstSize].weight = temp->weight;
                    break;
                }
                temp = temp->next;
            }
            
            (*mstSize)++;
        }
        
        // Update key value and parent index of the adjacent vertices of the picked vertex
        AdjListNode* temp = graph->array[u].head;
        while (temp) {
            int v = temp->dest;
            // If v is not yet included in MST and weight of u-v is less than key[v]
            if (mstSet[v] == false && temp->weight < key[v]) {
                parent[v] = u;
                key[v] = temp->weight;
            }
            temp = temp->next;
        }
    }
    
    free(parent);
    free(key);
    free(mstSet);
    
    return result;
}

// Compare edge orders of Prim's and Kruskal's algorithms
void compareEdgeOrders(AdjListGraph* adjGraph) {
    // Convert adjacency list to edge array for Kruskal's algorithm
    Graph* edgeGraph = adjListToEdgeArray(adjGraph);
    
    int primMstSize, kruskalMstSize;
    
    // Get edge orders from both algorithms
    EdgeOrder* primOrder = primMST(adjGraph, &primMstSize);
    EdgeOrder* kruskalOrder = kruskalMST(edgeGraph, &kruskalMstSize);
    
    // Print the edge orders
    printf("\nPrim's Algorithm Edge Order:\n");
    printf("Order\tEdge\t\tWeight\n");
    for (int i = 0; i < primMstSize; i++) {
        printf("%d\t(%d-%d)\t\t%d\n", i+1, primOrder[i].src, primOrder[i].dest, primOrder[i].weight);
    }
    
    printf("\nKruskal's Algorithm Edge Order:\n");
    printf("Order\tEdge\t\tWeight\n");
    for (int i = 0; i < kruskalMstSize; i++) {
        printf("%d\t(%d-%d)\t\t%d\n", i+1, kruskalOrder[i].src, kruskalOrder[i].dest, kruskalOrder[i].weight);
    }
    
    // Compare the two orders
    if (primMstSize != kruskalMstSize) {
        printf("\nThe two algorithms found different MSTs (different number of edges).\n");
    } else {
        printf("\nComparison of Edge Orders:\n");
        
        // Sort both edge sets by weight for clearer comparison
        int sameEdges = 0;
        for (int i = 0; i < primMstSize; i++) {
            bool found = false;
            for (int j = 0; j < kruskalMstSize; j++) {
                if ((primOrder[i].src == kruskalOrder[j].src && primOrder[i].dest == kruskalOrder[j].dest) ||
                    (primOrder[i].src == kruskalOrder[j].dest && primOrder[i].dest == kruskalOrder[j].src)) {
                    found = true;
                    break;
                }
            }
            if (found) sameEdges++;
        }
        
        printf("Edges in common: %d out of %d\n", sameEdges, primMstSize);
        
        if (sameEdges == primMstSize) {
            printf("Both algorithms found the same set of edges, but possibly in different orders.\n");
        } else {
            printf("The algorithms found different sets of edges for the MST.\n");
        }
    }
    
    // Clean up
    free(primOrder);
    free(kruskalOrder);
    free(edgeGraph->edges);
    free(edgeGraph);
}

int main() {
    // Create a sample graph for testing
    int V = 5; // Number of vertices
    AdjListGraph* graph = createAdjListGraph(V);
    
    // Add edges to the graph
    addEdge(graph, 0, 1, 2);
    addEdge(graph, 0, 3, 6);
    addEdge(graph, 1, 2, 3);
    addEdge(graph, 1, 3, 8);
    addEdge(graph, 1, 4, 5);
    addEdge(graph, 2, 4, 7);
    addEdge(graph, 3, 4, 9);
    
    printf("Comparing edge orders of Prim's and Kruskal's algorithms:\n");
    compareEdgeOrders(graph);
    
    // Example with a disconnected graph
    printf("\n\nTesting with a disconnected graph:\n");
    AdjListGraph* disconnectedGraph = createAdjListGraph(5);
    addEdge(disconnectedGraph, 0, 1, 1);
    addEdge(disconnectedGraph, 2, 3, 2);
    // Vertices 0,1 and 2,3 form disconnected components
    compareEdgeOrders(disconnectedGraph);
    
    // Example with a complete graph
    printf("\n\nTesting with a complete graph:\n");
    AdjListGraph* completeGraph = createAdjListGraph(4);
    addEdge(completeGraph, 0, 1, 10);
    addEdge(completeGraph, 0, 2, 6);
    addEdge(completeGraph, 0, 3, 5);
    addEdge(completeGraph, 1, 2, 15);
    addEdge(completeGraph, 1, 3, 4);
    addEdge(completeGraph, 2, 3, 8);
    compareEdgeOrders(completeGraph);
    
    // Clean up memory
    // Free adjacency list
    for (int i = 0; i < V; i++) {
        AdjListNode* temp = graph->array[i].head;
        while (temp) {
            AdjListNode* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->array);
    free(graph);
    
    return 0;
}
