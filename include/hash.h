/*
 *      HASH ht API 
 *  @brief The design has been based on Bob Nystrom’s Crafting 
 *  Interpreters book's chapter on hash hts and a post by 
 *  Ben Hoyt named How to Implement a Hash ht (in C). I modified the API
 *  to attend some safety concerns and add some modularity 
 *
*/ 
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stdbool.h>
#include "hash_fun.h" 
/*
 * SETS
*/

typedef struct Set Set;



/* HASH ht WITH STRING KEYS */


typedef struct HashTable HashTable;

/**
 * Creator function for the HashTableStr 
 *
 * @brief Creates a new hash ht whose keys are strings. The function
 * will allocate memory for the new ht and thus should be freed at the 
 * end of its used by the function `htstrDelete`.
 *
 * @param hashFun 
 * The hasing function to be used by the ht 
 * @param copy_data 
 * Indicates wether the ht should copy data when insertion function 
 * is invoked. If true, the ht will allocate memory and copy for both the 
 * string and the data.
 *
 * @return 
 * A new (HashTablestr ht) if all memory allocation for its component was successeful, or 
 * a (HashTableStr*) NULL ht.
 *
 * @see htstrInsert, htstrDelete
 *
 * @modify
 * This function modifies the global variable ALLOCATION_ERROR indicating 
 * precisely what went wrong.
 *
 * @constrains 
 * `capacity` should not be zero and should fit in the `size_t` 
 * type, i.e. should be less than or equal to SIZE_MAX.
 */ 
HashTable* htCreate(const HashFunction hash_fun, const size_t min_capacity, 
                   const bool copy_data, const KeyType key_type);

/**
 * Deleting function for the HashTableStr 
 *
 * @brief Free the allocated memory for the ht and its components.
 *
 * @param ht
 * The ht to be deleted.
 *
 */ 
void htDelete(HashTable* ht);

/**
 * Inserting/updating function for the HashTableStr. 
 *
 * @brief The inserting function for the HashTableStr will insert or update
 * data in the ht. If the ht was created as a copy ht,
 * this function will allocate memory for data inserted, otherwise it will 
 * only reference the position of the pair (key, data) in the ht. If there 
 * were any problems with the insertion, the global variables ALLOCATION_ERROR 
 * and INSERTION_ERROR. If the ht is at 70% of its total capacity, this function
 * will also expand the ht twice the current capacity.
 *
 * @param ht 
 * The ht where the insertion will be performed. 
 * @param key
 * A nul terminated `char` pointer (string).
 * @param data_size 
 * The data to be added.
 * @return 
 * true if the insertion was successeful, or false otherwise.
 * @modify 
 * The ht in use, ALLOCATION_ERROR or INSERTION_ERROR. 
*/
bool htSet(HashTable* ht, void* key, void* data, size_t data_size);
/**
 * Get function for the HashTableStr 
 * @brief This function searches and retrieve data from the hash ht.
 * It will return a void pointer to the data if the key passed was 
 * found in the ht, or a void NULL pointer otherise. The user can 
 * choose if the function returns a reference to the data in the ht 
 * or a copy of the data (that will be dynamically allocated). 
 * @param ht 
 * @param key 
 * @param get_copy 
 * @returns 
 * @modifies
 */
void* htGet(const HashTable* ht, void* key, const bool get_copy);



size_t htLength(const HashTable* ht);

size_t htCapacity(const HashTable* ht);

typedef struct HashTableIter HashTableIter; 

HashTableIter* htiterCreate(const HashTable* ht);

HashTableIter* htiterNext(HashTableIter* iter);

void* htGetKeyFromIter(const HashTableIter* const iter);

void* htGetDataFromIter(const HashTableIter* const iter);
#endif // HASH_TABLE_H 
