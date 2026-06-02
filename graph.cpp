#include <iostream>
#include <vector>

class Vertex {
    public: 
        int val; 
        std::vector<Vertex*> neighbours; 
        Vertex* next; 
        bool wildcard;
        std::vector<Vertex*> spans;

        Vertex(int val) : val(val), next(nullptr) {}
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

        void add_vertex(int val) {
            Vertex* v = new Vertex(val); 

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

        bool add_edge(int val1, int val2) {
            Vertex* v1 = find_vertex(val1); 
            Vertex* v2 = find_vertex(val2); 
            
            if (v1 != nullptr && v2 != nullptr) {
                v1->neighbours.push_back(v2); 
                v2->neighbours.push_back(v1); 
                return true; 
            }
            return false; 
        }

        std::vector<int> get_neighbours(int val) {
            std::vector<int> neighbour_val; 
            Vertex* v = find_vertex(val); 
            if (v!= nullptr) {
                for (Vertex* neighbour : v->neighbours) {
                    neighbour_val.push_back(neighbour->val); 
                }
            }
            return neighbour_val; 
        }

    private: 
        Vertex* find_vertex(int val) {
            Vertex* current = head; 
            while (current != nullptr) {
                if (current->val == val) {
                    return current; 
                }
            }
            return nullptr; 
        }
};

int main() {
    Graph g = Graph(); 
    g.add_vertex(0); 
    g.add_vertex(1); 
    g.add_vertex(2);
    g.add_edge(0, 1); 
    g.add_edge(0, 2); 
    g.add_edge(1, 2); 

    std::vector<int> zero_neighbours = g.get_neighbours(0); 
    std::vector<int> one_neighbours = g.get_neighbours(1); 
    std::vector<int> two_neighbours = g.get_neighbours(2); 
    for (int val : zero_neighbours) {
        std::cout << val << " "; 
    }
    std::cout << "\n"; 
    for (int val : one_neighbours) {
        std::cout << val << " "; 
    }
    std::cout << "\n"; 
    for (int val : two_neighbours) {
        std::cout << val << " "; 
    }
    std::cout << "\n"; 

    return 0; 
}