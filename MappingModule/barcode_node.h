#ifndef BARCODE_NODE_HEADER
#define BARCODE_NODE_HEADER

// this is a node in a linked list
// where we are using a linked list of nodes
typedef struct BarcodeNode
{
    char data;
    char x;
    char y;
    struct BarcodeNode *next;
} BarcodeNode;

BarcodeNode *BarcodeNode_createNode(int x, int y, char data);
void BarcodeNode_freeNode(BarcodeNode *e);

#endif