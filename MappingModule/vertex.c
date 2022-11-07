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

void Vertex_deleteVertex(Vertex *v)
{
    free(v);
}
