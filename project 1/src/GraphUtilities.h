#ifndef GRAPHUTILITIES_H
#define GRAPHUTILITIES_H

#include <string>

#include "Graph.h"

typedef struct {
	int id;
	std::string name;
	int x, y;

}  NodeInfo;

bool operator==(NodeInfo &n1, const NodeInfo &n2);

Graph<NodeInfo> loadGraph(vector<int> &houses, string vertexesFilename, string edgesFilename);
Graph<NodeInfo> getRandomGraph(vector<int> &houses, int vertexes);
void printGraph(const Graph<NodeInfo> &graph, const vector<int> &path, const vector<int> &stations);

double getEdgeWeight(int x1, int y1, int x2, int y2);

#endif
