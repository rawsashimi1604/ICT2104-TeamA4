#ifndef VERTEX_HEADER
#define VERTEX_HEADER

#include <stdbool.h>

// this is a graph vertex
typedef struct Vertex
{
    // x and y are coordinates in the maze
    int x;
    int y;
    bool visited;

    // holds the address of adjacent vertices
    struct Vertex *adjacencyList[4];
} Vertex;

Vertex *Vertex_createVertex(int x, int y);
void Vertex_deleteVertex(Vertex *v);

#endif