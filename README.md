# p*-graph: a C++ implementation of Chen's 2-MAXSAT algorithm

This project implements the graph structures from Yangjun Chen's paper
*An Efficient Algorithm for Solving the 2-MAXSAT Problem*. The paper turns a
2-MAXSAT instance (a CNF formula) into a DNF formula, represents each
conjunction as a **p\*-graph**, and merges all of them into a single
**trie-like graph** that is then searched bottom-up for a truth assignment
satisfying as many conjunctions as possible. The paper claims this search runs
in O(n²m⁴) time.

The code currently builds the trie-like graph and reproduces the worked example
from the paper (Table I and the trie / trie-like graph figures) exactly. The
search itself is not implemented yet.

## Background in one paragraph

Each conjunction becomes a *variable sequence* such as
`#.(c2,*).(c3,*).c1.c4.(c5,*).(c6,*).$`, where `#` and `$` are start and end
sentinels, `cN` must be true, and `(cN,*)` is a wildcard (true or false).
Negated literals are dropped, since any path through them would falsify the
conjunction. In the **p-graph** the sequence is a main path, and every wildcard
gets a *span* edge that skips over it. Consecutive wildcards produce overlapping
spans, and the **p\*-graph** adds their transitive closure so that any run of
wildcards can be skipped at once. Every `#`-to-`$` path is then a truth
assignment satisfying the conjunction (variables on the path are true, the rest
false). Finally, the main paths of all p\*-graphs are merged into a trie, and
their spans are lifted onto it. Every edge records which conjunctions it belongs
to.

## Status

| Step | State |
|---|---|
| Parse a sorted variable sequence into tokens | done |
| Build a p-graph (main path + spans) | done |
| Transitive closure of spans (p\*-graph) | done |
| Trie over main paths, with conjunction labels on tree edges | done |
| Lift every S\* onto the trie, merging shared spans | done |
| Leaf conjunction sets, (pre, post) encoding, ancestor test, branching nodes | done |
| Text and Graphviz output | done |
| CNF → DNF transformation and frequency-based variable ordering | not yet (input is given pre-sorted) |
| `SEARCH`: reachable subsets, upBounds, recursive trie-like subgraphs | not yet |

## Building and running

Requires `g++` with C++20 support and `make`.

```sh
make              # builds main.exe and test.exe
./main.exe        # prints the trie-like graph for the paper's example
./test.exe        # runs the checks; prints "all checks passed" on success
make clean
```

The makefile always names the binaries `*.exe` so the same targets work on
Windows (MinGW) and on macOS/Linux.

To draw the graph with [Graphviz](https://graphviz.org/):

```sh
./main.exe --dot > graph.dot
dot -Tpng graph.dot -o graph.png
```

Tree edges are drawn solid and spans dashed; each edge is labelled with its
conjunction set, and `$` leaves are double circles listing the conjunctions
that end there.

### Sample output

`./main.exe` lists nodes in preorder, named `v0`, `v1`, … as in the paper:

```
nodes (18):
  node  label (pre, post)  parent  conjunctions
  v0    #     (1, 18)      -
  v1    c2    (2, 17)      v0
  ...
  v7    $     (8, 1)       v6      D1, D3, D5
  ...

tree edges (17):
  v0   -> v1    {1, 2, 3, 4, 5, 6}
  ...

spans (38):
  v0   => v2    {1, 5, 6}
  ...
```

## Input format

`main.cpp` holds the six sorted variable sequences from Table I. Each sequence
is a string of tokens separated by `.`:

| Token | Meaning | Stored as `Token::var` |
|---|---|---|
| `#` | start sentinel | `0` |
| `$` | end sentinel | `-1` |
| `cN` | variable N, must be true | `N` |
| `(cN,*)` | variable N, optional (wildcard) | `N`, with `is_wildcard = true` |

Sequences must already be sorted by the global variable order and have negated
literals removed.

## Project layout

| File | Contents |
|---|---|
| `graph.h`, `graph.cpp` | `Edge`, `Vertex` and `Graph` classes, lookups, and the text / Graphviz printers |
| `pstar.h`, `pstar.cpp` | Parsing sequences into `Token`s, building a p-graph, computing the span closure |
| `trie.h`, `trie.cpp` | Building the trie-like graph, (pre, post) numbering, `isDescendant`, `branchingNodes` |
| `main.cpp` | Builds the graph for the paper's example and prints it |
| `test.cpp` | Checks the result node by node and edge by edge against the paper |
| `makefile` | Build rules |

## Design notes

- **Edges point to node ids, not pointers.** `Edge::dest_id` is an index into
  `Graph::nodes`, so growing the vector never leaves an edge pointing at a
  moved object.
- **Ownership.** A `Graph` owns its vertices and each `Vertex` owns its
  outgoing edges; both free them in their destructors. Copying is disabled to
  avoid double frees, so graphs are passed by reference (`Graph&`).
- **Trie children are keyed by variable label**, not by depth. Because negated
  literals are removed, sequences have different lengths, and the same depth
  can hold different variables in different conjunctions.
- **A variable and its wildcard share a trie node.** `c3` and `(c3,*)` follow the
  same tree edge; whether the variable can be skipped is carried by the spans.
- **Labels stay sorted for free.** Conjunctions are inserted in order 1…n and
  labels are appended, so every label set is already ascending.
- **Mapping p\*-graph positions to trie nodes.** While a conjunction's main path
  is walked down the trie, `node_at[j]` records which trie node holds its j-th
  token. Its spans are then lifted with that map, and a span that already exists
  between the same two trie nodes just gains another label.

## Tests

`test.cpp` checks:

- all 18 nodes of the example: label, preorder and postorder numbers, parent;
- all 17 tree edges and 38 spans, with their exact label sets;
- that no pair of nodes has more than one edge between them;
- the conjunction sets on the four `$` leaves;
- the branching nodes (`v1`, `v2`, `v3`) and the ancestor tests worked through
  in the paper;
- the closure of D6, whose four consecutive wildcards give 10 spans;
- edge cases: a conjunction with no wildcards, a single conjunction, and an
  empty formula.

**One deliberate difference from the paper's prose.** The examples in the text
label spans `v2 => v5` and `v3 => v5` with `{5}`. D3 has the overlapping spans
(2, 4) and (3, 5), so by the closure in Definition 2 it also has (2, 5), and the
correct label is `{3, 5}`. The tests follow the definition.

## Next steps

1. Compute reachable subsets through spans (RSs) for each branching node.
2. Derive upBounds from them.
3. Build the trie-like subgraphs over each upBound and implement the recursive
   `SEARCH`, including the `merge` of results and the hash table that skips
   repeated subgraphs.
4. Add the CNF → DNF transformation and frequency-based sorting so the program
   can take a 2-MAXSAT formula directly.

## Reference

Y. Chen, *An Efficient Algorithm for Solving the 2-MAXSAT Problem*. Extended
version of Y. Chen, "The 2-MAXSAT Problem Can Be Solved in Polynomial Time,"
Proc. CSCI 2022, IEEE, pp. 473–480.