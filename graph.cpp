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

int main() {
    std::string d1 = "#.(c2,*).(c3,*).c1.c4.(c5,*).(c6,*).$";

    std::vector<Token> conjunction; 
    conjunction = stringStringToToken(d1, '.'); 
    for (Token& t : conjunction) {
        std::cout << t.var << (t.is_wildcard ? "*" : "") << "\n";
    }
}