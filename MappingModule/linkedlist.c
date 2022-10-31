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

// TODO: merge vertex and node

// this is a graph vertex
struct vertex
{
    // x and y are coordinates in the maze
    int x;
    int y;
    bool visited;

    // holds the address of adjacent vertices
    struct vertex *adjacencyList[4];
};

// this is a node in a linked list
// where we are using a linked list of nodes
// to represent our graph
struct node
{
    struct vertex *data;
    struct node *next;
};

struct list
{
    Node *head;
};

Node *createnode(int x, int y);

Vertex *createVertex(int x, int y)
{
    Vertex *v = malloc(sizeof(Vertex));
    if (!v)
    {
        return NULL;
    }
    v->x = x;
    v->y = y;
    v->visited = false;
    for (unsigned int i = 0; i < 4; i++)
        v->adjacencyList[i] = 0;

    return v;
}

Node *createnode(int x, int y)
{
    Vertex *newVertex = createVertex(x, y);
    Node *newNode = malloc(sizeof(Node));
    if (!newNode)
    {
        return NULL;
    }
    newNode->data = newVertex;
    newNode->next = NULL;
    return newNode;
}

List *makelist()
{
    List *list = malloc(sizeof(List));
    if (!list)
    {
        return NULL;
    }
    list->head = NULL;
    return list;
}

void display(List *list)
{
    Node *current = list->head;
    if (list->head == NULL)
        return;
    for (; current != NULL; current = current->next)
    {
        printf("coord: (%d, %d) | visited: %s\n",
               current->data->x,
               current->data->y,
               current->data->visited ? "yes" : "no");
    }
}

void add(int x, int y, List *list)
{
    Node *current = NULL;
    if (list->head == NULL)
    {
        list->head = createnode(x, y);
    }
    else
    {
        current = list->head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = createnode(x, y);
    }
}

void delete(int x, int y, List *list)
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
            free(current->data);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void reverse(List *list)
{
    Node *reversed = NULL;
    Node *current = list->head;
    Node *temp = NULL;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        temp->next = reversed;
        reversed = temp;
    }
    list->head = reversed;
}
// Reversing the entire list by changing the direction of link from forward to backward using two pointers
void reverse_using_two_pointers(List *list)
{
    Node *previous = NULL;
    while (list->head)
    {
        Node *next_node = list->head->next; // points to second node in list
        list->head->next = previous;        // at initial making head as NULL
        previous = list->head;              // changing the nextpointer direction as to point backward node
        list->head = next_node;             // moving forward by next node
    }
    list->head = previous;
}
void destroy(List *list)
{
    Node *current = list->head;
    Node *next = current;
    while (current != NULL)
    {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}
