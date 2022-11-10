#ifndef GRAPH_HEADER
#define GRAPH_HEADER

#include "linkedlist.h"

typedef struct Graph
{
    List *list;
    unsigned char size;
    unsigned char numberOfNodesVisited;
} Graph;

Graph *Graph_makeGraph(void);

// customized add for our car, made especially for updateMap() func
// adds the vertex created to the list
// but also returns the vertex for the graph
// for updating adjacent list purposes
Vertex *Graph_addVertex(int x, int y, Graph *graph);
void Graph_addEdge(int x, int y, int x2, int y2, Graph *graph);
Vertex **Graph_adj(int x, int y, Graph *graph);
Vertex *Graph_getVertex(int x, int y, Graph *graph);
void *Graph_display(Graph *graph);
bool Graph_isExplored(Graph *graph);
void Graph_destroy(Graph *graph);
char *Graph_toString(Graph *graph);

#endif