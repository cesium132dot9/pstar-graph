#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "graph.h"

class Graph; 

struct Token {
    int var; 
    bool is_wildcard; 
};

/**
 * Splits the input string into a vector of Tokens
 */
std::vector<Token> splitString(std::string input, char delim); 

/**
 * Build the main path of the p-graph with no spans
 */
void buildP(Graph& g, const std::vector<Token>& conj); 