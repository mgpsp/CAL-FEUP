#include "GraphUtilities.h"

#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "graphviewer.h"
#include "Utilities.h"

using namespace std;

double getEdgeWeight(int x1, int y1, int x2, int y2) {
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

bool operator==(NodeInfo &n1, const NodeInfo &n2) {
		return (n1.x == n2.x && n1.y == n2.y);
}

Graph<NodeInfo> loadGraph(vector<int> &houses, string vertexesFilename, string edgesFilename)
{
	Graph<NodeInfo> graph;
	ifstream vertexesFile(vertexesFilename.c_str());
	string name, line;
	int x, y, vertexID = 0;

	if (vertexesFile.is_open())
	{
		while(getline(vertexesFile, line))
		{
			stringstream ss(line);
			ss >> name >> x >> y;

			NodeInfo v;
			v.id = vertexID;
			v.name = name;
			v.x = x;
			v.y = y;

			graph.addVertex(v);
			if (name == "house")
				houses.push_back(vertexID);
			vertexID++;
		}
	}

	ifstream edgesFile(edgesFilename.c_str());
	int a, b;

	if (edgesFile.is_open())
	{
		while(getline(edgesFile, line))
		{
			stringstream ss(line);
			ss >> a >> b;

			NodeInfo v1, v2;
			v1 = graph.getVertexSet()[a]->getInfo();
			v2 = graph.getVertexSet()[b]->getInfo();

			graph.addEdge(v1, v2, getEdgeWeight(v1.x, v1.y, v2.x, v2.y));
		}
	}

	return graph;
}

Graph<NodeInfo> getRandomGraph(vector<int> &houses, int vertexes)
{
	Graph<NodeInfo> graph;
	srand (time(NULL));

	for (int i = 0; i < vertexes; i++)
	{
		int x = rand() % 500 + 50;
		int y = rand() % 500 + 50;
		int house = rand() % 2;

		string name;
		if (house)
		{
			name = "house";
			houses.push_back(i);
		}
		else
			name = "junction";

		NodeInfo v;
		v.id = i;
		v.name = name;
		v.x = x;
		v.y = y;

		graph.addVertex(v);
	}

	for (int i = 0; i < vertexes; i++)
	{
		int ne;
		if (vertexes < 4)
			ne = rand() % vertexes;
		else
			ne = rand() % 4;
		NodeInfo v1 = graph.getVertexSet()[i]->getInfo();
		vector<int> e;

		for (int j = 0; j < ne; j++)
		{
			int d = rand() % vertexes;

			if (d != i && !exists(e, d) && graph.getVertexSet()[d]->getAdjacentEdges().size() < 4)
			{
				NodeInfo v2 = graph.getVertexSet()[d]->getInfo();
				graph.addEdge(v1, v2, getEdgeWeight(v1.x, v1.y, v2.x, v2.y));
			}
		}
	}

	return graph;
}

void printGraph(const Graph<NodeInfo> &graph, const vector<int> &path, const vector<int> &stations)
{
	GraphViewer *gv = new GraphViewer(600, 600, false);
	gv->createWindow(600, 600);
	gv->defineVertexColor("black");
	gv->defineEdgeColor("black");

	vector<Vertex<NodeInfo> *> vertexSet = graph.getVertexSet();
	for (size_t i = 0; i < vertexSet.size(); i++)
	{
		gv->addNode(vertexSet[i]->getInfo().id, vertexSet[i]->getInfo().x, vertexSet[i]->getInfo().y);
		if (vertexSet[i]->getInfo().name == "house")
			gv->setVertexColor(vertexSet[i]->getInfo().id, "red");
	}

	for (size_t i = 0; i < stations.size(); i++)
		gv->setVertexColor(stations[i], "green");

	int edgeID = 0;
	vector<int> edges;
	for (size_t i = 0; i < vertexSet.size(); i++)
	{
		vector<Edge<NodeInfo> > adj = vertexSet[i]->getAdjacentEdges();
		for (size_t j = 0; j < adj.size(); j++)
		{
			int src = vertexSet[i]->getInfo().id;
			int dst = adj[j].getDestination()->getInfo().id;

			if (!edgeExists(edges, src, dst))
			{
				gv->addEdge(edgeID, src, dst, EdgeType::UNDIRECTED);
				stringstream ss;
				ss << adj[j].getWeight();
				gv->setEdgeLabel(edgeID, ss.str());
				edges.push_back(src);
				edges.push_back(dst);
				edgeID++;
			}
		}
	}

	if (path.size() != 0)
	{
		for (size_t i = 0; i < path.size() - 1; i+=2)
		{
			gv->addEdge(edgeID, path[i], path[i + 1], EdgeType::UNDIRECTED);
			gv->setEdgeColor(edgeID, "green");
			gv->setEdgeThickness(edgeID, 2);
			edgeID++;
		}
	}

	cin.clear();
	cin.ignore(100, '\n');
	cout << "\n\tPress enter to continue...";
	cin.get();

	gv->closeWindow();
	delete(gv);
}
