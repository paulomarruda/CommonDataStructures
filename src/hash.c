#include <limits.h>
#include "../include/hash.h"
/**
 * HASH FUNCTIONS
 * --------------
 * Implementation of the hash functions we use.
*/

/**
 * 64BITs HASHING
*/

#define _FNV_OFFSET 14695981039346656037UL
#define _FNV_PRIME 1099511628211UL

cds_uint64 FNV1aHash(const void* key, const KeyType key_type){
    cds_uint64 hash = _FNV_OFFSET;
    const cds_char* str_key = (void*) NULL;
    cds_uint64 int_key = 0;
    switch (key_type) {
        case STR_KEY:
            str_key = (const cds_char*) key;
            for (const cds_char* p=str_key; *p!='\0'; p++){
                hash ^= (cds_uint64) (cds_uchar) *p;
                hash *= _FNV_PRIME;
            }
            break;
        case INT_KEY:
            int_key = *(cds_uint64*) key;
            hash ^= int_key;
            hash *= _FNV_PRIME;
            break;
    }
    return hash;
}
/**
 * AUXILIARY FUNCTIONS
 * -------------------
 * In this section we define static auxiliary functions that
 * will be used by the data structures implemented here. 
*/

#define LENGTH(hash) hash? hash->length: 0
#define CAPACITY(hash) hash? hash->capacity: 0
#define GET_EXANSION_RATE(capacity) (((double) capacity) * _EXPANSION_RATE_CHECK)
#define INVALID_SIZE(size) ((0 == size)? true: false)
/**
 * Returns the index based on the key.
*/
static cds_size _hashGetIndexFromKey(const HashFunction hash_fun, KeyType key_type, 
                                     const void* key, const cds_size capacity){
    cds_uint64 hash = hash_fun(key, key_type);
    // since the capacity is always a power of two
    // the reminder can be done easily by the following bitwise op
    cds_size index = (cds_size) (hash & ( (cds_uint64)(capacity - 1) ));
    return index;
}

/**
 * Returns whether the keys are equal.
*/
static cds_bool _hashKeyComp(const void* key1, const void* key2, const KeyType key_type){
    cds_bool result = false;
    switch (key_type) {
        case INT_KEY:
            result = (*(cds_int*) key1) == (*(cds_int*) key2);
            break;
        case STR_KEY:
            result =  0 == strcmp((cds_char*) key1, (char*) key2);
            break;
    }
    return result;
}

/*
 * HASH TABLES
 * -----------
 *  Here follows the implementation of a dynamic hash table.
*/

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

/**
 * Constructor function for hash table.
*/
HashTable* htCreate(const HashFunction hash_fun, const cds_size min_capacity, 
                    const KeyType key_type){
    if (min_capacity <= 0){
        // handle error
        return (HashTable*) NULL;
    }
    cds_size pow = _log2(min_capacity) + 1;
    if (pow >= _MAX_POW2_){
        //handle size_t overflow error 
        return (HashTable*) NULL;
    }
    //make the capacity the next power of 2 
    //of the min_capacity
    cds_size capacity = 1 << pow;
    HashTable* new_table = (HashTable*) malloc(sizeof(HashTable));
    if (!new_table){
        return (HashTable*) NULL;
    }
    HTEntry* entries = (HTEntry*) calloc(capacity, sizeof(HTEntry));
    if (!entries){
        //handle errors
        free(new_table);
        return (HashTable*) NULL;
    }
    new_table->capacity = capacity;
    new_table->length = 0;
    new_table->key_type = key_type;
    new_table->hash_fun = hash_fun;
    new_table->entries = entries;
    return new_table;
}

void htDelete(HashTable* table){
    if (!table){ 
        return;
    }
    free(table->entries);
    free(table);
}


cds_bool htSearch(const HashTable* const ht, const void* key){
    if (!key || !ht){
        return false;
    }
    cds_size index = _hashGetIndexFromKey(ht->hash_fun, ht->key_type, key, ht->capacity);
    while (ht->entries[index].key || !ht->entries[index].is_tombstone){
        if (_hashKeyComp(ht->entries[index].key, key, ht->key_type)){
            return true;
        }
        index++;
        if (ht->capacity == index){
            index = 0;
        }
    }
    return false;
}

static void _htUpdateEntry(HTEntry* entry, const void* data, const cds_size data_size){
    entry->data = data;
    entry->data_size = data_size;
}

