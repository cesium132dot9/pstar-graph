#pragma once

#include <vector>

//TODO: destructors
class Edge {
public: 
    bool is_span; 
    std::vector<int> labels; 
    Edge* next; 
    int dest; 

    Edge(bool is_span, int dest); 

    void addLabel(int label); 
};

class Vertex {
public: 
    int val; 
    Edge* edges; 

    Vertex(int val); 

    void addEdge(Edge* new_edge); 
};

class Graph {
public: 
    std::vector<Vertex*> nodes; 

    void addVertex(Vertex* vertex); 
};