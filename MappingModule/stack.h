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

#ifndef STACK_HEADER
#define STACK_HEADER

#include <stdint.h>

#include "vertex.h"
#include "node.h"

typedef struct Stack
{
    uint8_t size;
    Node *top;
} Stack;

Stack *Stack_makeStack(void);

// takes in an actual vertex by reference (will not create a copy)
bool Stack_push(Vertex *v, Stack *s);

// will return the actual vertex by reference (will affect the real graph)
Vertex *Stack_pop(Stack *s);

// displays the top of the stack
void Stack_peak(Stack *s);
void Stack_display(Stack *s);
void Stack_destroy(Stack *s);

// original destroy
// will free the memory allocated for the vertex inside the stack
// for testing purposes only
void Stack_destroy2(Stack *s);

#endif