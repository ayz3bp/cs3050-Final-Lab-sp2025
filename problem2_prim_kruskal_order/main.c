#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_EDGES 100
#define MAX_VERTICES 100

typedef struct {
    int u, v, weight;
} Edge;

typedef struct AdjNode {
    int to, weight;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    AdjNode* head;
} AdjList;

int parent[MAX_VERTICES];

void makeSet(int n) {
    for (int i = 0; i < n; i++)
        parent[i] = i;
}

int find(int u) {
    if (u != parent[u])
        parent[u] = find(parent[u]);
    return parent[u];
}

void unionSets(int u, int v) {
    parent[find(u)] = find(v);
}

int edgeCompare(const void* a, const void* b) {
    Edge* ea = (Edge*)a;
    Edge* eb = (Edge*)b;
    return ea->weight - eb->weight;
}

int kruskalMST(Edge edges[], int numEdges, int numVertices, Edge result[]) {
    makeSet(numVertices);
    qsort(edges, numEdges, sizeof(Edge), edgeCompare);

    int count = 0;
    for (int i = 0; i < numEdges && count < numVertices - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        if (find(u) != find(v)) {
            unionSets(u, v);
            result[count++] = edges[i];
        }
    }
    return count;
}

void primMST(AdjList* graph, int numVertices, Edge result[]) {
    bool visited[MAX_VERTICES] = {false};
    int edgeCount = 0;

    visited[0] = true; // Start from vertex 0

    while (edgeCount < numVertices - 1) {
        int minWeight = 1e9;
        int from = -1, to = -1;

        for (int u = 0; u < numVertices; u++) {
            if (!visited[u]) continue;
            AdjNode* node = graph[u].head;
            while (node) {
                if (!visited[node->to] && node->weight < minWeight) {
                    minWeight = node->weight;
                    from = u;
                    to = node->to;
                }
                node = node->next;
            }
        }

        if (to == -1) break; // Graph disconnected

        visited[to] = true;
        result[edgeCount++] = (Edge){from, to, minWeight};
    }
}

void printEdges(Edge edges[], int count, const char* label) {
    printf("%s Edge Order:\n", label);
    for (int i = 0; i < count; i++) {
        printf("(%d - %d) weight: %d\n", edges[i].u, edges[i].v, edges[i].weight);
    }
    printf("\n");
}

int main() {
    int numVertices = 4;

    // Define edges
    Edge edges[] = {
        {0, 1, 1},
        {0, 2, 4},
        {1, 2, 2},
        {1, 3, 5},
        {2, 3, 3}
    };
    int numEdges = sizeof(edges) / sizeof(edges[0]);

    // Build adjacency list
    AdjList graph[MAX_VERTICES] = {0};
    for (int i = 0; i < numEdges; i++) {
        AdjNode* n1 = malloc(sizeof(AdjNode));
        n1->to = edges[i].v;
        n1->weight = edges[i].weight;
        n1->next = graph[edges[i].u].head;
        graph[edges[i].u].head = n1;

        AdjNode* n2 = malloc(sizeof(AdjNode));
        n2->to = edges[i].u;
        n2->weight = edges[i].weight;
        n2->next = graph[edges[i].v].head;
        graph[edges[i].v].head = n2;
    }

    Edge primOrder[MAX_EDGES], kruskalOrder[MAX_EDGES];

    int kruskalCount = kruskalMST(edges, numEdges, numVertices, kruskalOrder);
    primMST(graph, numVertices, primOrder);

    printEdges(kruskalOrder, kruskalCount, "Kruskal");
    printEdges(primOrder, numVertices - 1, "Prim");

    return 0;
}
