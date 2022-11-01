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
// modified to be a queue, simple linked list implementation of a queue

#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include "node.h"
#include "vertex.h"

typedef struct Queue
{
    Node *front;
    Node *rear;
} Queue;

// might not want to expose this method as a public interface
// Queue_createElement(x, y);

Queue *Queue_makeQueue();
void Queue_enqueue(int x, int y, Queue *q);

// Note: does not free memory allocated for the element
// client application typically wants to process the element
// so client will free the memory using Node_freeNode()
Node *Queue_dequeue(Queue *q);

void Queue_display(Queue *q);
void Queue_destroy(Queue *q);

#endif
