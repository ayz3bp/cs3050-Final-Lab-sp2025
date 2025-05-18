#include <stdio.h>
#include <stdlib.h>

#define MAX_EDGES 1000
#define MAX_VERTICES 100

typedef struct {
    int u, v, weight;
} Edge;

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

int compareEdges(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

// Check if given edge is in *some* MST
int isEdgeInMST(Edge edges[], int numEdges, int numVertices, Edge query) {
    makeSet(numVertices);
    qsort(edges, numEdges, sizeof(Edge), compareEdges);

    for (int i = 0; i < numEdges; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;

        if (find(u) != find(v)) {
            unionSets(u, v);
        }

        // If the current edge is heavier and connects same component as query
        if (edges[i].u == query.u && edges[i].v == query.v && edges[i].weight == query.weight) {
            // Check if adding this edge would have formed a cycle
            if (find(query.u) == find(query.v)) {
                return 0; // It creates a cycle and is not needed
            } else {
                return 1; // It was part of the MST
            }
        }
    }
    return 0;
}

int main() {
    int numVertices = 4;
    Edge edges[] = {
        {0, 1, 1},
        {0, 2, 4},
        {1, 2, 2},
        {1, 3, 5},
        {2, 3, 3}
    };
    int numEdges = sizeof(edges) / sizeof(edges[0]);

    Edge query = {1, 2, 2};

    if (isEdgeInMST(edges, numEdges, numVertices, query)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }

    return 0;
}
