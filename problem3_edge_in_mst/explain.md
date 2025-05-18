first calculate the MST weight of the entire graph using Kruskal's algorithm
Then calculate the MST weight excluding the specified edge
If the MST weight increases when the edge is excluded, or if the graph becomes disconnected, then the edge must be part of the MST
An edge is in the MST if and only if it is the minimum weight edge in any cycle it participates in
first check if the edge is a bridge
If not a bridge, find the minimum weight edge in any path connecting the endpoints of the target edge in the graph with the target edge removed
If the target edge has weight less than this minimum, it must be in the MST
un Prim's algorithm to construct the MST
Then check if the specified edge was selected during the MST construction
