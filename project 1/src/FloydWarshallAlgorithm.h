#ifndef FLOYDWARSHALLALHORITHM_H
#define FLOYDWARSHALLALHORITHM_H

#include "Graph.h"
#include "GraphUtilities.h"

/**
 * Gets the shortest path for all (i, j) pairs using any intermediate vertices.
 */
void floydWarshallAlgorithm(const Graph<NodeInfo> &graph, std::vector<std::vector<double> > &dist, std::vector<std::vector<int> > &nextVertex);

#endif
