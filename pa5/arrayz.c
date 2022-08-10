/**
 * @file arrayz.c
 * @author Anthony Bisgood
 * @brief CSC 352. A collection of methods that perform calculations on arrays and integers
 * in c.
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "arrayz.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

/**
 * @brief Return the sum of every element in values
 * 
 * @param values 
 * @return long 
 */
long sum(long values[]) {
    int length = values[0];
    long sum = 0;
    for (int i = 1; i <= length; i++) {
        sum += values[i];
    }
    return sum;
}

/**
 * @brief Return the index of the minimum value in values
 * 
 * @param values 
 * @return long 
 */
long minil(long values[]) {
    int length = values[0];
    long minNum = INT_MAX;
    long index = -1;
    for (int i = 1; i <= length; i++) {
        if (values[i] < minNum) {
            minNum = values[i];
            index = i;
        }
    }
    return index;
}

/**
 * @brief Return the index of the minimum value in values
 * 
 * @param values 
 * @return long 
 */
long minid(double values[]) {
    int length = values[0];
    long minNum = INT_MAX;
    long index = -1;
    for (int i = 1; i <= length; i++) {
        if (values[i] < minNum) {
            minNum = values[i];
            index = i;
        }
    }
    return index;
}

/**
 * @brief Return the index of the maximum value in values
 * 
 * @param values 
 * @return long 
 */
long maxil(long values[]) {
    int length = values[0];
    long minNum = INT_MIN;
    long index = -1;
    for (int i = 1; i <= length; i++) {
        if (values[i] > minNum) {
            minNum = values[i];
            index = i;
        }
    }
    return index;
}

/**
 * @brief Return the index of the maximum value in values
 * 
 * @param values 
 * @return long 
 */
long maxid(double values[]) {
    int length = values[0];
    long minNum = INT_MIN;
    long index = -1;
    for (int i = 1; i <= length; i++) {
        if (values[i] > minNum) {
            minNum = values[i];
            index = i;
        }
    }
    return index;
}

/**
 * @brief Print the elements in values
 * 
 * @param values 
 */
void printal(long values[]) {
    int length = values[0];
    printf("%d ", length);
    int size = sizeof(&values) / sizeof(long);
    printf("%d ", size);
    printf("array: ");
    for (int i = 1; i <= length; i++) {
        printf("%ld, ", values[i]);
    }
    printf("\n");
}

/**
 * @brief Print the elements in values
 * 
 * @param values 
 */
void printad(double values[]) {
    int length = values[0];
    printf("%d ", length);
    int size = sizeof(&values) / sizeof(double);
    printf("%d ", size);
    printf("array: ");
    for (int i = 1; i <= length; i++) {
        printf("%lf, ", values[i]);
    }
    printf("\n");
}

/**
 * @brief Return the mean (average) of the elements in values
 * 
 * @param values 
 * @return double 
 */
double mean(long values[]) {
    long total = sum(values);
    double avg = (double) total/values[0];
    return avg;
}

/**
 * @brief Return the standard deviation of the elements in values
 * 
 * @param values 
 * @return double 
 */
double sdev(long values[]) {
    double theMean = mean(values);
    int length = values[0];
    double sumMeanSquaredSum = 0;
    for (int i = 1; i <= length; i++) {
        sumMeanSquaredSum += pow((values[i] - theMean), 2);
    }
    double standardDev = sqrt(sumMeanSquaredSum/length);
    return standardDev;
}