#include <iostream>
#include "Graph.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Graph myGraph(3,5);
    std::cout << myGraph.area() << std::endl;
    return 0;
}