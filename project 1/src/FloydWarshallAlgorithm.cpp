#include "FloydWarshallAlgorithm.h"

using namespace std;

void floydWarshallAlgorithm(const Graph<NodeInfo> &graph, vector<vector<double> > &dist, vector<vector<int> > &nextVertex)
{
	int numVertex = graph.getNumVertex();

	vector<Vertex<NodeInfo> *> vertexSet = graph.getVertexSet();
	for (size_t i = 0; i < vertexSet.size(); i++)
	{
		int vertexID = vertexSet[i]->getInfo().id;
		dist[vertexID][vertexID] = 0;

		vector<Edge<NodeInfo> > adj = vertexSet[i]->getAdjacentEdges();
		for (size_t j = 0; j < adj.size(); j++)
		{
			dist[vertexSet[i]->getInfo().id][adj[j].getDestination()->getInfo().id] = adj[j].getWeight();
			nextVertex[vertexID][adj[j].getDestination()->getInfo().id] = adj[j].getDestination()->getInfo().id;
		}
	}

	for (int k = 0; k < numVertex; k++) {
		for (int i = 0; i < numVertex; i++) {
			for (int j = 0; j < numVertex; j++)
			{
				if (dist[i][j] > dist[i][k] + dist[k][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					nextVertex[i][j] = nextVertex[i][k];
				}
			}
		}
	}
}
