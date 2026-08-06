#include <string>
#include <vector>
#include "pstar.h"
#include "graph.h"
#include "trie.h"

int main(int argc, char* argv[]) {
    // the sorted variable sequences of Table I
    std::vector<std::string> formula = {
        "#.(c2,*).(c3,*).c1.c4.(c5,*).(c6,*).$",
        "#.c2.c3.(c1,*).(c5,*).(c6,*).$",
        "#.c2.(c3,*).(c1,*).(c4,*).c5.(c6,*).$",
        "#.(c2,*).(c1,*).(c4,*).(c6,*).$",
        "#.(c2,*).c3.(c1,*).(c4,*).(c5,*).c6.$",
        "#.(c2,*).(c3,*).(c4,*).(c5,*).$"
    };

    std::vector<std::vector<Token>> conjunctions;

    for (const std::string& d : formula) {
        conjunctions.push_back(splitString(d));
    }

    Graph g;
    buildTrieGraph(g, conjunctions);

    if (argc > 1 && std::string(argv[1]) == "--dot") {
        g.printDot(std::cout);
    }
    else {
        g.printTrie(std::cout);
    }
}
