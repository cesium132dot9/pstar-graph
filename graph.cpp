#include "graph.h"

Edge::Edge(bool is_span, int dest_id) : is_span(is_span), dest_id(dest_id) {}

void Edge::addLabel(int label) {
    labels.push_back(label); 
}

Vertex::Vertex(int val, int id) : val(val), id(id) {}

void Vertex::addEdge(Edge* new_edge) {
    edges.push_back(new_edge); 
}

void Graph::addVertex(Vertex* vertex) {
    nodes.push_back(vertex); 
}

void Graph::printEdges(std::ostream& os) const {
    os << "src -> Val Dest [is_span] [labels]\n"; 
    for (Vertex* v : nodes) {
        for (Edge* e : v->edges) {
            os << v->val << " -> "; 
            os << nodes[e->dest_id]->val << " "; 
            os << e->dest_id; 
            os << " [" << (e->is_span ? "span" : "") << "] "; 

            os << "["; 
            for (size_t i = 0; i < e->labels.size(); i++) {
                if (i > 0) {
                    os << ", "; 
                }
                os << e->labels[i]; 
            }
            os << "]\n"; 
        }
    }
}