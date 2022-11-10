#ifndef BARCODE_NODE_HEADER
#define BARCODE_NODE_HEADER

// this is a node in a linked list
// where we are using a linked list of nodes
// to represent our graph
typedef struct BarcodeNode
{
    int x;
    int y;
    char data;
    struct BarcodeNode *next;
} BarcodeNode;

BarcodeNode *BarcodeNode_createNode(int x, int y, char data);
void BarcodeNode_freeNode(BarcodeNode *e);

#endif