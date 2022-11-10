#include <stdlib.h>

#include "node.h"

Node *Node_createNode(int x, int y)
{
    Vertex *newVertex = Vertex_createVertex(x, y);
    Node *newNode = malloc(sizeof(Node));
    if (!newNode)
        return NULL;

    newNode->data = newVertex;
    newNode->next = NULL;
    return newNode;
}

void Node_freeNode(Node *e)
{
    Vertex_deleteVertex(e->data);
    free(e);
}
