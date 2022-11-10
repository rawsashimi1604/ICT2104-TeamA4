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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "node.h"
#include "queue.h"

Queue *Queue_makeQueue(void)
{
    Queue *q = malloc(sizeof(Queue));
    if (!q)
    {
        return NULL;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void Queue_display(Queue *q)
{
    Node *current = q->front;
    if (q->front == NULL)
        return;
    for (; current != NULL; current = current->next)
    {
        printf("coord: (%d, %d) | visited: %s\n",
               current->data->x,
               current->data->y,
               current->data->visited ? "yes" : "no");
    }
}

bool Queue_enqueue(Vertex *v, Queue *q)
{
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL)
        return false;
    newNode->data = v;
    newNode->next = NULL;
    if (q->front == NULL)
    {
        q->front = newNode;
        q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
    return true;
}

Vertex *Queue_dequeue(Queue *q)
{
    if (q->front == NULL)
        return NULL;

    Node *front = q->front;
    q->front = front->next;
    q->size--;
    Vertex *v = front->data;
    free(front);
    return v;
}

void Queue_destroy(Queue *q)
{
    Node *current = q->front;
    Node *next = current;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(q);
}

// Original destroy that WILL free the memory inside the stack
// do not use, use Stack_destory instead
// this is for testing purposes only
void Queue_destroy2(Queue *q)
{
    Node *current = q->front;
    Node *next = current;
    while (current != NULL)
    {
        next = current->next;
        Node_freeNode(current);
        current = next;
    }
    free(q);
}
