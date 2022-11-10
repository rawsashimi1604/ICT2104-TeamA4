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
// modified to be a stack, simple linked list implementation of a stack

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "node.h"
#include "stack.h"

Stack *Stack_makeStack()
{
    Stack *s = malloc(sizeof(Stack));
    if (!s)
    {
        return NULL;
    }
    s->top = NULL;
    s->size = 0;
    return s;
}

void Stack_display(Stack *s)
{
    Node *current = s->top;
    if (s->top == NULL)
        return;
    for (; current != NULL; current = current->next)
    {
        printf("coord: (%d, %d) | visited: %s\n",
               current->data->x,
               current->data->y,
               current->data->visited ? "yes" : "no");
    }
}

// customized add for our car, made especially for updateMap() func
// adds the vertex created to the list
// but also returns the vertex for the graph
// for updating adjacent list purposes
bool *Stack_push(int x, int y, Stack *s)
{
    Node *newNode = Node_createNode(x, y);
    if (newNode == NULL)
        return false;

    if (s->top == NULL)
    {
        s->top = newNode;
    }
    else
    {
        newNode->next = s->top;
        s->top = newNode;
    }
    s->size++;
    return true;
}

// old add provided by the library
// void add(int x, int y, List *list)
// {
//     Node *current = NULL;
//     if (list->head == NULL)
//     {
//         list->head = createnode(x, y);
//     }
//     else
//     {
//         current = list->head;
//         while (current->next != NULL)
//         {
//             current = current->next;
//         }
//         current->next = createnode(x, y);
//     }
// }

// Note: does not free memory allocated for the element
// client application typically wants to process the element
// so client will free the memory, using Node_freeNode()
Node *Stack_pop(Stack *s)
{
    if (s->top == NULL)
        return NULL;

    Node *top = s->top;
    s->top = top->next;
    s->size--;
    return top;
}

void Stack_peak(Stack *s)
{
    if (s->top == NULL)
    {
        printf("Stack is empty.\n");
        return;
    }
    printf("coord: (%d, %d) | visited: %s\n",
           s->top->data->x,
           s->top->data->y,
           s->top->data->visited ? "yes" : "no");
}

void Stack_destroy(Stack *s)
{
    Node *current = s->top;
    Node *next = current;
    while (current != NULL)
    {
        next = current->next;
        Node_freeNode(current);
        current = next;
    }
    free(s);
}
