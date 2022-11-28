#ifndef GRAPH_HEADER
#define GRAPH_HEADER

#include <stdint.h>

#include "linkedlist.h"

typedef struct Graph
{
    uint8_t size;
    uint8_t numberOfNodesVisited;
    List *list;
} Graph;

Graph *Graph_makeGraph(void);

// customized add for our car, made especially for updateMap() func
// adds the vertex created to the list
// but also returns the vertex for the graph
// for updating adjacent list purposes
Vertex *Graph_addVertex(int x, int y, Graph *graph);

// adds a undirected bidirectional edge between given two verticies in the graph
// if it exists
void Graph_addEdge(Vertex *vertex1, Vertex *vertex2);

// returns the adjacency list given the x and y coordinate of a vertex
Vertex **Graph_adj(int x, int y, Graph *graph);
Vertex *Graph_getVertex(int x, int y, Graph *graph);
void Graph_display(Graph *graph);
bool Graph_isExplored(Graph *graph);
void Graph_destroy(Graph *graph);

void Graph_addEdgeOld(int x1, int y1, int x2, int y2, Graph *graph);

#endif
