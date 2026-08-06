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

    /**
     * Bookkeeping for the trie-like graph. Left at the defaults in a plain p*-graph.
     */
    int pre = 0;                // preorder number in the trie
    int post = 0;               // postorder number in the trie
    int parent_id = -1;         // parent in the trie, -1 for the root
    std::vector<int> conj_ids;  // conjunctions ending here, set on the '$' leaves

    Vertex(int val, int id);
    ~Vertex();

    Vertex(const Vertex&) = delete;
    Vertex& operator=(const Vertex&) = delete;

    void addEdge(Edge* new_edge);
};

class Graph {
public:
    std::vector<Vertex*> nodes;

    Graph() = default;
    ~Graph();

    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    void addVertex(Vertex* vertex);

    /**
     * Creates a vertex whose id is its index in nodes, and returns that id
     */
    int addNode(int val, int parent_id = -1);

    /**
     * Returns the edge of the given kind from src_id to dest_id, or nullptr
     */
    Edge* findEdge(int src_id, int dest_id, bool is_span) const;

    /**
     * Returns the child of src_id reached by a tree edge and holding the given
     * variable, or -1 when there is no such child
     */
    int findTreeChild(int src_id, int label) const;

    /**
     * True when the node has more than one tree child
     */
    bool isBranching(int id) const;

    /**
     * Returns a vector of all the spans
     */
    std::vector<Edge*> getSpans();

    void printEdges(std::ostream& os) const;

    /**
     * Prints the trie-like graph: the nodes with their (pre, post) encoding,
     * then the tree edges, then the spans
     */
    void printTrie(std::ostream& os) const;

    /**
     * Writes the trie-like graph as a Graphviz digraph
     */
    void printDot(std::ostream& os) const;
};

/**
 * Renders a vertex value the way the paper writes it: # for 0, $ for -1, cN otherwise
 */
std::string labelToString(int val);
