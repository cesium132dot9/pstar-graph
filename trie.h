#pragma once

#include <vector>
#include "graph.h"
#include "pstar.h"

/**
 * Builds the trie-like graph G over every conjunction in D.
 *
 * conjunctions[i] is the sorted variable sequence of D(i+1), sentinels included.
 * The trie is built over the main paths, so a variable and the same variable
 * written as a wildcard share a node. Every tree edge and every span carries the
 * 1-based numbers of the conjunctions it belongs to.
 */
void buildTrieGraph(Graph& g, const std::vector<std::vector<Token>>& conjunctions);

/**
 * True when v is a descendant of u, decided with the (pre, post) encoding
 */
bool isDescendant(const Graph& g, int u, int v);

/**
 * Returns the ids of the nodes with more than one tree child, in preorder
 */
std::vector<int> branchingNodes(const Graph& g);
