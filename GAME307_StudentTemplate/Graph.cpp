#include "Graph.h"


MATH::Vec2 operator - (const MATH::Vec2& a, const MATH::Vec3& b) {
	return { a.x - b.x, a.y - b.y };
}

MATH::Vec2 operator * (const MATH::Vec2& a, const float b) {
	return { a.x * b, a.y * b };
}
MATH::Vec2 ToVec2(const MATH::Vec3 a) {
	return { a.x, a.y };
}

float mag(const MATH::Vec2& a) {
	return std::sqrt(a.x * a.x + a.y * a.y);
}


float distance(const MATH::Vec2& a, const MATH::Vec2& b) {
	return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

MATH::Vec2 normalize(const MATH::Vec2& a) {
	float magnitude = mag(a);
	if (magnitude <= FLT_EPSILON) {
		return { 0.0f, 0.0f };
	}
	return { a.x / magnitude, a.y / magnitude };
}

	std::vector<Graph::Node*> Graph::getNeighbors(const Node& node) const {
	std::vector<Node*> neighbors;
	for (const auto connection : connect) {
		if (connection->node1->id == node.id)
			neighbors.push_back(connection->node2);
		else if (connection->node2->id == node.id)
			neighbors.push_back(connection->node1);
	}
	return neighbors;
}

	std::vector<Graph::Node*>& Graph::getNodes() {
		return nodes;
	}

	std::vector<Graph::Connection*>& Graph::getConnections() {
		return connect;
	}

	Graph::Node& Graph::addNode(MATH::Vec2 pos) {
		nodes.emplace_back(new Node{ pos, nextnodeid++ });
		return*nodes.back();
	}

	Graph::Connection& Graph::addConnect(Node& node1, Node& node2, float cost) {
		connect.emplace_back(new Connection{ &node1, &node2, cost });
		connectionweightmap[node1.id][node2.id] = cost;
		connectionweightmap[node2.id][node1.id] = cost;
		return*connect.back();
	}

	float Graph::getconnectionweight(Node& node1, Node& node2) {
		auto iterate1 = connectionweightmap.find(node1.id);
		if (iterate1 != connectionweightmap.end()) {
			auto iterate2 = iterate1->second.find(node2.id);
			if (iterate2 != iterate1->second.end()) {
				return iterate2->second;
			}
		}
		return INFINITY;
	}

	Graph::~Graph() {
		for (Node* node : nodes) {
			delete node;
		};
		for (Connection* con : connect) {
			delete con;
		};
	}

	Graph::Node& Graph::getNearestNode(const MATH::Vec2& position) const {
		Node* nearestNode = nullptr;
		float nearestDist = FLT_MAX;
		for (const auto& node : nodes) {
			const float dist = distance(node->pos, position);
			if (dist < nearestDist) {
				nearestNode = node;
				nearestDist = dist;
			}
		}
		return *nearestNode;
	}

	std::vector<Graph::Connection*> Graph::getConnections(const Node& node) const
	{
		std::vector<Connection*> nodeConnections;
		for (const auto connection : connect) {
			if (connection->node1->id == node.id || connection->node2->id == node.id)
				nodeConnections.push_back(connection);
		}
		return nodeConnections;
	}

	float Path::distancetonextnode(MATH::Vec2 position) {
		if (nodes.empty() || currentNode < 0 || currentNode >= nodes.size()) {
			return -1.0f;
		}
		return distance(position, nodes[currentNode].pos);
	}

	SteeringOutput Path::getsteering(Body* move)
	{
		float distance = distancetonextnode(ToVec2(move->getPos()));
		if (distance < 0.25f) {
			++currentNode;
		}
		if (nodes.empty() || currentNode >= nodes.size()) {
			return SteeringOutput();
		}
		auto targetpos = nodes[currentNode].pos;
		auto desiredvel = targetpos - ToVec2(move->getPos());
		if (mag(desiredvel) > FLT_EPSILON) {
			desiredvel = normalize(desiredvel);
			float maxspeed = 0.25f;
			desiredvel *= maxspeed;
		}
		auto steering = desiredvel - move->getVel();
		std::cout << "\033[2J\033[H";
		std::cout << "Current Node Index: " << currentNode << std::endl;
		std::cout << "Distance to next node: " << distance << std::endl;
		std::cout << "Desired Vel: {x: " << desiredvel.x << ", y: " << desiredvel.y << "}\n";
		std::cout << "Steering: {x: " << steering.x << ", y: " << steering.y << "}\n";
		return { {steering.x, steering.y, 0}, 0 };
	}

	Path findpath(Graph& graph, Graph::Node& start, Graph::Node& goal) {
		using PQElement = std::pair<float, Graph::Node*>;
		std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>> frontier;
		frontier.emplace(0.0f, &start);
		std::unordered_map<int, float> costsofar;
		costsofar[start.id] = 0.0f;
		std::unordered_map<int, Graph::Node*> camefrom;
		camefrom[start.id] = nullptr;
		while (!frontier.empty()) {
			auto current = frontier.top().second;
			frontier.pop();
			if (current->id == goal.id) {
				Path path;
				for (auto node = current; node != nullptr; node = camefrom[node->id]) {
					path.nodes.push_back(*node);
				}
				std::reverse(path.nodes.begin(), path.nodes.end());
				return path;
			}
			for (auto neighbor : graph.getNeighbors(*current)) {
				float newcost = costsofar[current->id] + graph.getconnectionweight(*current, *neighbor);
				if (costsofar.find(neighbor->id) == costsofar.end() || newcost < costsofar[neighbor->id]) {
					costsofar[neighbor->id] = newcost;
					float priority = newcost + distance(neighbor->pos, goal.pos);
					frontier.emplace(priority, neighbor);
					camefrom[neighbor->id] = current;
				}
			}
		}
		return Path{};
	}
