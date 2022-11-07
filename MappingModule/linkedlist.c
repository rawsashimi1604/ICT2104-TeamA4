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
// added vertex struct
// added createVertex()
// modified add, delete, and display functions to support vertex struct coordinates
// modified delete to check for memory leaks
// no memory leaks currently known

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "linkedlist.h"

List *List_makeList(void)
{
    List *list = malloc(sizeof(List));
    if (!list)
    {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void List_display(List *list)
{
    Node *current = list->head;
    if (list->head == NULL)
        return;
    for (; current != NULL; current = current->next)
    {
        printf("coord: (%d, %d) | visited: %s | adjacent: (%d, %d), (%d, %d), (%d, %d), (%d, %d)\n",
               current->data->x,
               current->data->y,
               current->data->visited ? "yes" : "no",
               current->data->adjacencyList[0] == NULL ? -100 : current->data->adjacencyList[0]->x,
               current->data->adjacencyList[0] == NULL ? -100 : current->data->adjacencyList[0]->y,
               current->data->adjacencyList[1] == NULL ? -100 : current->data->adjacencyList[1]->x,
               current->data->adjacencyList[1] == NULL ? -100 : current->data->adjacencyList[1]->y,
               current->data->adjacencyList[2] == NULL ? -100 : current->data->adjacencyList[2]->x,
               current->data->adjacencyList[2] == NULL ? -100 : current->data->adjacencyList[2]->y,
               current->data->adjacencyList[3] == NULL ? -100 : current->data->adjacencyList[3]->x,
               current->data->adjacencyList[3] == NULL ? -100 : current->data->adjacencyList[3]->y);
    }
}

// customized add for our car, made especially for updateMap() func
// adds the vertex created to the list
// but also returns the newly added vertex for the graph
// for updating adjacent list purposes
Vertex *List_addVertex(int x, int y, List *list)
{
    Node *newNode = NULL;
    if (list->head == NULL)
    {
        newNode = Node_createNode(x, y);
        if (newNode == NULL)
            return NULL;

        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        newNode = Node_createNode(x, y);
        if (newNode == NULL)
            return NULL;

        list->tail->next = newNode;
        list->tail = newNode;
    }
    return newNode->data;
}

Vertex *List_getVertex(int x, int y, List *list)
{
    Node *trav = list->head;
    while (trav != NULL)
    {
        if (trav->data->x == x && trav->data->y == y)
            return trav->data;
        trav = trav->next;
    }
    return NULL;
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

void List_delete(int x, int y, List *list)
{
    Node *current = list->head;
    Node *previous = current;
    while (current != NULL)
    {
        if (current->data->x == x && current->data->y == y)
        {
            previous->next = current->next;
            if (current == list->head)
                list->head = current->next;
            Node_freeNode(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void List_destroy(List *list)
{
    Node *current = list->head;
    Node *next = current;
    while (current != NULL)
    {
        next = current->next;
        Node_freeNode(current);
        current = next;
    }
    free(list);
}
