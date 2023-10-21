/*
 *  HASH API 
 *  @brief The design has been based on Bob Nystrom’s Crafting 
 *  Interpreters book's chapter on hash hts and a post by 
 *  Ben Hoyt named How to Implement a Hash ht (in C). I modified the API
 *  to attend some safety concerns and add some modularity 
 *
*/ 
#ifndef HASH_H
#define HASH_H
#include "common.h"

/*
 * Setting the default action for status as to warn.
 * This can be changed by defining the preprossessor
 * macro `DEFAULT_STATUS_ACTION` as `ACTION_EXIT` before
 * including this header.
*/
#ifndef DEFAULT_STATUS_ACTION
#define DEFAULT_STATUS_ACTION ACTION_WARN
#endif //DEFAULT_STATUS_ACTION

/**
 * HASH FUNCTIONS
 * @brief API for hash functions and key types.
*/

/**
 * @brief The types of key supported so far.
*/
typedef enum KeyType{
    STR_KEY,
    INT_KEY,
    SIZE_KEY,
}KeyType;

/**
 * @brief Type definition for hash function pointers.
*/
typedef cds_uint64 (*HashFunction)(const void *key, const KeyType type);

/**
 * @brief FNV1a hash function.
 *
 * A non-cryptographic hash function.
 *
* @param key The key to be hashed
* @param key_type The type of the key
* @return the hash
*/
cds_uint64 FNV1aHash(const void* key, const KeyType key_type);

/*
 * SETS
 * @brief API for the set structure.
*/

/**
 * @brief Opaque definition of the set structure.
*/
typedef struct Set Set;

/**
 * @brief Creator function for the set structure.
 *
 * This function will allocate memory for the new set. This memory
 * has to be later freed by the appropriate deleting function
 * (`setDelete`). The size of the allocation, i.e. the capacity
 * of the set, is rougthly 2 times of the min capacity passed, namely
 * the least power of two greater than the minimum capacity passed.
 *
 * @param min_capacity
 * @param as_copy
 * @param hash_fun
 * @param key_type
 * @return A pointer to new empty set, if memory allocations were successeful, or 
 * a NULL pointer otherwise.
 * @raises
 * `ALLOCATION_ERROR`
 * `INVALID_CAPACITY`
 * @see `setDelete`, `ALLOCATION_ERROR`, `INVALID_CAPACITY`
*/
Set* setCreate(const cds_size min_capacity, const cds_bool as_copy, 
               const HashFunction hash_fun, const KeyType key_type);

/**
 * @brief deleting function for the set structure.
 *
 * @param set A pointer to the set to be deleted.
*/
void setDelete(Set* set);

/**
 * @brief Inserts a new key into the set.
 *
 * @param set
 * @param key
 * @param key_size
 * @return True if the insertion was successeful, or false otherwise; if the key 
 * is already in the set, this function also return true.
*/
bool setInsert(Set* set, const void* key, const cds_size key_size);
/**
 * @brief searching function for the set structure.
 *
 * @param set
 * @param key
 * @return key
 * @raise
*/
bool setSearch(const Set* const set, const void* const key);
/** 
 * HASH TABLE
 * @brief API for the hash table structure.
*/

/**
 * @brief Opaque definition of the hash table type.
*/
typedef struct HashTable HashTable;

/**
 *
 * @brief Creating function for the hash table structure.
 *
 * The function will allocate memory
 * for the new ht and thus should be freed at the end of its used by
 * the function `htstrDelete`.
 *
 * @param hashFun
 * The hashing function to be used by the ht
 * @param min_capacity
 * The minimum capacity that the hash table should have.
 * @param copy_data 
 * Indicates wether the ht should copy data when insertion function
 * is invoked. If true, the ht will allocate memory and copy for both the
 * string and the data. This allocated memory is freed automatically when
 * the hash table is destroyed.
 * @param key_type
 * Indicates the type of the key.
 *
 * @returns
 * A pointer for the new hash table.
 *
 * @see htstrDelete
 *
 * @raises
 * ALLOCATION_ERROR
 * If `malloc` fails to allocate memory for the table.
 * INVALID_CAPACITY
 * If non-positive `min_capacity` was passed.
 * OVERFLOW_ERROR
 * If overflow ocurs in `size_t` when determine the
 * capacity of the table.
 *
 * @constrains 
 * `capacity` should not be zero and should fit in the `size_t` 
 * type, i.e. should be less than or equal to SIZE_MAX.
 */ 
HashTable* htCreate(const HashFunction hash_fun, const cds_size min_capacity, 
                   const cds_bool copy_data, const KeyType key_type);

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
 * @brief Searching function for the hash table structure.
 *
 * @param ht A pointer to the hash table.
 * @param key The key to be searched.
 * @return True if the key is present in the table, or false otherwise.
*/
bool htSearch(const HashTable* const ht, const void* key);

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
bool htSet(HashTable* ht, const void* key, const cds_size key_size, const void* data, 
           const cds_size data_size);
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
const void* htGet(const HashTable* ht, void* key, const cds_bool get_copy);

size_t htLength(const HashTable* ht);

size_t htCapacity(const HashTable* ht);

typedef struct HTIter HTIter; 

HTIter* htiterCreate(const HashTable* ht);

HTIter* htiterNext(HTIter* iter);

void* htGetKeyFromIter(const HTIter* const iter);

void* htGetDataFromIter(const HTIter* const iter);

#endif // HASH_TABLE_H
