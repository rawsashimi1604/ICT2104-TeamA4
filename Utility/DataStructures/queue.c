/*
 * queue.c
 *
 *  Created on: 17 Nov 2022
 *      Author: loowe
 */

#include "queue.h"


// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (float*)malloc(
        queue->capacity * sizeof(float));
    return queue;
}

// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
float dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return FLT_MIN;
    float item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
float front(struct Queue* queue)
{
    if (isEmpty(queue))
        return FLT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
float rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return FLT_MIN;
    return queue->array[queue->rear];
}
