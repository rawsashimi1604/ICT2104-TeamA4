/*
 * kalman_filter.h
 *
 *  Created on: 17 Nov 2022
 *      Author: loowe
 */

#ifndef UTILITY_FILTERS_KALMAN_FILTER_H_
#define UTILITY_FILTERS_KALMAN_FILTER_H_

/*************************************************************
 * INCLUDES
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*************************************************************
 * DEFINITIONS
 */
#define uint8    unsigned char
#define sint8    signed char
#define uint16   unsigned int
#define sint16   signed int
#define uint32   unsigned long
#define sint32   signed long

/*************************************************************
 * STRUCTS
 */
typedef struct kalman_filter_data
{
    /* Transition matrix: 2x2 */
    float Phi_matrix[4];
    /* Q covariance plant noise matrix: 2x2 */
    float Q_matrix[4];
    /* Sensitivity matrix: 1X2 */
    float H_matrix[2];
    /* Observation noise: R covariance matrix 1x1 */
    float R_matrix;
    /* P plus current covariance matrix 2x2: estimate error */
    float P_plus[4];
    /* x plus current state vector 2x1: value, speed */
    float x_plus[2];
} kalman_filter_data_s;

/*************************************************************
 * VARIABLES
 */
extern kalman_filter_data_s KALMAN_DATA;

/*************************************************************
 * FUNCTIONS
 */
float Filter_KalmanFilter(float, kalman_filter_data_s*);



#endif /* UTILITY_FILTERS_KALMAN_FILTER_H_ */
