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
    queue->size = 0;

    // This is important, see the enqueue
    queue->array = (float*)malloc(
        queue->capacity * sizeof(float));

    // memset queue array values
    memset(queue->array, 0, queue->capacity * sizeof(float));
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
void enqueue(struct Queue* queue, float item)
{
    // Add item to queue
    // if its not full, add to index size
    if (!isFull(queue)) {
        queue->array[queue->size] = item;

    }

    // If its full, dequeue the first element
    // mvoe all items by 1
    else {
        dequeue(queue);
        int i;
        for (i = 1; i < queue->capacity; i++) {
            queue->array[i-1] = queue->array[i];
        }

        queue->array[queue->size] = item;

    }
    queue->size++;

    printf("\nSMA Queue:\n");
    printQueue(queue);
}

// Function to remove an item from queue.
// It changes front and size
float dequeue(struct Queue* queue)
{
    // Invalid operation, queue is empty...
    if (isEmpty(queue)) {
        return FLT_MIN;
    }

    // Get the dequeued item
    float dequeuedItem = queue->array[0];

    queue->size--;

    return dequeuedItem;
}

// Print queue
void printQueue(struct Queue* queue){

    // print all values in the queue
    printf("[");
    int i;
    for (i = 0; i < queue->size; i++) {
        (i < queue->size - 1) ?
            printf("%.2f, ", queue->array[i]):
            printf("%.2f", queue->array[i]);
    }
    printf("]\n");


}
