#include <iostream>
#include <string>
#include <vector>
#include "pstar.h"
#include "graph.h"
#include "trie.h"

static int failures = 0;

static void check(bool ok, const std::string& what) {
    if (!ok) {
        failures++;
        std::cout << "FAIL: " << what << "\n";
    }
}

/**
 * The sorted variable sequences of Table I
 */
static const std::vector<std::string> kFormula = {
    "#.(c2,*).(c3,*).c1.c4.(c5,*).(c6,*).$",
    "#.c2.c3.(c1,*).(c5,*).(c6,*).$",
    "#.c2.(c3,*).(c1,*).(c4,*).c5.(c6,*).$",
    "#.(c2,*).(c1,*).(c4,*).(c6,*).$",
    "#.(c2,*).c3.(c1,*).(c4,*).(c5,*).c6.$",
    "#.(c2,*).(c3,*).(c4,*).(c5,*).$"
};

struct NodeSpec {
    int name;
    const char* label;
    int pre;
    int post;
};

struct EdgeSpec {
    int src;
    int dest;
    std::vector<int> labels;
};

/**
 * The 18 nodes of Fig. 4, named the way the paper names them
 */
static const std::vector<NodeSpec> kNodes = {
    {0, "#", 1, 18},  {1, "c2", 2, 17},  {2, "c3", 3, 12},  {3, "c1", 4, 8},
    {4, "c4", 5, 4},  {5, "c5", 6, 3},   {6, "c6", 7, 2},   {7, "$", 8, 1},
    {8, "c5", 9, 7},  {9, "c6", 10, 6},  {10, "$", 11, 5},  {11, "c4", 12, 11},
    {12, "c5", 13, 10}, {13, "$", 14, 9}, {14, "c1", 15, 16}, {15, "c4", 16, 15},
    {16, "c6", 17, 14}, {17, "$", 18, 13}
};

/**
 * The tree edges of Fig. 2, each with the conjunctions running through it
 */
static const std::vector<EdgeSpec> kTreeEdges = {
    {0, 1, {1, 2, 3, 4, 5, 6}}, {1, 2, {1, 2, 3, 5, 6}}, {1, 14, {4}},
    {2, 3, {1, 2, 3, 5}},       {2, 11, {6}},            {3, 4, {1, 3, 5}},
    {3, 8, {2}},                {4, 5, {1, 3, 5}},       {5, 6, {1, 3, 5}},
    {6, 7, {1, 3, 5}},          {8, 9, {2}},             {9, 10, {2}},
    {11, 12, {6}},              {12, 13, {6}},           {14, 15, {4}},
    {15, 16, {4}},              {16, 17, {4}}
};

/**
 * The spans of Fig. 4, each with the conjunctions it belongs to. Note that the
 * paper's prose in Example 1 and Example 2 writes v2 => v5 as {5}, but D3 has
 * the overlapping spans (2, 4) and (3, 5), so (2, 5) is in its closure too and
 * the edge is {3, 5}. Same for v3 => v5. Definition 2 is what is checked here.
 */
static const std::vector<EdgeSpec> kSpans = {
    {0, 2, {1, 5, 6}}, {0, 3, {1}},     {0, 11, {6}},    {0, 12, {6}},
    {0, 13, {6}},      {0, 14, {4}},    {0, 15, {4}},    {0, 16, {4}},
    {0, 17, {4}},      {1, 3, {1, 3}},  {1, 4, {3}},     {1, 5, {3}},
    {1, 11, {6}},      {1, 12, {6}},    {1, 13, {6}},    {1, 15, {4}},
    {1, 16, {4}},      {1, 17, {4}},    {2, 4, {3, 5}},  {2, 5, {3, 5}},
    {2, 6, {5}},       {2, 8, {2}},     {2, 9, {2}},     {2, 10, {2}},
    {2, 12, {6}},      {2, 13, {6}},    {3, 5, {3, 5}},  {3, 6, {5}},
    {3, 9, {2}},       {3, 10, {2}},    {4, 6, {1, 5}},  {4, 7, {1}},
    {5, 7, {1, 3}},    {8, 10, {2}},    {11, 13, {6}},   {14, 16, {4}},
    {14, 17, {4}},     {15, 17, {4}}
};

/**
 * Helper function mapping the paper's node names onto the internal ids, which
 * follow creation order instead of preorder
 */
static std::vector<int> namedNodes(const Graph& g) {
    std::vector<int> ids(g.nodes.size(), -1);

    for (size_t id = 0; id < g.nodes.size(); id++) {
        int pre = g.nodes[id]->pre;

        if (pre < 1 || pre > static_cast<int>(g.nodes.size())) {
            check(false, "node " + std::to_string(id) + " has no valid preorder number");
            continue;
        }
        ids[pre - 1] = static_cast<int>(id);
    }

    return ids;
}

/**
 * Helper function building the trie-like graph over a set of variable sequences
 */
static void buildFrom(Graph& g, const std::vector<std::string>& sequences) {
    std::vector<std::vector<Token>> conjunctions;

    for (const std::string& d : sequences) {
        conjunctions.push_back(splitString(d));
    }

    buildTrieGraph(g, conjunctions);
}

static void checkEdge(const Graph& g, const std::vector<int>& v, const EdgeSpec& spec, bool is_span) {
    std::string what = "v" + std::to_string(spec.src) + (is_span ? " => v" : " -> v") + std::to_string(spec.dest);

    if (v[spec.src] < 0 || v[spec.dest] < 0) {
        check(false, what + " endpoints missing");
        return;
    }

    Edge* e = g.findEdge(v[spec.src], v[spec.dest], is_span);

    if (e == nullptr) {
        check(false, what + " missing");
        return;
    }

    check(e->labels == spec.labels, what + " labels");
}

