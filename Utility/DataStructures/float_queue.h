/*
 * queue.h
 *
 *  Created on: 17 Nov 2022
 *      Author: loowe
 */

#ifndef UTILITY_DATASTRUCTURES_FLOAT_QUEUE_H_
#define UTILITY_DATASTRUCTURES_FLOAT_QUEUE_H_

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
struct FloatQueue {
    int size;
    unsigned capacity;
    float* array;
};

typedef struct FloatQueue FloatQueue;

/*************************************************************
 * FUNCTIONS
 */

struct FloatQueue* createQueue(unsigned capacity);
int isFull(struct FloatQueue* queue);
int isEmpty(struct FloatQueue* queue);
void enqueue(struct FloatQueue* queue, float item);
float dequeue(struct FloatQueue* queue);
float peek(struct FloatQueue* queue);
void printQueue(struct FloatQueue* queue);

#endif /* UTILITY_DATASTRUCTURES_FLOAT_QUEUE_H_ */
