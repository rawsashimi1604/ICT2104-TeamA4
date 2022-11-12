#ifndef VERTEX_HEADER
#define VERTEX_HEADER

#include <stdbool.h>

// this is a graph vertex
typedef struct Vertex
{
    // x and y are coordinates in the maze
    char x;
    char y;
    bool visited;

    // holds the address of adjacent vertices
    struct Vertex *adjacencyList[4];
} Vertex;

Vertex *Vertex_createVertex(int x, int y);
void Vertex_freeVertex(Vertex *v);

// will write string data to the buffer provided
void Vertex_writeStrToBuff(char buff[], Vertex *v);

#endif