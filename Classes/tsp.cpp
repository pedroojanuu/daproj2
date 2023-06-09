#include "Graph.h"
#include "VertexEdge.h"
#include <vector>
#include "two-opt.h"


#include <queue>

std::vector<unsigned int> tspMST(const Graph& graph) {
    std::vector<unsigned int> order;
    if (graph.getNumVertex() == 0)
        return order;

    std::priority_queue<std::pair<double, Vertex*>, std::vector<std::pair<double, Vertex*>>, std::greater<>> pq;
    std::vector<bool> visited(graph.getNumVertex(), false);

    // Start with the first vertex in the graph
    visited[0] = true;
    for (Edge* e : graph.findVertex(0)->getAdj()) {
        pq.push(std::make_pair(e->getDist(), e->getDest()));
    }

    while (!pq.empty()) {
        Vertex* curr = pq.top().second;
        pq.pop();

        if (visited[curr->getId()])
            continue;

        visited[curr->getId()] = true;
        order.push_back(curr->getId());

        for (Edge* e : curr->getAdj()) {
            if (!visited[e->getDest()->getId()]) {
                pq.push(std::make_pair(e->getDist(), e->getDest()));
            }
        }
    }

    return order;
}

double tspApproximation(const Graph& graph) {
    std::vector<unsigned int> order = tspMST(graph);
    double totalCost = 0.0;

    double distf = (get_edge(graph.findVertex(0), graph.findVertex(order[0]))->getDist());
    totalCost += distf;

    unsigned int numVertices = order.size();
    for (unsigned int i = 0; i < numVertices - 1; i++) {
        unsigned int curr = order[i];
        unsigned int next = order[i + 1];
        if (get_edge(graph.findVertex(curr), graph.findVertex(next))== nullptr) {
            double dist = haversine(graph.findVertex(curr)->getLat(), graph.findVertex(curr)->getLon(),
                                    graph.findVertex(next)->getLat(), graph.findVertex(next)->getLon());
            totalCost += dist;
        }
        else {
            double dist1 = (get_edge(graph.findVertex(curr), graph.findVertex(next))->getDist());
            totalCost += dist1;
        }
    }

    unsigned int first = 0;
    unsigned int last = order.back();
    if (get_edge(graph.findVertex(first), graph.findVertex(last))== nullptr) {
        double dist = haversine(graph.findVertex(first)->getLat(), graph.findVertex(first)->getLon(),
                                graph.findVertex(last)->getLat(), graph.findVertex(last)->getLon());
        totalCost += dist;
    }
    else {
        double dist1 = (get_edge(graph.findVertex(first), graph.findVertex(last))->getDist());
        totalCost += dist1;
    }

    return totalCost;
}
