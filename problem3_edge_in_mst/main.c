#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Define edge structure
typedef struct Edge {
    int src;
    int dest;
    int weight;
    struct Edge* next;
} Edge;

// Define graph structure
typedef struct Graph {
    int V;              // Number of vertices
    Edge** adjList;     // Adjacency list
} Graph;

// Function to create a new edge
Edge* createEdge(int src, int dest, int weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->src = src;
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = NULL;
    return newEdge;
}

// Function to create a graph with V vertices
Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->adjList = (Edge**)malloc(V * sizeof(Edge*));
    
    // Initialize all adjacency lists as empty
    for (int i = 0; i < V; i++) {
        graph->adjList[i] = NULL;
    }
    
    return graph;
}

// Function to add an edge to the graph
void addEdge(Graph* graph, int src, int dest, int weight) {
    // Add edge from src to dest
    Edge* newEdge = createEdge(src, dest, weight);
    newEdge->next = graph->adjList[src];
    graph->adjList[src] = newEdge;
    
    // Add edge from dest to src for undirected graph
    newEdge = createEdge(dest, src, weight);
    newEdge->next = graph->adjList[dest];
    graph->adjList[dest] = newEdge;
}

// Structure to represent a subset for union-find
typedef struct Subset {
    int parent;
    int rank;
} Subset;

// Find function with path compression
int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Union function by rank
void Union(Subset subsets[], int x, int y) {
    int rootX = find(subsets, x);
    int rootY = find(subsets, y);
    
    if (rootX == rootY) return;
    
    // Attach smaller rank tree under root of higher rank tree
    if (subsets[rootX].rank < subsets[rootY].rank)
        subsets[rootX].parent = rootY;
    else if (subsets[rootX].rank > subsets[rootY].rank)
        subsets[rootY].parent = rootX;
    else {
        // If ranks are same, make one as root and increment its rank
        subsets[rootY].parent = rootX;
        subsets[rootX].rank++;
    }
}

// Structure to store edges for Kruskal's algorithm
typedef struct EdgeForKruskal {
    int src, dest, weight;
} EdgeForKruskal;

// Compare function for qsort
int compareEdges(const void* a, const void* b) {
    return ((EdgeForKruskal*)a)->weight - ((EdgeForKruskal*)b)->weight;
}

// Method 1: Using Kruskal's algorithm
// Check if the edge is in MST by removing it and seeing if MST weight increases
bool isEdgeInMST_Kruskal(Graph* graph, int u, int v) {
    int V = graph->V;
    int E = 0; // Count edges
    
    // Count the total number of edges
    for (int i = 0; i < V; i++) {
        Edge* temp = graph->adjList[i];
        while (temp) {
            if (temp->src < temp->dest) // Count each edge only once
                E++;
            temp = temp->next;
        }
    }
    
    // Create an array of all edges
    EdgeForKruskal* edges = (EdgeForKruskal*)malloc(E * sizeof(EdgeForKruskal));
    int e = 0;
    
    for (int i = 0; i < V; i++) {
        Edge* temp = graph->adjList[i];
        while (temp) {
            if (temp->src < temp->dest) { // Add each edge only once
                edges[e].src = temp->src;
                edges[e].dest = temp->dest;
                edges[e].weight = temp->weight;
                e++;
            }
            temp = temp->next;
        }
    }
    
    // Sort edges in increasing order of weight
    qsort(edges, E, sizeof(EdgeForKruskal), compareEdges);
    
    // Calculate MST weight with all edges
    int mstWeight = 0;
    Subset* subsets = (Subset*)malloc(V * sizeof(Subset));
    
    // Initialize subsets
    for (int i = 0; i < V; i++) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }
    
    int edgeCount = 0;
    for (int i = 0; i < E && edgeCount < V - 1; i++) {
        int setU = find(subsets, edges[i].src);
        int setV = find(subsets, edges[i].dest);
        
        if (setU != setV) {
            mstWeight += edges[i].weight;
            Union(subsets, setU, setV);
            edgeCount++;
        }
    }
    
    // Reset subsets for recalculation
    for (int i = 0; i < V; i++) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }
    
    // Calculate MST weight without the specified edge
    int mstWeightWithoutEdge = 0;
    edgeCount = 0;
    
    for (int i = 0; i < E && edgeCount < V - 1; i++) {
        // Skip the specified edge
        if ((edges[i].src == u && edges[i].dest == v) || 
            (edges[i].src == v && edges[i].dest == u))
            continue;
            
        int setU = find(subsets, edges[i].src);
        int setV = find(subsets, edges[i].dest);
        
        if (setU != setV) {
            mstWeightWithoutEdge += edges[i].weight;
            Union(subsets, setU, setV);
            edgeCount++;
        }
    }
    
    // Check if graph is still connected
    bool isConnected = true;
    int root = find(subsets, 0);
    for (int i = 1; i < V; i++) {
        if (find(subsets, i) != root) {
            isConnected = false;
            break;
        }
    }
    
    free(subsets);
    free(edges);
    
    // If graph is not connected without the edge, it's a bridge and must be in MST
    if (!isConnected)
        return true;
    
    // If MST weight increases without the edge, then the edge is in MST
    return mstWeightWithoutEdge > mstWeight;
}

