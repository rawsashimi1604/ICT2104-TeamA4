#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vertex.h"

Vertex *Vertex_createVertex(int x, int y)
{
    Vertex *v = malloc(sizeof(Vertex));
    if (v == NULL)
        return NULL;

    v->x = x;
    v->y = y;
    v->visited = false;
    for (size_t i = 0; i < 4; i++)
        v->adjacencyList[i] = NULL;

    return v;
}

void Vertex_freeVertex(Vertex *v)
{
    free(v);
}

void Vertex_writeStrToBuff(char buff[], Vertex *v)
{
    if (v == NULL)
        return;
    snprintf(buff, 100, "coord: (%d, %d) | visited: %s | adjacent: (%d, %d), (%d, %d), (%d, %d), (%d, %d)",
             v->x, v->y, v->visited ? "yes" : "no",
             v->adjacencyList[0] == NULL ? -100 : v->adjacencyList[0]->x,
             v->adjacencyList[0] == NULL ? -100 : v->adjacencyList[0]->y,
             v->adjacencyList[1] == NULL ? -100 : v->adjacencyList[1]->x,
             v->adjacencyList[1] == NULL ? -100 : v->adjacencyList[1]->y,
             v->adjacencyList[2] == NULL ? -100 : v->adjacencyList[2]->x,
             v->adjacencyList[2] == NULL ? -100 : v->adjacencyList[2]->y,
             v->adjacencyList[3] == NULL ? -100 : v->adjacencyList[3]->x,
             v->adjacencyList[3] == NULL ? -100 : v->adjacencyList[3]->y);
}
