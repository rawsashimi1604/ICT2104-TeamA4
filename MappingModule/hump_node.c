#include <stdlib.h>

#include "hump_node.h"

HumpNode *HumpNode_createNode(int x, int y)
{
    HumpNode *newNode = malloc(sizeof(HumpNode));
    if (!newNode)
        return NULL;

    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

void HumpNode_freeNode(HumpNode *e)
{
    free(e);
}
