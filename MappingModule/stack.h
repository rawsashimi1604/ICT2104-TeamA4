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

#include "vertex.h"
#include "node.h"

typedef struct Stack
{
    Node *top;
} Stack;

// might not want to explore this as a public interface
// Element *Stack_createElement(int x, int y);

Stack *Stack_makeStack(void);
void *Stack_push(int x, int y, Stack *s);

// Note: does not free memory allocated for the element
// client application typically wants to process the element
// so client will free the memory using Node_freeNode()
Node *Stack_pop(Stack *s);

// displays the top of the stack
void Stack_peak(Stack *s);
void Stack_display(Stack *s);
void Stack_destroy(Stack *s);

#endif