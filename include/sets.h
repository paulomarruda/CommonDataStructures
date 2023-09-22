#ifndef SETS_H
#define SETS_H
#include <stdbool.h> 
#include "hash_fun.h" 
/**
 * @brief Opaque type for set structure
*/
typedef struct Set Set;

/**
 * @brief Create a new set.
 * @param hash_function
 *  A `HashFunction` type hash function.
 * @param min_capacity
 *  the minimal capacity that the set should have. 
 * @param key_type
 *  A the type of the key.
 * @param as_copy
 *  Determines wether the set should copy the stored key.
 * @return 
 *  A new set with the specified paraameters if 
 * the allocation of memory was succeseful, a `NULL` pointer
 * otherwise.
*/
Set* setCreate(const HashFunction hash_fun, const size_t min_capacity, const KeyType key_type, 
               const bool as_copy);

/** 
 * @brief Frees all the allocated memory for the given set.
 * @param set 
 *  A set to be deleted.
*/
void setDelete(Set* set);

/**
 * @brief Looks for a key in the set.
 * @param set
 *  The set in which the search will be performed.
 * @param key
 *  The key to be searched.
 * @return 
 *  `true` if the key was found in the set, or `false` otherwise.
*/

/**
 * @brief Constant to determine if the set should expand.
 */
#define EXPAND_RATE_CHECK 0.75

bool setContains(const Set* set, const void* key);

/**
 * @brief Insert a new key in the given set. This function will
 * expand the set to twice its capacity whenever the length of the 
 * set exceeds its capacity multiplied by `EXPAND_RATE_CHECK` constant. 
 * Both the set and the key are assumed to be not `NULL`.
 * @param set
 *  A not `NULL` set.
 * @param key
 *  A not `NULL` key.
 * @param key_size
 *  The size of the key.
 * @return
 *  `true` if the insersion was successeful, or false otherwise. 
*/
bool setInsert(Set* set, const void* key, const size_t key_size);

/**
 * @brief Determines the length of the set.
 * @param set
 *  A not `NULL` set.
 *  @return 
 *   The current length of the set.
*/
size_t setLenght(const Set* set);
/**
 * @brief Determines the capacity of the set.
 * @param set
 *  A not `NULL` set.
 * @return 
 *  The current capacity of the set.
*/
size_t setCapacity(const Set* set);

#endif // SETS_H
