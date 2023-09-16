#pragma once
#include <stdbool.h>
#include <stdlib.h>

#ifndef LINEAR_H
#define LINEAR_H
/* 
 * LINKED LISTS 
*/ 

typedef struct LinkedList LinkedList;

LinkedList* llCreate(const bool as_copy);

void llDestroy(LinkedList* ll);

bool llPrepend(LinkedList* ll, void* data, const size_t data_size);

void* llPeek(LinkedList* ll, const bool as_copy, size_t* const data_size);

void* llPop(LinkedList* ll, size_t* const data_size);

/** 
 * Double Linked Lists 
*/

/*
 * DYNAMIC ARRAYS 
*/ 
typedef struct DynArr DynArr;
/**
 * @brief Crate a new dynamic allocated array
 * @param capacity
 *  the minimal capacity of the array. The actual capacity stored is the
 *  least power of two larger than the capacity.
 * @param data_size
 *  the size of the data to be stored.
 * @param as_copy
 *  determines if the stored data should be copied at insertions or updates.
 * @return 
 *  a new dynmaic allocated array. If there were any problems allocating memory or 
 *  overflow of `size_t`, it will return a NULL array.
*/ 
DynArr* dynarrCreate(const size_t capacity, const size_t data_size, const bool as_copy);

/**
 * @brief deleting function for a dynamic allocated array.
 * @param da
 *  the array to be destroyed.
*/ 
void dynarrDestroy(DynArr* da);

bool dynarrPush(DynArr* da, void* data);

bool dynarrSet(DynArr* da, const size_t index, void* data);

void* dynarrGet(DynArr* da, const size_t index, const bool get_copy);

void* dynarrPop(DynArr* da, size_t index);

/**
 * STACKS 
*/


#endif// LINEAR_H
