/*!
 * @file linear.h
 * @copyright GNU General Public Licence 3 or Later (GPLv3).
 * @author Paulo Arruda
 * @brief Header file containing APIs for linear type data structures.
 * @defgroup linear
 * @{
*/
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "common.h"

#ifndef LINEAR_H
#define LINEAR_H

#include "hash.h"

#ifndef LINEAR_DEFAULT_STATUS_ACTION_ALLOCATION_FAILURE
#define LINEAR_DEFAULT_STATUS_ACTION_ALLOCATION_FAILURE ACTION_WARN
#endif //DEFAULT_STATUS_ACTION_ALLOCATION_FAILURE

#ifndef LINEAR_DEFAULT_STATUS_ACTION_INVALID_SIZE
#define LINEAR_DEFAULT_STATUS_ACTION_INVALID_SIZE ACTION_WARN
#endif //LINEAR_DEFAULT_STATUS_ACTION_INVALID_SIZE

#ifndef LINEAR_DEFAULT_STATUS_ACTION_INVALID_RANGE
#define LINEAR_DEFAULT_STATUS_ACTION_INVALID_RANGE ACTION_WARN
#endif //LINEAR_DEFAULT_STATUS_ACTION_INVALID_RANGE

#ifndef LINEAR_DEFAULT_STATUS_ACTION_NULL_DATA
#define LINEAR_DEFAULT_STATUS_ACTION_NULL_DATA ACTION_WARN
#endif // LINEAR_DEFAULT_STATUS_ACTION_NULL_DATA

#ifndef LINEAR_DEFAULT_STATUS_ACTION_TYPE_OVERFLOW
#define LINEAR_DEFAULT_STATUS_ACTION_TYPE_OVERFLOW ACTION_WARN
#endif // LINEAR_DEFAULT_STATUS_ACTION_TYPE_OVERFLOW

/*!
 * @brief Opaque data type definition for the Vector structure.
 * @note The vector structure is a pure C implementation for the C++
 * `std::vector` class. It offers the following methods:
 * - `vectorCreate`;
 * - `vectorDelete`;
 * - `vectorPush`;
 * - `vectorGetAt`;
 * - `vectorPopAt`;
 * - `vectorLength`;
 * - `vectorUpdateAte`; and
 * - `vectorCapacity;`
*/
typedef struct Vector Vector;

/*!
 * @brief Constructor function for the vector structure.
 * @param min_capacity The minimal capacity of the vector.
 * @param data_size The size of the data to be stored.
 * @return A pointer to a new empty vector if all memory allocations were 
 * sucesseful, or a `NULL` pointer otherwise.
*/ 
Vector* vectorCreate(const cds_size min_capacity, const cds_size data_size);

/*! 
* @brief Destructor function for the Vector data type.
 * @param vec A pointer to the vector..
*/ 
void vectorDelete(Vector* vec);

/*!
 * @brief Retrieve data from avector at specific a index.
 * @param vec A pointer to the vector.
 * @param index The index where the data is stored.
 * @return A void pointer to the data, if any, or a `NULL` pointer if either
 * the vector is empty or the index is out of range.
*/
void* vectorGetAt(const Vector* vec, const size_t index);

/*!
 * @brief Push a new data into the vector.
 * @note In the case that the length of the vector reaches 85% of the 
 * current capacity, this function will also expand the vector to
 * twice its current capacity.
 * @param vec A pointer to the vector.
 * @param data A pointer to the data to be stored.
 * @return True if the insertion was sucesseful, or false otherwise.
 * @note The insersion is considered unsucesseful if the pointer to the vector
 * or to the data passed is `NULL` or the vector cannot expand itself further
 * and there is no space to store the new data.
*/
cds_bool vectorPush(Vector* const vec, void* data);

/*!
 * @brief Returns the data stored at the the specified index and translate
 * the rest of the vector to occupy the voided space.
 * @note If desired, this function will shrink the vector to half of its
 * current capacity whenever the length is less than or equalt to 20% of
 * its current capacity.
 * @param vec A pointer to the vector.
 * @param index The index where the data should be retrieved and deleted.
 * @param shrink Determines whether the vector should shrink.
 * @return The data previously stored at the specified index, or a `NULL` 
 * pointer if either the vector is empty, the pointer to the vector or to the 
 * data is `NULL`, or the index passed is out of range.
*/
void* vectorPopAt(Vector* vec, cds_size index, cds_bool shrink);

/*!
 * @brief Updates value in the vector at a given index.
 * @param vec A pointer to the vector.
 * @param data A pointer to the new data.
 * @param index The index where the update should occur.
 * @return The old value stored in the vector at the specified index.
*/
void* vectorUpdateAt(Vector* const vec, void* data, const cds_size index);

/*!
 * @brief Retrieve the current length of a vector.
 * @param vec A pointer to the vector whose length will be retrieved.
 * @return The current length of the vector (assuming the length of a 
 * null pointer to be 0).
*/
cds_size vectorLength(const Vector* const vec);

/*!
 * @brief Retrieves the current capacity of the vector.
 * @param vec A pointer to the vector.
 * @return The current capacity of the vector (assuming that the capacity 
 * of a null vector is 0).
*/
cds_size vectorCapacity(const Vector* const vec);

/****************** Tuples *******************/

/*!
 * @brief Opaque definition for the tuple structure.
 * @note The tuple API implements an immutable array-like structure inspired
 * by Pythons's `tuple` class.
*/

