#include "pstar.h"

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

std::vector<Token> splitString(std::string input, char delim) {
    std::vector<Token> conj; 
    std::stringstream ss(input); 
    std::string variable; 

    while (std::getline(ss, variable, delim)) {
        conj.push_back(stringToToken(variable)); 
    }

    return conj; 
}

void buildPGraph(Graph& g, const std::vector<Token>& conj) {
    for (size_t i = 0; i < conj.size(); i++) {
        Token t = conj[i];
        Vertex* v = new Vertex(t.var, i); 
        g.addVertex(v);

        if (i + 1 < conj.size()) {
            Edge* e = new Edge(false, i, i+1); 
            v->addEdge(e); 
        }

        if (t.is_wildcard) {
            Edge* span = new Edge(true, i-1, i+1); 
            g.nodes[i-1]->addEdge(span); 
        }
    }
}

// /**
//  * Helper function to check if two spans overlap
//  */
// bool overlaps(std::pair<int, int> a, std::pair<int, int> b) {
//     if (a.first <= b.first < a.second || b.first <= a.first < b.second) {
//         return true; 
//     }
//     else {
//         return false; 
//     }
// }

// void computeClosure() {

// }