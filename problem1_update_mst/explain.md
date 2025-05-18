When adding a new vertex to the MST, find the minimum weight edge to add to the existing MST. This is to maintain the properties of the MST.
Maintaine the adjancency list properties
Modified it to take the entire adjancency list rather than just the edges of the new vertex and iterate through all edges from the new vertex 
to find the one with minimum weight to any vertex in the existing MST
The utility of the data structure is actually pretty straightfoward. Instead of recomputing the entire MST from scratch, this
approach is  more efficient


