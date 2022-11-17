/*
 * ema_filter.c
 *
 *  Created on: 17 Nov 2022
 *      Author: loowe
 */

/*************************************************************
 * INCLUDES
 */
#include "Utility/Filters/ema_filter.h"

/*************************************************************
 * VARIABLES
 */
bool isFirstValue = true;

/*************************************************************
 * FUNCTIONS
 */

float Filter_EMAFilter(Queue* queue, float value) {

    // If the EMA queue is not full, we cannot calculate,
    // Return SMA avg
    float smaFilterValue = Filter_SMAFilter(queue);

    if (!isFull(queue)) {
        return smaFilterValue;
    }

    // If its the first time we are calculating EMA
    if (isFirstValue) {
        isFirstValue = false;
        previousEMA = smaFilterValue;
        return previousEMA;
    }

    // We have a full queue now...
    // We can calculate EMA values...
    float emaValue;

    // Calculate EMA
    // (1-a) * prev + (a) * curr
    emaValue =
            (1 - EMA_FILTER_WEIGHTAGE) * previousEMA +
            EMA_FILTER_WEIGHTAGE * value;

    // Set the next previousEMA value...
    previousEMA = emaValue;
    return emaValue;
}
