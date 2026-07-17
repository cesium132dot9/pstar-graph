#include "graph.h"

Edge::Edge(bool is_span, int src_id, int dest_id) : is_span(is_span), src_id(src_id), dest_id(dest_id) {}

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

std::vector<Edge*> Graph::getSpans() {
    std::vector<Edge*> spans; 

    for (Vertex* v : nodes) {
        for (Edge* e : v->edges) {
            if (e->is_span) {
                spans.push_back(e); 
            }
        }
    }
    return spans; 
}

void Graph::printEdges(std::ostream& os) const {
    os << "src_val src_id -> dest_val dest_id [is_span] [labels]\n"; 
    for (Vertex* v : nodes) {
        for (Edge* e : v->edges) {
            os << v->val << " " << v->id << " -> "; 
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