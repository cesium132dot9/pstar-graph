#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "graph.h"

class Graph; 

struct Token { 
    int var; 
    bool is_wildcard; 
};

/**
 * Splits the input string into a vector of Tokens
 */
std::vector<Token> splitString(std::string input); 

/**
 * Build the p-graph
 */
void buildPGraph(Graph& g, const std::vector<Token>& conj); 

/**
 * Compute the union of the transitive closure of p and S*
 */
void computeClosure(Graph& p); 