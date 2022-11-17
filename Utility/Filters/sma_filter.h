/*
 * sma_filter.h
 *
 *  Created on: 17 Nov 2022
 *      Author: loowe
 */

#ifndef UTILITY_FILTERS_SMA_FILTER_H_
#define UTILITY_FILTERS_SMA_FILTER_H_

/*************************************************************
 * INCLUDES
 */
#include "Utility/DataStructures/queue.h"

/*************************************************************
 * FUNCTIONS
 */
float Filter_SMAFilter(Queue* queue);

#endif /* UTILITY_FILTERS_SMA_FILTER_H_ */
