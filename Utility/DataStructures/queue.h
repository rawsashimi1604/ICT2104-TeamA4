/*
 * queue.h
 *
 *  Created on: 17 Nov 2022
 *      Author: loowe
 */

#ifndef UTILITY_DATASTRUCTURES_QUEUE_H_
#define UTILITY_DATASTRUCTURES_QUEUE_H_

// https://www.geeksforgeeks.org/introduction-and-array-implementation-of-queue/
/*************************************************************
 * DEFINITIONS
 */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

/*************************************************************
 * STRUCTS
 */
struct Queue {
    int front, rear, size;
    unsigned capacity;
    float* array;
};

typedef struct Queue Queue;

/*************************************************************
 * FUNCTIONS
 */

struct Queue* createQueue(unsigned capacity);
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, int item);
float dequeue(struct Queue* queue);
float front(struct Queue* queue);
float rear(struct Queue* queue);

#endif /* UTILITY_DATASTRUCTURES_QUEUE_H_ */
