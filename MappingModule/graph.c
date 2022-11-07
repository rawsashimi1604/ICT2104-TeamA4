#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"

Graph *Graph_makeGraph(void)
{
    Graph *graph = malloc(sizeof(Graph));
    if (graph == NULL)
        return NULL;

    graph->list = List_makeList();
    if (graph->list == NULL)
        return NULL;

    graph->numberOfNodesVisited = 0;
    graph->size = 0;

    return graph;
}

// customized add for our car, made especially for updateMap() func
// adds the vertex created to the list
// but also returns the vertex for the graph
// for updating adjacent list purposes
Vertex *Graph_addVertex(int x, int y, Graph *graph)
{
    // check if vertex already exists, if not don't add it to the graph
    Vertex *existingVertex = List_getVertex(x, y, graph->list);
    if (existingVertex != NULL)
        return existingVertex;
    Vertex *newVertex = List_addVertex(x, y, graph->list);
    if (newVertex != NULL)
        graph->size++;

    return newVertex;
}

// adds a bidirectional link for both coordinates, if it exists
void Graph_addEdge(int x, int y, int x2, int y2, Graph *graph)
{
    Vertex *v = List_getVertex(x, y, graph->list);

    Vertex **adj = Graph_adj(v->x, v->y, graph);
    for (size_t i = 0; i < 4; i++)
    {
        if (v->adjacencyList[i] == NULL)
            continue;
        // check and prevent adding duplicate edges
        if (adj[i]->x == x2 && adj[i]->y == y2)
            return;
    }

    Vertex *w = List_getVertex(x2, y2, graph->list);
    if (v == NULL || w == NULL)
        return;

    bool isValidEdge = false;
    // add v and w to each other
    for (int i = 0; i < 4; i++)
    {
        if (v->adjacencyList[i] != NULL)
            continue;
        v->adjacencyList[i] = w;
        isValidEdge = true;
        break;
    }

    if (!isValidEdge)
        return;

    for (int i = 0; i < 4; i++)
    {
        if (w->adjacencyList[i] != NULL)
            continue;
        w->adjacencyList[i] = v;
        break;
    }
}

Vertex **Graph_adj(int x, int y, Graph *graph)
{
    Vertex *v = List_getVertex(x, y, graph->list);
    if (v == NULL)
        return NULL;
    return v->adjacencyList;
}

char *Graph_toString(Graph *graph);
void *Graph_display(Graph *graph)
{
    List_display(graph->list);
}

bool Graph_isExplored(Graph *graph)
{
    return graph->numberOfNodesVisited == graph->size;
}

void Graph_destroy(Graph *graph)
{
    List_destroy(graph->list);
    free(graph);
}
