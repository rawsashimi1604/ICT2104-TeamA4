/*
 * sma_filter.c
 *
 *  Created on: 17 Nov 2022
 *      Author: loowe
 */
/*************************************************************
 * INCLUDES
 */
#include "Utility/Filters/sma_filter.h"

/*************************************************************
 * FUNCTIONS
 */
float Filter_SMAFilter(Queue* queue){

    int i;
    float sum = 0;
    float avgSum = 0;

    for (i = 0; i < queue->size; i++){
        sum += queue->array[i];
    }

    avgSum = sum / queue->size;
    return avgSum;
}
