#include "graph.h"

Edge::Edge(bool is_span, int dest) : is_span(is_span), next(nullptr), dest(dest) {}

void Edge::addLabel(int label) {
    labels.push_back(label); 
}


Vertex::Vertex(int val) : val(val), edges(nullptr) {}

void Vertex::addEdge(Edge* new_edge) {
    if (edges == nullptr) {
        edges = new_edge; 
        return; 
    }

    Edge* curr = edges; 
    while (curr->next != nullptr) {
        curr = curr->next; 
    }
    curr->next = new_edge; 
}


void Graph::addVertex(Vertex* vertex) {
    nodes.push_back(vertex); 
}


//TODO: test out in main
int main() {
    return 0; 
}