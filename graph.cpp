#include "graph.h"

#include <algorithm>

Edge::Edge(bool is_span, int src_id, int dest_id) : is_span(is_span), src_id(src_id), dest_id(dest_id) {}

void Edge::addLabel(int label) {
    labels.push_back(label);
}

Vertex::Vertex(int val, int id) : val(val), id(id) {}

Vertex::~Vertex() {
    for (Edge* e : edges) {
        delete e;
    }
}

void Vertex::addEdge(Edge* new_edge) {
    edges.push_back(new_edge);
}

Graph::~Graph() {
    for (Vertex* v : nodes) {
        delete v;
    }
}

void Graph::addVertex(Vertex* vertex) {
    nodes.push_back(vertex);
}

int Graph::addNode(int val, int parent_id) {
    int id = static_cast<int>(nodes.size());
    Vertex* v = new Vertex(val, id);
    v->parent_id = parent_id;
    nodes.push_back(v);

    return id;
}

Edge* Graph::findEdge(int src_id, int dest_id, bool is_span) const {
    for (Edge* e : nodes[src_id]->edges) {
        if (e->dest_id == dest_id && e->is_span == is_span) {
            return e;
        }
    }

    return nullptr;
}

int Graph::findTreeChild(int src_id, int label) const {
    for (Edge* e : nodes[src_id]->edges) {
        if (!e->is_span && nodes[e->dest_id]->val == label) {
            return e->dest_id;
        }
    }

    return -1;
}

bool Graph::isBranching(int id) const {
    int children = 0;

    for (Edge* e : nodes[id]->edges) {
        if (!e->is_span) {
            children++;
        }
    }

    return children > 1;
}

std::vector<Edge*> Graph::getSpans() {
    std::vector<Edge*> spans;

    for (Vertex* v : nodes) {
        for (Edge* e : v->edges) {
            if (e->is_span) {
                spans.push_back(e);
            }
        }
    }
    return spans;
}

std::string labelToString(int val) {
    if (val == 0) {
        return "#";
    }
    if (val == -1) {
        return "$";
    }

    return "c" + std::to_string(val);
}

/**
 * Helper function naming a node the way the paper does, after its preorder
 * position. Falls back to the internal id when the graph carries no encoding.
 */
static std::string nodeName(const Vertex* v) {
    return "v" + std::to_string(v->pre > 0 ? v->pre - 1 : v->id);
}

/**
 * Helper function rendering a set of conjunction numbers as {1, 3, 5}
 */
static std::string labelSetToString(const std::vector<int>& labels) {
    std::string out = "{";

    for (size_t i = 0; i < labels.size(); i++) {
        if (i > 0) {
            out += ", ";
        }
        out += std::to_string(labels[i]);
    }

    return out + "}";
}

/**
 * Helper function right padding a field so the printed columns line up
 */
static std::string pad(const std::string& s, size_t width) {
    std::string out = s;

    while (out.size() < width) {
        out += ' ';
    }

    return out;
}

/**
 * Helper function returning the nodes in preorder
 */
static std::vector<const Vertex*> preorder(const std::vector<Vertex*>& nodes) {
    std::vector<const Vertex*> order(nodes.begin(), nodes.end());

    std::sort(order.begin(), order.end(), [](const Vertex* a, const Vertex* b) {
        if (a->pre != b->pre) {
            return a->pre < b->pre;
        }
        return a->id < b->id;
    });

    return order;
}

/**
 * Helper function collecting the edges of one kind, ordered by the preorder
 * position of their endpoints
 */
static std::vector<Edge*> edgesOfKind(const std::vector<Vertex*>& nodes, bool is_span) {
    std::vector<Edge*> edges;

    for (const Vertex* v : nodes) {
        for (Edge* e : v->edges) {
            if (e->is_span == is_span) {
                edges.push_back(e);
            }
        }
    }

    std::sort(edges.begin(), edges.end(), [&nodes](const Edge* a, const Edge* b) {
        const Vertex* a_src = nodes[a->src_id];
        const Vertex* b_src = nodes[b->src_id];

        if (a_src->pre != b_src->pre) {
            return a_src->pre < b_src->pre;
        }
        if (a->src_id != b->src_id) {
            return a->src_id < b->src_id;
        }

        const Vertex* a_dest = nodes[a->dest_id];
        const Vertex* b_dest = nodes[b->dest_id];

        if (a_dest->pre != b_dest->pre) {
            return a_dest->pre < b_dest->pre;
        }
        return a->dest_id < b->dest_id;
    });

    return edges;
}

