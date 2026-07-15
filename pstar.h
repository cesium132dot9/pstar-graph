#pragma once

#include <vector>
#include <string>

class Graph; 

struct Token {
    int var; 
    bool is_wildcard; 
};

/**
 * Splits the input string into a vector of Tokens
 */
std::vector<Token> stringToToken(std::string input, char delim); 

/**
 * Build the main path of the p-graph with no spans
 */
void buildPath(Graph& g, const std::vector<Token>& conj); 