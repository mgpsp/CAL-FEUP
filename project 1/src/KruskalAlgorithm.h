#ifndef KRUSKALALGORITHM_H
#define KRUSKALALGORITHM_H

#include "Graph.h"
#include "GraphUtilities.h"

std::vector<int> kruskalAlgorithm(std::vector<Edge<NodeInfo> > &edges, const std::vector<int> &vertexSet, const std::vector<std::vector<int> > &nextVertex,const std::vector<std::vector<double> > &dist, double stationCost, int maxStations, std::vector<int> &stationsLocation);

int findParent(const std::vector<int> &parent, int index);

/**
 * Gets the path between 2 vertexes, inserting in newPath only the edges that are not yet in the graph.
 * Calculates and returns the cost of the newPath.
 * Updates the parent vector.
 */
vector<int> getPath(const std::vector<int> &path, const std::vector<std::vector<int> > &nextVertex, std::vector<int> &parent, int i, int j);

/*
 * Considering the new edges in the path, updates the edges vector with the correct weights.
 */
void updateEdges(std::vector<Edge<NodeInfo> > &edges, const std::vector<int> &path, const std::vector<std::vector<int> > &nextVertex, const std::vector<std::vector<double> > &dist, int i);

/**
 * Places a station for each tree of the graph.
 */
std::vector<int> placeStations(const std::vector<int> &path, const std::vector<int> &parent);

#endif
