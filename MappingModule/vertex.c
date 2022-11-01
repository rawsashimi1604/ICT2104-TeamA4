#include <stdlib.h>
#include <stdbool.h>

#include "vertex.h"

Vertex *Vertex_createVertex(int x, int y)
{
    Vertex *v = malloc(sizeof(Vertex));
    if (!v)
    {
        return NULL;
    }
    v->x = x;
    v->y = y;
    v->visited = false;
    for (unsigned int i = 0; i < 4; i++)
        v->adjacencyList[i] = 0;

    return v;
}

void Vertex_deleteVertex(Vertex *v)
{
    free(v);
}
