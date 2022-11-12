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
    if (v == NULL)
        return;

    // ensure duplicate edges are not added
    for (size_t i = 0; i < 4; i++)
    {
        if (v->adjacencyList[i] == NULL)
            continue;
        // check and prevent adding duplicate edges
        if (v->adjacencyList[i]->x == x2 && v->adjacencyList[i]->y == y2)
            return;
    }

    Vertex *w = List_getVertex(x2, y2, graph->list);
    if (w == NULL)
        return;

    bool isEdgeOneConnected = false;

    // add v and w to each other
    for (int i = 0; i < 4; i++)
    {
        if (v->adjacencyList[i] != NULL)
            continue;
        v->adjacencyList[i] = w;
        isEdgeOneConnected = true;
        break;
    }

    if (!isEdgeOneConnected)
        return;

    bool isEdgeTwoConnected = false;

    for (int i = 0; i < 4; i++)
    {
        if (w->adjacencyList[i] != NULL)
            continue;
        w->adjacencyList[i] = v;
        isEdgeTwoConnected = true;
        break;
    }

    // undo edgeOne because edgeTwo did not connect successfully
    // this means incorrect usage of the method
    if (!isEdgeTwoConnected)
    {
        for (int i = 0; i < 4; i++)
        {
            if (v->adjacencyList[i] == w)
            {
                v->adjacencyList[i] = NULL;
                break;
            }
        }
    }
}

// function takes in 3 arguments, the x, y coordinate of the vertex
// and the graph
// returns an array of 4 Vertex pointers
// for client to interate through the adjacency list
// note: it is the responsibility of the client to NOT go beyond size of 4
Vertex **Graph_adj(int x, int y, Graph *graph)
{
    Vertex *v = List_getVertex(x, y, graph->list);
    if (v == NULL)
        return NULL;
    return v->adjacencyList;
}

void *Graph_display(Graph *graph)
{
    List_display(graph->list);
}

bool Graph_isExplored(Graph *graph)
{
    return graph->numberOfNodesVisited == graph->size;
}

Vertex *Graph_getVertex(int x, int y, Graph *g)
{
    return List_getVertex(x, y, g->list);
}

void Graph_destroy(Graph *graph)
{
    List_destroy(graph->list);
    free(graph);
}
