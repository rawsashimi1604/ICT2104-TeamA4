#include <stdlib.h>

#include "node.h"

Node *Node_createNode(int x, int y)
{
    Vertex *newVertex = Vertex_createVertex(x, y);
    if (newVertex == NULL)
        return NULL;
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        Vertex_freeVertex(newVertex);
        return NULL;
    }

    newNode->data = newVertex;
    newNode->next = NULL;
    return newNode;
}

void Node_freeNode(Node *e)
{
    Vertex_freeVertex(e->data);
    free(e);
}
