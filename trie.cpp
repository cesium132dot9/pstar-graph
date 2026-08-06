#include "trie.h"

#include <algorithm>

/**
 * Helper function numbering every node with its preorder and postorder position.
 * Tree children are visited in the order they were inserted, which is what makes
 * the numbering come out the same as in the paper.
 */
static void assignPrePost(Graph& g, int id, int& pre, int& post) {
    g.nodes[id]->pre = ++pre;

    for (Edge* e : g.nodes[id]->edges) {
        if (!e->is_span) {
            assignPrePost(g, e->dest_id, pre, post);
        }
    }

    g.nodes[id]->post = ++post;
}

void buildTrieGraph(Graph& g, const std::vector<std::vector<Token>>& conjunctions) {
    if (conjunctions.empty() || conjunctions[0].empty()) {
        return;
    }

    int root = g.addNode(conjunctions[0][0].var);

    for (size_t i = 0; i < conjunctions.size(); i++) {
        const std::vector<Token>& conj = conjunctions[i];

        if (conj.empty()) {
            continue;
        }

        int label = static_cast<int>(i) + 1;

        // the p*-graph of this conjunction, whose node ids are positions in conj
        Graph p;
        buildPGraph(p, conj);
        computeClosure(p);

        // first step: walk the main path down the trie, extending it where needed
        std::vector<int> node_at(conj.size());
        node_at[0] = root;
        int cur = root;

        for (size_t j = 1; j < conj.size(); j++) {
            int child = g.findTreeChild(cur, conj[j].var);
            Edge* tree_edge;

            if (child == -1) {
                child = g.addNode(conj[j].var, cur);
                tree_edge = new Edge(false, cur, child);
                g.nodes[cur]->addEdge(tree_edge);
            }
            else {
                tree_edge = g.findEdge(cur, child, false);
            }

            // a conjunction crosses a tree edge at most once, so labels stay unique
            tree_edge->addLabel(label);
            node_at[j] = child;
            cur = child;
        }

        g.nodes[cur]->conj_ids.push_back(label);

        // second step: lift S* onto the trie, merging spans shared with earlier conjunctions
        for (Edge* s : p.getSpans()) {
            int src = node_at[s->src_id];
            int dest = node_at[s->dest_id];
            Edge* span = g.findEdge(src, dest, true);

            if (span == nullptr) {
                span = new Edge(true, src, dest);
                g.nodes[src]->addEdge(span);
            }

            span->addLabel(label);
        }
    }

    int pre = 0;
    int post = 0;
    assignPrePost(g, root, pre, post);
}

bool isDescendant(const Graph& g, int u, int v) {
    return g.nodes[v]->pre > g.nodes[u]->pre && g.nodes[v]->post < g.nodes[u]->post;
}

std::vector<int> branchingNodes(const Graph& g) {
    std::vector<int> branching;

    for (size_t id = 0; id < g.nodes.size(); id++) {
        if (g.isBranching(static_cast<int>(id))) {
            branching.push_back(static_cast<int>(id));
        }
    }

    std::sort(branching.begin(), branching.end(), [&g](int a, int b) {
        return g.nodes[a]->pre < g.nodes[b]->pre;
    });

    return branching;
}
