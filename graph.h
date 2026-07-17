#pragma once

#include <vector>
#include <string>
#include <iostream>

class Edge {
public: 
    bool is_span; 
    std::vector<int> labels; 
    int src_id; 
    int dest_id; 

    Edge(bool is_span, int src_id, int dest_id); 

    /**
     * Adds a singular integer to labels
     */
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

    /**
     * Returns a vector of all the spans
     */
    std::vector<Edge*> getSpans(); 

    void printEdges(std::ostream& os) const;  
};