//
// Created by thyriaen on 12/12/17.
//

#include "Graph.h"

Graph::Graph(int x, int y) {
    width = x;
    height = y;
}

int Graph::area() {
    return width*height;
}