/*
 * Provides the "operator overloading" auxiliar functions 
 * for the data structure classes 
*/

#ifndef COMPARISON_FUNCTIONS_H
#define COMPARISON_FUNCTIONS_H

#include <stdint.h>
/**
 * @brief Opaque type for a comparison function pointer of a 
 * trichotomous binary property X.
 *
 * This function has to be implemented for each type of non-
 * trivial data. The function is expected to return -1, 0, 1.
*/
typedef int8_t (*TComparisonFun)(const void* const x, const void* const y);

/**
 * @brief trichotomous comparison of integer ordeing.
 * @param x A void pointer to an integer.
 * @param y A void pointer to an integer.
 * @return -1 if `x` is less than `y`, 0 if the two numbers are 
 * equal, or 1 if `x` is greater than `y`.
*/
int8_t intOrderComp(const void* const x, const void* const y);

#endif
