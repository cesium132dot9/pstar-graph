#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Edge {
public: 
    bool is_span; 
    std::vector<int> labels; 
    int dest_id; 

    Edge(bool is_span, int dest_id); 

    void addLabel(int label); 
};

class Vertex {
public: 
    int val; 
    int id; 
    std::vector<Edge*> edges; 

    Vertex(int val, int id); 

    void addEdge(Edge* new_edge); 
};

class Graph {
public: 
    std::vector<Vertex*> nodes; 

    void addVertex(Vertex* vertex); 
    void printEdges(std::ostream& os) const;  
};