// Method 2: Using cycle property
// An edge is in MST if and only if it's the minimum weight edge in a cycle it forms
bool isEdgeInMST_CycleProperty(Graph* graph, int u, int v) {
    // Find the weight of edge (u, v)
    int edgeWeight = INT_MAX;
    Edge* temp = graph->adjList[u];
    while (temp) {
        if (temp->dest == v) {
            edgeWeight = temp->weight;
            break;
        }
        temp = temp->next;
    }
    
    if (edgeWeight == INT_MAX) {
        printf("Edge (%d, %d) not found in the graph\n", u, v);
        return false;
    }
    
    // Create a subgraph by removing edge (u, v)
    Graph* subgraph = createGraph(graph->V);
    for (int i = 0; i < graph->V; i++) {
        temp = graph->adjList[i];
        while (temp) {
            if (!((temp->src == u && temp->dest == v) || 
                  (temp->src == v && temp->dest == u))) {
                addEdge(subgraph, temp->src, temp->dest, temp->weight);
            }
            temp = temp->next;
        }
    }
    
    // Check if there's a path from u to v in the subgraph
    // If not, then (u, v) is a bridge and must be in the MST
    bool* visited = (bool*)calloc(graph->V, sizeof(bool));
    
    // DFS to check if v is reachable from u
    bool dfs(Graph* g, int start, int target, bool visited[]) {
        visited[start] = true;
        if (start == target)
            return true;
            
        Edge* temp = g->adjList[start];
        while (temp) {
            if (!visited[temp->dest]) {
                if (dfs(g, temp->dest, target, visited))
                    return true;
            }
            temp = temp->next;
        }
        return false;
    }
    
    bool isReachable = dfs(subgraph, u, v, visited);
    free(visited);
    
    // If not reachable, edge is a bridge and must be in MST
    if (!isReachable)
        return true;
    
    // If reachable, then check if edge (u, v) is the minimum weight edge 
    // in the cycle formed by the path from u to v plus the edge (u, v)
    // This requires finding the path and checking all edge weights
    
    // For simplicity, we can use a different approach:
    // Run a modified DFS to find the minimum weight edge in any path from u to v
    
    bool hasWeakEdge(Graph* g, int start, int target, bool visited[], int* minWeight) {
        visited[start] = true;
        if (start == target)
            return true;
            
        Edge* temp = g->adjList[start];
        while (temp) {
            if (!visited[temp->dest]) {
                if (hasWeakEdge(g, temp->dest, target, visited, minWeight)) {
                    if (temp->weight < *minWeight)
                        *minWeight = temp->weight;
                    return true;
                }
            }
            temp = temp->next;
        }
        visited[start] = false; // Backtrack
        return false;
    }
    
    visited = (bool*)calloc(graph->V, sizeof(bool));
    int minWeight = INT_MAX;
    hasWeakEdge(subgraph, u, v, visited, &minWeight);
    free(visited);
    
    // Free the subgraph
    for (int i = 0; i < subgraph->V; i++) {
        temp = subgraph->adjList[i];
        while (temp) {
            Edge* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(subgraph->adjList);
    free(subgraph);
    
    // If edge (u, v) has weight strictly less than the minimum weight edge
    // in any path from u to v in the subgraph, then it must be in the MST
    return edgeWeight < minWeight;
}

// Method 3: Using Prim's algorithm
// Run Prim's and check if the edge is selected
bool isEdgeInMST_Prim(Graph* graph, int u, int v) {
    int V = graph->V;
    int* parent = (int*)malloc(V * sizeof(int));
    int* key = (int*)malloc(V * sizeof(int));
    bool* mstSet = (bool*)calloc(V, sizeof(bool));
    
    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }
    
    // Start with the first vertex
    key[0] = 0;
    
    // MST will have V vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum key vertex not yet included in MST
        int minKey = INT_MAX, minIndex;
        for (int v = 0; v < V; v++) {
            if (mstSet[v] == false && key[v] < minKey) {
                minKey = key[v];
                minIndex = v;
            }
        }
        
        // Add the picked vertex to the MST set
        mstSet[minIndex] = true;
        
        // Update key values of the adjacent vertices
        Edge* temp = graph->adjList[minIndex];
        while (temp) {
            int v = temp->dest;
            if (mstSet[v] == false && temp->weight < key[v]) {
                parent[v] = minIndex;
                key[v] = temp->weight;
            }
            temp = temp->next;
        }
    }
    
    // Check if edge (u, v) is in the MST
    bool inMST = false;
    for (int i = 1; i < V; i++) {
        if ((parent[i] == u && i == v) || (parent[i] == v && i == u)) {
            inMST = true;
            break;
        }
    }
    
    free(parent);
    free(key);
    free(mstSet);
    
    return inMST;
}

