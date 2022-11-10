/*
MIT License

Copyright (c) 2019 Alan Skorkin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// credit goes to https://github.com/skorks/c-linked-list

// modified by Pang Ka Ho

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "barcode_linkedlist.h"

BarcodeList *BarcodeList_makeList(void)
{
    BarcodeList *list = malloc(sizeof(BarcodeList));
    if (!list)
    {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void BarcodeList_display(BarcodeList *list)
{
    BarcodeNode *current = list->head;
    if (list->head == NULL)
        return;
    for (; current != NULL; current = current->next)
    {
        printf("coord: (%d, %d) | data: %c\n",
               current->x,
               current->y,
               current->data);
    }
}

// returns the status of the operation
bool BarcodeList_addNode(int x, int y, char data, BarcodeList *list)
{
    BarcodeNode *newNode = BarcodeNode_createNode(x, y, data);
    if (newNode == NULL)
        return false;
    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    return true;
}

void BarcodeList_destroy(BarcodeList *list)
{
    BarcodeNode *current = list->head;
    BarcodeNode *next = current;
    while (current != NULL)
    {
        next = current->next;
        BarcodeNode_freeNode(current);
        current = next;
    }
    free(list);
}