typedef struct Tuple Tuple;

/*!
 * @bried Create tuple from values
 * @param data_size The size of the data to be added.
 * @return A pointer to a new tuple create from the values, if all memory allocations
 * were successeful, or a `NULL` pointer otherwise.
*/
Tuple* tupleCreate(const cds_size length, const cds_size data_size, ...);

/**
 * @brief Cronstructor function for the tuple structure from arrays.
 * @param min_capacity
 * @param data_size
 * @return A pointer to a new empty tuple if all memory allocations were
 * successeful, otherwise return a null pointer.
 * @raise
*/
Tuple* tupleFromArray(const void* const arr, const cds_size length, const cds_size data_size);

/**
 * @brief Destructor function for the tuple structure.
 * @param tuple A pointer to the tuple to be destroyed.
*/
void tupleDelete(Tuple* tuple);
/**
 * @brief Retrieve data stored in the tuple at the specified index
 * @param tuple
 * @param index
 * @return A void pointer to data stored in the tuple at the specified index,
 * if any, or `NULL` if the index is out of range.
*/
const void* tupleGetAt(const Tuple* const tuple, const cds_size index);

cds_size tupleLength(const Tuple* const tuple);
/**
 * SINGLY LINKED LIST
 * ------------------
 * @brief API for the singly linked list structure.
*/
typedef struct SLList SLList;

/**
 * @brief Creator function for the singly linked list structure.
 * @param data_size The size of the data to be stored.
 * @return A new empty linked list if all allocations of
 * memory were successeful, or NULL otherwise.
*/
SLList* sllistCreate(cds_size data_size);
/**
 * DOUBLE LINKED LIST
 * ------------------
 * @brief API for the double linked list structure.
*/
typedef struct DLList DLList;

/*******************************
 * ITERATORS
 * ---------
********************************/

/*!
 * @brief
*/
enum IterableType{
    VECTOR,
    TUPLE,
    SLLIST,
    HASH_TABLE,
    SET,
};

/*!
 * @brief opaque definition of the linear iterator structure.
 * @note The linear iterator structure iterates over the CDS container types. The
 * current supported containers are:
 * - Vectors;
 * - Tuples;
 * - Singly Linked Lists;
 * - Hash Tables (iteration occours over the keys);
 * - Sets;
*/
typedef struct Iter Iter;

/*!
 * @brief constructor function for the iterator structure.
 * @param container A pointer to the container.
 * @param type The type of the container.
 * @return A pointer to a new iterator referencing the begining of the 
 * container if all allocations were sucesseful and the container is not empty
 * or its pointer `NULL`, otherwise it returns a `NULL` pointer.
*/
Iter* iterCreate(const void* const container, const enum IterableType type);

/**
 * @brief Destructor function for the iterator structure.
 * @note This function should be executed when the iteration is stoped before
 * reaching the end of the container, since the `iterNext` deletes the iterator
 * when it reaches the end of the container.
*/
void iterDelete(Iter* iter);
/*!
 * @brief Next operator for the iterator structure.
 * @param iter A pointer to the iterator.
 * @return A pointer to the iterator referencing the next not empty position in
 * the container, if any, or a `NULL` if either the iterator is `NULL` or there
 * are no non-empty positions left in the container.
 * @note In an `for` or in a `while` loop, the stop condition is to be set
 * whenever this function returns a `NULL` pointer. When the end of the container
 * is reached, this function will delete the iterator.
*/
Iter* iterNext(Iter* iter);

/*!
 * @brief Retrieves the data referenced by the iterator.
 * @param iter A pointer to the iterator.
 * @param[out] pdata_size A pointer to the size of the data to be retrieved.
 * @return A void pointer to the data if any, or a `NULL` pointer if the iterator
 * is empty.
*/
const void* iterGetData(const Iter* const iter);

/*****************************************
 * STACK
 * -----
 ****************************************/
/**
 * @brief Opaque definition of the stack structure.
*/
typedef struct Stack Stack;

/**
 * @brief Constructor function for the stack structure.
 * @param data_size The size of the data to be stored.
 * @return A pointer to a new empty stack if all memory allocations
 * were sucesseful, or `NULL` pointer otherwise.
*/
Stack* stackCreate(cds_size data_size);

/**
 * @brief Destructor function for the stack structure.
 * @param stack A pointer to the stack to be deleted.
*/
void stackDelete(Stack* stack);

/**
* @brief Adds new data to the specified stack.
* @param stack A pointer to the stack.
* @param data A pointer to the data.
* @return `true` if the insertion was sucesseful, or `false` otherwise.
*/
cds_bool stackPush(Stack* stack, void* data);

/**
 * @brief Retrieve and delete the position of the last element added to the 
 * stack.
 * @param stack A pointer to the stack.
 * @return A void pointer to the last data added, if any, or a `NULL` pointer
 * otherwise.
*/
void* stackPop(Stack* stack);

/**
 * QUEUES
 * ------
*/

typedef struct Queue Queue;

Queue* queueCreate(cds_size data_size);

void queueDelete(Queue* queue);

cds_bool queuePush(Queue* queue);

void* queuePop(Queue* queue);

#endif// LINEAR_H

/**
 * @} // end of the linear doxygen group.
*/

#ifdef __cplusplus
};
#endif // __cplusplus

