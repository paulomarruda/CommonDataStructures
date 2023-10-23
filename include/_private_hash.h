/*!
 * @file _private_hash.h
 * @copyright GNU General Public Licence 3 or Later (GPLv3).
 * @author Paulo Arruda
 * @brief Explicit declaration of the structures used by the hash API.
*/


#include "hash.h"

#ifndef _PRIVITE_HASH_H
#define _PRIVITE_HASH_H

/**
 * Definition of the entries of the hash table structure.
*/
typedef struct HTEntry{
    cds_bool is_tombstone;
    cds_size key_size;
    cds_size data_size;
    const void* data;
    const void* key;
}HTEntry;

/**
 * Definition of the hash table structure.
*/
struct HashTable{
    cds_size capacity;
    cds_size length;
    KeyType key_type;
    HashFunction hash_fun;
    HTEntry* entries;
};
/* Definition of the set entries **/
typedef struct {
    cds_bool is_tombstone;
    size_t key_size;
    const void* key;
}SetEntry;

/* Definition of the set structure **/
struct Set{
    KeyType key_type;
    size_t capacity;
    size_t length;
    HashFunction hash_fun;
    SetEntry* entries;
};

#endif // _PRIVITE_HASH_H
