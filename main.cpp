#include "pstar.h"
#include "graph.h"

int main() {
    std::string d1 = "#.(c2,*).(c3,*).c1.c4.(c5,*).(c6,*).$";
    std::string d2 = "#.c2.c3.(c1,*).(c5,*).(c6,*).$";
    std::string d3 = "#.c2.(c3,*).(c1,*).(c4,*).c5.(c6,*).$"; 
    std::string d4 = "#.(c2,*).(c1,*).(c4,*).(c6,*).$"; 
    std::string d5 = "#.(c2,*).c3.(c1,*).(c4,*).(c5,*).c6.$"; 
    std::string d6 = "#.(c2,*).(c3,*).(c4,*).(c5,*).$"; 

    Graph* g = new Graph(); 

    std::cout << "p-graph: \n"; 
    buildPGraph(*g, splitString(d2, '.')); 
    g->printEdges(std::cout); 

    std::cout << "p*-graph: \n"; 
    computeClosure(*g); 
    g->printEdges(std::cout); 
}