#pragma once
#include "Vector.h"
#include <vector>
#include "VMath.h"
#include <unordered_map>
#include <queue>
#include "SteeringOutput.h"
#include "Body.h"

class Graph
{
public:
	struct Node {
		MATH::Vec2 pos;
		int id;
	};
	struct Connection {
		Node* node1, *node2;
		float cost;
	};
	std::vector<Node*> getNeighbors(const Node& node) const;
	std::vector<Node*>& getNodes();
	std::vector<Connection*>& getConnections();;
	Node& addNode(MATH::Vec2 pos);;
	Connection& addConnect(Node &node1, Node &node2, float cost = 1);;
	float getconnectionweight(Node& node1, Node& node2);
	~Graph();
	Node& getNearestNode(const MATH::Vec2& position) const;
	std::vector<Connection*> getConnections(const Node& node) const;
	// Array of connections outgoing from a given node
	/*function getConnections(fromNode);*/
private:
	std::vector<Node*> nodes;
	std::vector<Connection*> connect;
	std::unordered_map<int, std::unordered_map<int, float>> connectionweightmap;
	int nextnodeid;
};

struct Path {
	int currentNode = 0;
	std::vector<Graph::Node> nodes;
	float distancetonextnode(MATH::Vec2 position);
	SteeringOutput getsteering(Body* move);
};

Path findpath(Graph &graph, Graph::Node &start, Graph::Node &goal);



