#ifndef NODE_HEADER
#define NODE_HEADER

#include "vertex.h"

// this is a node in a linked list
// where we are using a linked list of nodes
// to represent our graph
typedef struct Node
{
    struct Node *next;
    struct Vertex *data;
} Node;

Node *Node_createNode(int x, int y);
void Node_freeNode(Node *e);

#endif