#include "graph.h"

class Edge {
public: 
    bool is_span; 
    std::vector<int> labels;
    Edge* next; 
    int dest; 

    Edge(bool is_span, int dest) : is_span(is_span), next(nullptr), dest(dest) {}

    void addLabel(int label) {
        labels.push_back(label); 
    }
};

class Vertex {
public: 
    int val; 
    Edge* edges; 

    Vertex(int val) : val(val), edges(nullptr) {}

    void addEdge(Edge* new_edge) {
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
};

class Graph {        
public: 
    std::vector<Vertex*> nodes; 

    void addVertex(Vertex* vertex) {
        nodes.push_back(vertex); 
    }
};

int main() {
    return 0; 
}