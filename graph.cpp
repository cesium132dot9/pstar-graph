#include <iostream>
#include <vector>

class Vertex {
    public: 
        std::vector<int> vals; 
        std::vector<Edge*> edges; 
        Vertex* next; 

        Vertex(std::vector<int> vals) : vals(vals), next(nullptr) {}
};

class Edge {
    public: 
        bool is_span; 
        Vertex* prev; 
        Vertex* next;

    Edge(bool is_span, Vertex* prev, Vertex* next) : is_span(is_span), prev(prev), next(next) {}
};

class Graph {        
    public: 
        Vertex* head; 

        Graph() : head(nullptr) {}

        ~Graph() {
            Vertex* current = head; 
            while (current != nullptr) { 
                Vertex* next = current->next; 
                delete current; 
                current = next; 
            }
        }

        void add_vertex(std::vector<int> vals) {
            Vertex* v = new Vertex(vals); 

            if (head == nullptr) {
                head = v; 
                return; 
            }
            else {
                Vertex* current = head; 
                while (current != nullptr) {
                    current = current->next; 
                }
                current->next = v; 
            }
        }

        //TODO: rerewrite add_edge
        // bool add_edge(int val1, int val2) {
        //     Vertex* v1 = find_vertex(val1); 
        //     Vertex* v2 = find_vertex(val2); 
            
        //     if (v1 != nullptr && v2 != nullptr) {
        //         v1->neighbours.push_back(v2); 
        //         v2->neighbours.push_back(v1); 
        //         return true; 
        //     }
        //     return false; 
        // }

        //TODO: rewrite get_neighbours
        // std::vector<int> get_neighbours(int val) {
        //     std::vector<int> neighbour_val; 
        //     Vertex* v = find_vertex(val); 
        //     if (v!= nullptr) {
        //         for (Vertex* neighbour : v->neighbours) {
        //             neighbour_val.push_back(neighbour->val); 
        //         }
        //     }
        //     return neighbour_val; 
        // }

    private: 
        //TODO: rewrite find_vertex
        // Vertex* find_vertex(int val) {
        //     Vertex* current = head; 
        //     while (current != nullptr) {
        //         if (current->val == val) {
        //             return current; 
        //         }
        //     }
        //     return nullptr; 
        // }
};

int main() {
    return 0; 
}