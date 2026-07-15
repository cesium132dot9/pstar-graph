#include "pstar.h"
#include "graph.h"

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
std::vector<Token> stringToToken(std::string input, char delim) {
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
void buildPath(Graph& g, const std::vector<Token>& conj) {
    for (size_t i = 0; i < conj.size(); i++) {
        Token t = conj[i];
        Vertex* v = new Vertex(t.var, i); 
        g.addVertex(v);

        if (i + 1 < conj.size()) {
            Edge* e = new Edge(false, i+1); 
            v->addEdge(e); 
        }
    }
}

int main() {
    std::string d1 = "#.(c2,*).(c3,*).c1.c4.(c5,*).(c6,*).$";

    Graph* g = new Graph(); 
    buildPath(*g, stringToToken(d1, '.')); 
    
    g->printEdges(std::cout); 
}