void Graph::printEdges(std::ostream& os) const {
    os << "src_val src_id -> dest_val dest_id [is_span] [labels]\n";
    for (Vertex* v : nodes) {
        for (Edge* e : v->edges) {
            os << labelToString(v->val) << " " << v->id << " -> ";
            os << labelToString(nodes[e->dest_id]->val) << " ";
            os << e->dest_id;
            os << " [" << (e->is_span ? "span" : "") << "] ";

            os << "[";
            for (size_t i = 0; i < e->labels.size(); i++) {
                if (i > 0) {
                    os << ", ";
                }
                os << e->labels[i];
            }
            os << "]\n";
        }
    }
}

void Graph::printTrie(std::ostream& os) const {
    std::vector<Edge*> tree_edges = edgesOfKind(nodes, false);
    std::vector<Edge*> spans = edgesOfKind(nodes, true);

    os << "nodes (" << nodes.size() << "):\n";
    os << "  " << pad("node", 6) << pad("label", 6) << pad("(pre, post)", 13)
       << pad("parent", 8) << "conjunctions\n";

    for (const Vertex* v : preorder(nodes)) {
        std::string encoding = "(" + std::to_string(v->pre) + ", " + std::to_string(v->post) + ")";
        std::string parent = v->parent_id < 0 ? "-" : nodeName(nodes[v->parent_id]);

        os << "  " << pad(nodeName(v), 6) << pad(labelToString(v->val), 6) << pad(encoding, 13)
           << (v->conj_ids.empty() ? parent : pad(parent, 8));

        for (size_t i = 0; i < v->conj_ids.size(); i++) {
            if (i > 0) {
                os << ", ";
            }
            os << "D" << v->conj_ids[i];
        }
        os << "\n";
    }

    os << "\ntree edges (" << tree_edges.size() << "):\n";
    for (Edge* e : tree_edges) {
        os << "  " << pad(nodeName(nodes[e->src_id]), 5) << "-> "
           << pad(nodeName(nodes[e->dest_id]), 6) << labelSetToString(e->labels) << "\n";
    }

    os << "\nspans (" << spans.size() << "):\n";
    for (Edge* e : spans) {
        os << "  " << pad(nodeName(nodes[e->src_id]), 5) << "=> "
           << pad(nodeName(nodes[e->dest_id]), 6) << labelSetToString(e->labels) << "\n";
    }
}

void Graph::printDot(std::ostream& os) const {
    os << "digraph G {\n";
    os << "    rankdir=LR;\n";
    os << "    node [shape=circle, fontsize=10];\n";
    os << "    edge [fontsize=9];\n\n";

    for (const Vertex* v : preorder(nodes)) {
        os << "    n" << v->id << " [label=\"" << nodeName(v) << "\\n" << labelToString(v->val)
           << "\\n(" << v->pre << ", " << v->post << ")";

        for (size_t i = 0; i < v->conj_ids.size(); i++) {
            os << (i > 0 ? "," : "\\n") << "D" << v->conj_ids[i];
        }
        os << "\"";

        if (!v->conj_ids.empty()) {
            os << ", shape=doublecircle";
        }
        os << "];\n";
    }

    os << "\n";
    for (Edge* e : edgesOfKind(nodes, false)) {
        os << "    n" << e->src_id << " -> n" << e->dest_id
           << " [label=\"" << labelSetToString(e->labels) << "\"];\n";
    }

    os << "\n";
    for (Edge* e : edgesOfKind(nodes, true)) {
        os << "    n" << e->src_id << " -> n" << e->dest_id
           << " [label=\"" << labelSetToString(e->labels) << "\", style=dashed, constraint=false];\n";
    }

    os << "}\n";
}
