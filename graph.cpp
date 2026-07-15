#include "graph.h"

Edge::Edge(bool is_span, int dest_id) : is_span(is_span), next(nullptr), dest_id(dest_id) {}

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

// void Graph::printEdges() {
//     for (Vertex* v : nodes) {
//         std::cout << v->val << " "; 
//         for (Edge* e : v->edges) {
//             std::cout << e->dest_id; 
//             std::cout << " [" << (e->is_span ? "span" : "") << "]  "; 
//             std::cout << 
//         }
//     }
// }


struct Token {
    int var; // 0 for #, -1 for $
    bool is_wildcard; 
};

/**
 * Helper function for split which takes a string and returns it as a Token* object.
 */
Token stringToToken(std::string variable) {
    Token token;  
    if (variable == "#") {
        token.var = 0; 
        token.is_wildcard = false; 
    }
    else if (variable == "$") {
        token.var = -1; 
        token.is_wildcard = false; 
    }
    else if (variable[0] == '(') {
        token.var = std::stoi(variable.substr(2, (variable.find(",") - 2))); 
        token.is_wildcard = true; 
    }
    else {
        token.var = std::stoi(variable.substr(1));
        token.is_wildcard = false; 
    }

    return token; 
}

/**
 * Splits the input string into a vector of Tokens
 */
std::vector<Token> stringStringToToken(std::string input, char delim) {
    std::vector<Token> conj; 
    std::stringstream ss(input); 
    std::string variable; 

    while (std::getline(ss, variable, delim)) {
        conj.push_back(stringToToken(variable)); 
    }

    return conj; 
}

/**
 * Build the main path of the p-graph with no spans
 */
Graph* buildPath(const std::vector<Token>& conj) {
    Graph* g = new Graph(); 

    for (int i = 0; i < conj.size(); i++) {
        Token t = conj[i];
        Vertex* v = new Vertex(t.var, i); 
        g->addVertex(v);

        if (i + 1 < conj.size()) {
            Edge* e = new Edge(false, i+1); 
            v->addEdge(e); 
        }
    }
    return g; 
}

int main() {
    std::string d1 = "#.(c2,*).(c3,*).c1.c4.(c5,*).(c6,*).$";

    Graph* g = buildPath(stringStringToToken(d1, '.')); 

    for (Vertex* v : g->nodes) {
        std::cout << v->val << " "; 
        for (Edge* e : v->edges) {
            std::cout << g->nodes[e->dest_id]->val << "\n"; 
        }
    }
}