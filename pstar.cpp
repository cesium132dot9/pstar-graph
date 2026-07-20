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

void buildPGraph(Graph& p, const std::vector<Token>& conj) {
    for (size_t i = 0; i < conj.size(); i++) {
        Token t = conj[i];
        Vertex* v = new Vertex(t.var, i); 
        p.addVertex(v);

        if (i + 1 < conj.size()) {
            Edge* e = new Edge(false, i, i+1); 
            v->addEdge(e); 
        }

        if (t.is_wildcard) {
            Edge* span = new Edge(true, i-1, i+1); 
            p.nodes[i-1]->addEdge(span); 
        }
    }
}

/**
 * Helper function to check if two spans overlap
 */
bool overlaps(std::pair<int, int> a, std::pair<int, int> b) {
    if (((a.first <= b.first) && (b.first < a.second)) || ((b.first <= a.first) && (a.first < b.second))) {
        return true; 
    }
    else {
        return false; 
    }
}

std::vector<std::pair<int, int>> spansToPairs(std::vector<Edge*> spans) {
    std::vector<std::pair<int, int>> pairs; 

    for (Edge* e : spans) {
        pairs.push_back({e->src_id, e->dest_id}); 
    }

    return pairs; 
}

void computeClosure(Graph& p) {
    std::vector<std::pair<int, int>> pairs = spansToPairs(p.getSpans()); 
    bool changed = true; 

    while (changed) {
        changed = false; 
        size_t num_spans = pairs.size(); 

        for (size_t i = 0; i < num_spans - 1; i++) {
            for (size_t j = i + 1; j < num_spans; j++) {
                std::pair<int, int> a = pairs[i]; 
                std::pair<int, int> b = pairs[j]; 
                
                if (overlaps(a, b)) {
                    std::pair<int, int> u = {std::min(a.first, b.first), std::max(a.second, b.second)}; 

                    if ((std::find(pairs.begin(), pairs.end(), u)) != pairs.end()) {
                        continue; 
                    }
                    else {
                        Edge* closure = new Edge(true, u.first, u.second); 
                        p.nodes[u.first]->addEdge(closure); 
                    }
                }
            }
        }
    }
}