int main() {
    // Create a sample graph
    Graph* graph = createGraph(5);
    
    // Add edges to the graph
    addEdge(graph, 0, 1, 2);
    addEdge(graph, 0, 3, 6);
    addEdge(graph, 1, 2, 3);
    addEdge(graph, 1, 3, 8);
    addEdge(graph, 1, 4, 5);
    addEdge(graph, 2, 4, 7);
    addEdge(graph, 3, 4, 9);
    
    // Check if edge (0, 1) is in MST
    int u = 0, v = 1;
    printf("Is edge (%d, %d) in MST?\n", u, v);
    
    // Method 1: Using Kruskal's algorithm
    bool inMST_Kruskal = isEdgeInMST_Kruskal(graph, u, v);
    printf("Method 1 (Kruskal's): %s\n", inMST_Kruskal ? "Yes" : "No");
    
    // Method 2: Using cycle property
    bool inMST_CycleProperty = isEdgeInMST_CycleProperty(graph, u, v);
    printf("Method 2 (Cycle Property): %s\n", inMST_CycleProperty ? "Yes" : "No");
    
    // Method 3: Using Prim's algorithm
    bool inMST_Prim = isEdgeInMST_Prim(graph, u, v);
    printf("Method 3 (Prim's): %s\n", inMST_Prim ? "Yes" : "No");
    
    // Check another edge
    u = 1, v = 3;
    printf("\nIs edge (%d, %d) in MST?\n", u, v);
    
    // Method 1: Using Kruskal's algorithm
    inMST_Kruskal = isEdgeInMST_Kruskal(graph, u, v);
    printf("Method 1 (Kruskal's): %s\n", inMST_Kruskal ? "Yes" : "No");
    
    // Method 2: Using cycle property
    inMST_CycleProperty = isEdgeInMST_CycleProperty(graph, u, v);
    printf("Method 2 (Cycle Property): %s\n", inMST_CycleProperty ? "Yes" : "No");
    
    // Method 3: Using Prim's algorithm
    inMST_Prim = isEdgeInMST_Prim(graph, u, v);
    printf("Method 3 (Prim's): %s\n", inMST_Prim ? "Yes" : "No");
    
    // Free the graph
    for (int i = 0; i < graph->V; i++) {
        Edge* temp = graph->adjList[i];
        while (temp) {
            Edge* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->adjList);
    free(graph);
    
    return 0;
}
