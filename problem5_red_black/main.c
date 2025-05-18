#include <stdio.h>
#include <stdlib.h>

#define MAXV 1000
#define MAXE 10000

typedef struct {
    int u, v, w;
} Edge;

typedef struct {
    int parent, rank;
} Subset;

Edge edges[MAXE];
int numVertices, numEdges;

int cmpEdge(const void *a, const void *b) {
    return ((Edge*)a)->w - ((Edge*)b)->w;
}

int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void unionSets(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
    if (xroot == yroot) return;
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int isConnected(int limit) {
    Subset subsets[MAXV];
    for (int i = 0; i < numVertices; i++) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    int count = 0;
    for (int i = 0; i < numEdges; i++) {
        if (edges[i].w > limit) break;
        int u = edges[i].u;
        int v = edges[i].v;
        if (find(subsets, u) != find(subsets, v)) {
            unionSets(subsets, u, v);
            count++;
        }
    }

    return count == numVertices - 1;
}

int computeBST() {
    qsort(edges, numEdges, sizeof(Edge), cmpEdge);

    int left = 0, right = numEdges - 1, ans = -1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (isConnected(edges[mid].w)) {
            ans = edges[mid].w;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return ans;
}

int main() {
    scanf("%d %d", &numVertices, &numEdges);
    for (int i = 0; i < numEdges; i++) {
        scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].w);
    }

    int bottleneck = computeBST();
    if (bottleneck == -1) {
        printf("No spanning tree exists\n");
    } else {
        printf("Bottleneck weight: %d\n", bottleneck);
    }

    return 0;
}