/**
 * Reproduces the trie of Fig. 2 and the trie-like graph of Fig. 4
 */
static void testFigure4() {
    Graph g;
    buildFrom(g, kFormula);

    check(g.nodes.size() == kNodes.size(), "trie-like graph has 18 nodes");
    if (g.nodes.size() != kNodes.size()) {
        return;
    }

    std::vector<int> v = namedNodes(g);

    for (const NodeSpec& spec : kNodes) {
        std::string what = "v" + std::to_string(spec.name);
        const Vertex* node = g.nodes[v[spec.name]];

        check(labelToString(node->val) == spec.label, what + " label");
        check(node->pre == spec.pre, what + " pre");
        check(node->post == spec.post, what + " post");
    }

    // every node but the root reaches its parent through the trie
    for (const Vertex* node : g.nodes) {
        if (node->id == 0) {
            check(node->parent_id == -1, "the root has no parent");
        }
        else {
            check(node->parent_id >= 0, "v" + std::to_string(node->pre - 1) + " has a parent");
        }
    }

    size_t tree_edges = 0;
    size_t spans = 0;

    for (const Vertex* node : g.nodes) {
        for (const Edge* e : node->edges) {
            if (e->is_span) {
                spans++;
            }
            else {
                tree_edges++;
            }
            check(e->src_id == node->id, "edges are stored on their source");
        }
    }

    check(tree_edges == kTreeEdges.size(), "trie-like graph has 17 tree edges");
    check(spans == kSpans.size(), "trie-like graph has 38 spans");

    for (const EdgeSpec& spec : kTreeEdges) {
        checkEdge(g, v, spec, false);
    }
    for (const EdgeSpec& spec : kSpans) {
        checkEdge(g, v, spec, true);
    }

    // no pair of nodes is joined by both a tree edge and a span, and no edge is duplicated
    for (const Vertex* node : g.nodes) {
        for (size_t i = 0; i < node->edges.size(); i++) {
            for (size_t j = i + 1; j < node->edges.size(); j++) {
                check(node->edges[i]->dest_id != node->edges[j]->dest_id,
                      "v" + std::to_string(node->pre - 1) + " has one edge per target");
            }
        }
    }

    // the conjunctions satisfied by each tree path, Lemma 2
    check(g.nodes[v[7]]->conj_ids == std::vector<int>({1, 3, 5}), "v7 carries {D1, D3, D5}");
    check(g.nodes[v[10]]->conj_ids == std::vector<int>({2}), "v10 carries {D2}");
    check(g.nodes[v[13]]->conj_ids == std::vector<int>({6}), "v13 carries {D6}");
    check(g.nodes[v[17]]->conj_ids == std::vector<int>({4}), "v17 carries {D4}");

    std::vector<int> branching = branchingNodes(g);
    check(branching == std::vector<int>({v[1], v[2], v[3]}), "the branching nodes are v1, v2, v3");

    // the ancestor tests worked through on page 5 of the paper
    check(isDescendant(g, v[2], v[9]), "v9 is a descendant of v2");
    check(!isDescendant(g, v[14], v[6]), "v6 is not a descendant of v14");
    check(!isDescendant(g, v[6], v[14]), "v14 is not a descendant of v6");
}

/**
 * The closure of D6, whose four consecutive wildcards span every pair of nodes
 * two or more steps apart
 */
static void testClosureOfD6() {
    Graph p;
    buildPGraph(p, splitString("#.(c2,*).(c3,*).(c4,*).(c5,*).$"));
    computeClosure(p);

    check(p.nodes.size() == 6, "D6 has 6 nodes");
    check(p.getSpans().size() == 10, "S6* has 10 spans");

    for (int i = 0; i < 6; i++) {
        for (int j = i + 2; j < 6; j++) {
            check(p.findEdge(i, j, true) != nullptr,
                  "S6* covers (" + std::to_string(i) + ", " + std::to_string(j) + ")");
        }
    }
}

/**
 * A conjunction without wildcards has an empty closure, which used to underflow
 * the loop bound in computeClosure
 */
static void testNoWildcards() {
    Graph p;
    buildPGraph(p, splitString("#.c2.c3.$"));
    computeClosure(p);

    check(p.nodes.size() == 4, "a conjunction without wildcards has 4 nodes");
    check(p.getSpans().empty(), "a conjunction without wildcards has no spans");

    Graph g;
    buildFrom(g, {"#.c2.c3.$"});

    check(g.nodes.size() == 4, "its trie-like graph is a single path");
    check(g.getSpans().empty(), "its trie-like graph has no spans");
    check(g.nodes[3]->conj_ids == std::vector<int>({1}), "its leaf carries {D1}");
    check(branchingNodes(g).empty(), "a single path has no branching node");
}

/**
 * A trie-like graph over one conjunction is just its p*-graph, and an empty
 * formula builds nothing
 */
static void testDegenerateInputs() {
    Graph g;
    buildFrom(g, {kFormula[0]});

    check(g.nodes.size() == 8, "one conjunction gives its own 8 nodes");
    check(g.getSpans().size() == 6, "one conjunction keeps its 6 spans");
    check(g.nodes[0]->pre == 1 && g.nodes[7]->post == 1, "the single path is numbered end to end");

    Graph empty;
    buildFrom(empty, {});
    check(empty.nodes.empty(), "an empty formula builds nothing");
}

int main() {
    testFigure4();
    testClosureOfD6();
    testNoWildcards();
    testDegenerateInputs();

    if (failures == 0) {
        std::cout << "all checks passed\n";
        return 0;
    }

    std::cout << failures << " check(s) failed\n";
    return 1;
}
