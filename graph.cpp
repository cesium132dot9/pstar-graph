#include <iostream>
#include <vector>
using namespace std;

class Edge {
public: 
    bool is_span; 
    vector<int> labels;
    Edge* next; 

    Edge(bool is_span) : is_span(is_span), next(nullptr) {}

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
        Edge* curr = edges; 
        while (curr->next != nullptr) {
            curr = curr->next; 
        }
        curr->next = new_edge; 
    }
};

class Graph {        
public: 
    vector<Vertex*> nodes; 

    void addVertex(Vertex* vertex) {
        nodes.push_back(vertex); 
    }
};

int main() {
    return 0; 
}