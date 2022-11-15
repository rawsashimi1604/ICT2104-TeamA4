#ifndef HUMP_NODE_HEADER
#define HUMP_NODE_HEADER

#include <stdint.h>

// this is a node in a linked list
// where we are using a linked list of nodes
typedef struct HumpNode
{
    int8_t x;
    int8_t y;
    struct HumpNode *next;
} HumpNode;

HumpNode *HumpNode_createNode(int x, int y);
void HumpNode_freeNode(HumpNode *e);

#endif