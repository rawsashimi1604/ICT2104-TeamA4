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

#ifndef BARCODE_LINKEDLIST_HEADER
#define BARCODE_LINKEDLIST_HEADER

#include <stdbool.h>

#include "barcode_node.h"

typedef struct BarcodeList
{
    BarcodeNode *head;
    BarcodeNode *tail;
} BarcodeList;

BarcodeList *BarcodeList_makeList(void);

// old add provided by library
// void add(int x, int y, List *list);

bool BarcodeList_addNode(int x, int y, char data, BarcodeList *list);
void BarcodeList_display(BarcodeList *list);
void BarcodeList_destroy(BarcodeList *list);

#endif
