/*
 * ema_filter.h
 *
 *  Created on: 17 Nov 2022
 *      Author: loowe
 */

#ifndef UTILITY_FILTERS_EMA_FILTER_H_
#define UTILITY_FILTERS_EMA_FILTER_H_

/*************************************************************
 * INCLUDES
 */
#include <stdbool.h>
#include <Utility/DataStructures/float_queue.h>

#include "Utility/Filters/sma_filter.h"



/*************************************************************
 * DEFINITIONS
 */
#define EMA_FILTER_WEIGHTAGE    0.20f

/*************************************************************
 * VARIABLES
 */
float previousEMA;

/*************************************************************
 * FUNCTIONS
 */

float Filter_EMAFilter(FloatQueue* queue, float value);

#endif /* UTILITY_FILTERS_EMA_FILTER_H_ */
