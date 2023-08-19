#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <climits>
using namespace std;

// Define a Node class
class Node {
public:
    string id;
    string type;

    Node(string id, string type) : id(id), type(type) {}
};

// Define an Edge class
class Edge {
public:
    int cost;

    Edge(int cost) : cost(cost) {}
};

// Define the Universe and Galaxy classes
class Universe {
public:
    unordered_map<string, Node*> nodes;

    void addNode(Node* node) {
        nodes[node->id] = node;
    }
};

class Galaxy {
public:
    Universe* universe;
    unordered_map<string, unordered_map<string, Edge*>> edgeMap;

    Galaxy(Universe* universe) : universe(universe) {}

    void addEdge(Node* fromNode, Node* toNode, int cost) {
        if (!edgeMap.count(fromNode->id)) {
            edgeMap[fromNode->id] = unordered_map<string, Edge*>();
        }
        edgeMap[fromNode->id][toNode->id] = new Edge(cost);
    }
};

// Function to find the shortest path between two nodes using Dijkstra's algorithm
void shortestPath(Galaxy* galaxy, Node* source, Node* destination) {
    unordered_map<string, int> distances;
    unordered_map<string, string> prevNodes;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    for (auto& nodePair : galaxy->universe->nodes) {
        distances[nodePair.first] = INT_MAX;
    }

    distances[source->id] = 0;
    pq.push(make_pair(0, source->id));

    while (!pq.empty()) {
        auto curr = pq.top();
        pq.pop();

        string currNodeId = curr.second;
        int currDist = curr.first;

        if (currDist > distances[currNodeId]) continue;

        for (auto& edgePair : galaxy->edgeMap[currNodeId]) {
            Node* neighborNode = galaxy->universe->nodes[edgePair.first];
            int newDist = currDist + edgePair.second->cost;

            if (newDist < distances[neighborNode->id]) {
                distances[neighborNode->id] = newDist;
                prevNodes[neighborNode->id] = currNodeId;
                pq.push(make_pair(newDist, neighborNode->id));
            }
        }
    }

    // Print the shortest path and total cost
    vector<string> path;
    string currNode = destination->id;
    while (!currNode.empty()) {
        path.push_back(currNode);
        currNode = prevNodes[currNode];
    }

    cout << "Shortest Path: ";
    for (int i = path.size() - 1; i >= 0; --i) {
        cout << path[i];
        if (i > 0) cout << " -> ";
    }

    cout << "\nTotal Cost: " << distances[destination->id] << endl;
}

int main() {
    // Create the universe
    Universe universe;
    Node* A4 = new Node("A4", "BG");
    Node* B4 = new Node("B4", "BG");
    Node* C4 = new Node("C4", "BG");
    universe.addNode(A4);
    universe.addNode(B4);
    universe.addNode(C4);

    // Create the galaxy
    Galaxy galaxy(&universe);
    galaxy.addEdge(A4, B4, 50);
    galaxy.addEdge(B4, C4, 30);

    // Find and print the shortest path and total cost
    Node* sourceNode = A4;
    Node* destinationNode = C4;
    shortestPath(&galaxy, sourceNode, destinationNode);
    while (true) {
        cout << "Menu:\n";
        cout << "1. Create Node\n";
        cout << "2. Create Edge\n";
        cout << "3. Find Shortest Path (Within Galaxy)\n";
        cout << "4. Find Shortest Path (Inter-Galaxy)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string nodeId, nodeType;
                cout << "Enter node ID: ";
                cin >> nodeId;
                cout << "Enter node type: ";
                cin >> nodeType;
                Node* newNode = new Node(nodeId, nodeType);
                universe.addNode(newNode);
                cout << "Node created successfully!\n";
                break;
            }
            case 2: {
                string fromNodeId, toNodeId;
                int cost;
                cout << "Enter source node ID: ";
                cin >> fromNodeId;
                cout << "Enter destination node ID: ";
                cin >> toNodeId;
                cout << "Enter cost: ";
                cin >> cost;
                if (universe.nodes.count(fromNodeId) && universe.nodes.count(toNodeId)) {
                    Node* fromNode = universe.nodes[fromNodeId];
                    Node* toNode = universe.nodes[toNodeId];
                    galaxy.addEdge(fromNode, toNode, cost);
                    cout << "Edge created successfully!\n";
                } else {
                    cout << "Invalid node IDs.\n";
                }
                break;
            }
        case 3: {
            string sourceNodeId, destinationNodeId;
            cout << "Enter source node ID: ";
            cin >> sourceNodeId;
            cout << "Enter destination node ID: ";
            cin >> destinationNodeId;
        
            if (universe.nodes.count(sourceNodeId) && universe.nodes.count(destinationNodeId)) {
                Node* sourceNode = universe.nodes[sourceNodeId];
                Node* destinationNode = universe.nodes[destinationNodeId];
        
                shortestPath(&galaxy, sourceNode, destinationNode);
            } else {
                cout << "Invalid node IDs.\n";
            }
            break;
        }
        
        // Case 4: Find Shortest Path (Inter-Galaxy)
        case 4: {
            string sourceNodeId, destinationNodeId;
            cout << "Enter source node ID: ";
            cin >> sourceNodeId;
            cout << "Enter destination node ID: ";
            cin >> destinationNodeId;
        
            if (universe.nodes.count(sourceNodeId) && universe.nodes.count(destinationNodeId)) {
                Node* sourceNode = universe.nodes[sourceNodeId];
                Node* destinationNode = universe.nodes[destinationNodeId];
        
                // Define a new Universe and Galaxy for inter-galactic paths
                Universe interGalacticUniverse;
                Galaxy interGalacticGalaxy(&interGalacticUniverse);
        
                // Populate inter-galactic Galaxy with required nodes and edges
                // You need to add the necessary nodes and edges between galaxies here
        
                shortestPath(&interGalacticGalaxy, sourceNode, destinationNode);
            } else {
                cout << "Invalid node IDs.\n";
            }
            break;
        }
            case 5:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }
    }
    // Clean up
    delete A4;
    delete B4;
    delete C4;

    return 0;
}
