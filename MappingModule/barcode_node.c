#include <stdlib.h>

#include "barcode_node.h"

BarcodeNode *BarcodeNode_createNode(int x, int y, char data)
{
    BarcodeNode *newNode = malloc(sizeof(BarcodeNode));
    if (!newNode)
        return NULL;

    newNode->x = x;
    newNode->y = y;
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void BarcodeNode_freeNode(BarcodeNode *e)
{
    free(e);
}
