Implented an adjacency list representation for prims algorithm
An edge array representation for kruskals algorithm, which makes sorting edges by weight straightforward
For prim's algo, started with an arbitrary vertex and grow the MST by adding the minimum weight edge that connects a vertex in the MST to a vertex outside it
For Kruskal's algo, sort all edges by weight and then add them to the MST if they don't create a cycle
For utility, while both find a minimum spanning tree if it exists, they take different approaches. Prims grows a single tree incrimentally while Kruskals considers all edges 
There are some instances in which you want to use prims because it maintains a connected compoment throughout the construction, while kruskals could be better if you want to work with multiple components that later become joined
