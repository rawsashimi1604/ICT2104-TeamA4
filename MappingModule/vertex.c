#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

    char temp[10] = {0};
    snprintf(buff, 100, "(%d, %d): ", v->x, v->y);
    for (size_t i = 0; i < 4; i++)
    {
        if (v->adjacencyList[i] == NULL)
            continue;
        snprintf(temp, 10, "(%d, %d) ", v->adjacencyList[i]->x, v->adjacencyList[i]->y);
        strcat(buff, temp);
    }
}
