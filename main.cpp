#include "pstar.h"
#include "graph.h"

int main() {
    std::string d1 = "#.(c2,*).(c3,*).c1.c4.(c5,*).(c6,*).$";

    Graph* g = new Graph(); 
    buildP(*g, splitString(d1, '.')); 

    g->printEdges(std::cout); 
}