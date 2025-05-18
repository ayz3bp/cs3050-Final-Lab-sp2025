int isEdgeInMST(Edge edges[], int numEdges, int numVertices, Edge query) {
    makeSet(numVertices);
    qsort(edges, numEdges, sizeof(Edge), compareEdges);

    for (int i = 0; i < numEdges; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;

        int ru = find(u);
        int rv = find(v);

        // Check for undirected match
        int isQueryEdge = 
            ((u == query.u && v == query.v) || (u == query.v && v == query.u)) && (w == query.weight);

        if (ru != rv) {
            if (isQueryEdge) {
                return 1; // It connects different components and would be used
            }
            unionSets(ru, rv);
        } else if (isQueryEdge) {
            return 0; // It would have formed a cycle, so not in MST
        }
    }

    return 0;
}
