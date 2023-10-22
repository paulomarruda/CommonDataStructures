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
#include "linear.h"
/*
 * Setting the default action for status as to warn.
 * This can be changed by defining the preprossessor
 * macro `DEFAULT_STATUS_ACTION` as `ACTION_EXIT` before
 * including this header.
*/
#ifndef HASH_DEFAULT_STATUS_ACTION
#define HASH_DEFAULT_STATUS_ACTION ACTION_WARN
#endif //HASH_DEFAULT_STATUS_ACTION

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
Set* setCreate(const cds_size min_capacity, const HashFunction hash_fun, const KeyType key_type);

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

/*!
 * @brief Opaque definition of the hash table structure.
 * @note The API does not hold ownership over the data or the keys stored. As such,
 * any memory allocations to create the pair key and data will not be freed by
 * the table's destructor function.
*/
typedef struct HashTable HashTable;

/**
 *
 * @brief Constructor function for the hash table structure.
 * @note This function will return a memory allocated pointer to a new and empty
 * hash table. This pointer should later be deleted by the destructor function for the
 * hash table structure. Currently, the API supports integers type key (`INT_KEY`)
 * and string (null terminated `char` pointers, `STR_KEY`) as keys. The actual capacity
 * stored at the new table is the least power of 2 larger than the `min_capacity`.
 * @param hashFun A function pointer to the hashing function.
 * @param min_capacity The minimum capacity that the hash table should have.
 * @param key_type Indicates the type of the key.
 * @returns A pointer for a new hash table if all memory allocations were successeful,
 * or a `NULL` pointer if either the memory allocations fail or overflow of `size_t`
 * occours.
 * @see `htstrDelete`.
 */ 
HashTable* htCreate(const HashFunction hash_fun, const cds_size min_capacity, 
                    const KeyType key_type);

/*!
 * @brief Destructor function for the hash table structure.
 * @param ht A pointer to the hash table.
 * @note This function does not free any memory allocation that occoured for the
 * creation of the keys and the data stored; as such, they should be freed properly
 * to avoid memory leaks.
 */ 
void htDelete(HashTable* ht);

/**
 * @brief Searching function for the hash table structure.
 * @param ht A pointer to the hash table.
 * @param key A pointer to the key.
 * @return True if the key is present in the table, or false otherwise.
*/
bool htSearch(const HashTable* const ht, const void* key);

/*!
 * @brief Inserting/updating function for the hash table structure.
 * @param ht A pointer to the hash table.
 * @param key A pointer to the key.
 * @param data_size The data to be added/updated.
 * @return `true` if the insertion was successeful, or false the table cannot expand
 * further and there are no space left for the insersion.
 * @note This function will expand the hash table whenever the length of the table + 1
 * reaches at least 85% of its total capacity. The new capacity will be twice as the
 * old capacity.
*/
bool htSet(HashTable* ht, const void* key, const cds_size key_size, const void* data, 
           const cds_size data_size);
/*! 
 * @brief This function searches and retrieve data from the hash table.
 * @param ht A pointer to the table.
 * @param key A pointer to the key.
 * @param[out] pdata_size A pointer to the data size of the data to be retrieved.
 * @returns A void pointer to the data stored at the key, if any, or a `NULL` pointer
 * otherwise.
 */
const void* htGet(const HashTable* ht, const void* key, cds_size* pdata_size);

/*!
 * @brief Get funtion for the length of the table.
 * @param ht A pointer to the hash table.
 * @return The current length of the table.
*/
size_t htLength(const HashTable* ht);
/*!
 * @brief Get function for the capacity of the table.
 * @param ht A pointer to the table.
 * @return The current capacity of the table.
*/
size_t htCapacity(const HashTable* ht);

#endif // HASH_TABLE_H