static bool _htExpand(HashTable* ht){
    const cds_size new_capacity = ht->capacity << 1;
    if (new_capacity <= ht->capacity){
        return false;
    }
    HTEntry* new_entries = (HTEntry*) calloc(new_capacity, sizeof(HTEntry));
    if (!new_entries){
        return false;
    }
    cds_size new_index;
    for (cds_size index=0; index<ht->capacity; index++){
        if (ht->entries[index].key){
            new_index = _hashGetIndexFromKey(ht->hash_fun, ht->key_type, 
                                             ht->entries[index].key, new_capacity);
            while (new_entries[new_index].key){
                new_index++;
                if (new_capacity == new_index){
                    new_index = 0;
                }
            }
            new_entries[new_index].key = ht->entries[index].key;
            new_entries[new_index].key_size = ht->entries[index].key_size;
            new_entries[new_index].data = ht->entries[index].data;
            new_entries[new_index].data_size = ht->entries[index].data_size;
        }
    }
    free(ht->entries);
    ht->entries = new_entries;
    ht->capacity = new_capacity;
    return true;
}

static cds_bool _htSetData(HashTable* ht, const void* key, const cds_size key_size,
                           const void* data, const cds_size data_size){
    cds_size index = _hashGetIndexFromKey(ht->hash_fun, ht->key_type, key, ht->capacity);
    while (ht->entries[index].key){
        if (_hashKeyComp(ht->entries[index].key, key, ht->key_type)){
            _htUpdateEntry(&ht->entries[index], data, data_size);
            return true;
        }
        index++;
        if (ht->capacity == index){
            index = 0;
        }
    }
    // calloc already initialized `is_tombstone` as 0!
    ht->entries[index].key = key;
    ht->entries[index].data = data;
    ht->entries[index].data_size = data_size;
    ht->entries[index].key_size = key_size;
    ht->length++;
    return true;
}
// TO-DO update the values should occur befor the expansion!!
cds_bool htSet(HashTable *ht, const void *key, const cds_size key_size, 
               const void *data, const cds_size data_size){
    if (!ht || !key || !data || INVALID_SIZE(key_size) || INVALID_SIZE(data_size)){
        return false;
    }
    if (GET_EXANSION_RATE(ht->capacity) <= (double) ht->length+1){
        (void) _htExpand(ht);
    }
    if (ht->length + 1 >= ht->capacity){
        return false;
    }
    return _htSetData(ht, key, key_size, data, data_size);
}

cds_size htLength(const HashTable* const ht){
    return LENGTH(ht);
}
cds_size htCapacity(const HashTable* const ht){
    return CAPACITY(ht);
}

const void* htGet(const HashTable* ht, const void* key, cds_size* pdata_size){
    cds_size index = _hashGetIndexFromKey(ht->hash_fun, ht->key_type, key, ht->capacity);
    while (ht->entries[index].key || !ht->entries[index].is_tombstone){
        if (_hashKeyComp(ht->entries[index].key, key, ht->key_type)){
            if (pdata_size){
                *pdata_size = ht->entries[index].data_size;
            }
            return ht->entries[index].data;
        }
        index++;
        if (ht->capacity == index){
            index = 0;
        }
    }
    return NULL;
}

const void* htPop(HashTable* ht, const void* key){ 
    cds_size index = _hashGetIndexFromKey(ht->hash_fun, ht->key_type, key, ht->capacity);
    while (ht->entries[index].key || !ht->entries[index].is_tombstone){
        if (_hashKeyComp(ht->entries[index].key, key, ht->key_type)){
            ht->entries[index].is_tombstone = true;
            ht->entries[index].key = NULL;
            ht->entries[index].key_size = 0;
            const void* data = ht->entries[index].data;
            ht->entries[index].data = NULL;
            ht->entries[index].data_size = 0;
            ht->length--;
            return data;
        }
        index++;
        if (ht->capacity == index){
            index = 0;
        }
    }
    return NULL;
}

/**
 * SETS
 * ----
 *  Implementation of the set structure
*/

typedef struct {
    cds_bool is_tombstone;
    size_t key_size;
    const void* key;
}SetEntry;

struct Set{
    KeyType key_type;
    size_t capacity;
    size_t length;
    HashFunction hash_fun;
    SetEntry* entries;
};

Set* setCreate(const size_t min_capacity, const HashFunction hash_fun, 
               const KeyType key_type){
    Set* new_set = (Set*) malloc(sizeof(Set));
    if (!new_set){
        return (Set*) NULL;
    }
    size_t pow = _log2(min_capacity) + 1;
    if (pow >= _MAX_POW2_){
        //handle overflow error
        free(new_set);
        return (Set*) NULL;
    }
    size_t capacity = 1 << pow;
    SetEntry* entries = (SetEntry*) calloc(capacity, sizeof(SetEntry*));
    if (!entries){
        free(new_set);
        return (Set*) NULL;
    }
    new_set->key_type = key_type;
    new_set->capacity = capacity;
    new_set->length = 0;
    new_set->hash_fun = hash_fun;
    new_set->entries = entries;
    return new_set;
}

