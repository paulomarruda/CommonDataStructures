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
#define GET_EXANSION_RATE(capacity) ((double) capacity) * _EXPANSION_RATE_CHECK

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
        case SIZE_KEY:
            result = (*(cds_size*)key1 == *(cds_size*) key2);
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
    while (ht->entries[index].key){
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
    HTEntry* new_entries = (HTEntry*) realloc(ht->entries, new_capacity * sizeof(HTEntry));
    if (!new_entries){
        return false;
    }
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
    ht->entries[index].key = key;
    ht->entries[index].data = data;
    ht->entries[index].data_size = data_size;
    ht->entries[index].key_size = key_size;
    ht->length++;
    return true;
}

cds_bool htSet(HashTable *ht, const void *key, const cds_size key_size, 
               const void *data, const cds_size data_size){
    if (GET_EXANSION_RATE(ht->capacity) <= (double) ht->length){
        (void) _htExpand(ht);
    }
    if (ht->length + 1 >= ht->capacity || !ht || !key || !data){
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

const void* htGet(const HashTable* ht, const void* key){
    cds_size index = _hashGetIndexFromKey(ht->hash_fun, ht->key_type, key, ht->capacity);
    while (ht->entries[index].key){
        if (_hashKeyComp(ht->entries[index].key, key, ht->key_type)){
            return ht->entries[index].data;
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
    const void* key;
    size_t key_size;
}SetEntry;

struct Set{
    HashFunction hash_fun;
    bool as_copy;
    size_t capacity;
    size_t length;
    KeyType key_type;
    SetEntry** entries;
};

Set* setCreate(const size_t min_capacity, const bool as_copy, 
               const HashFunction hash_fun, const KeyType key_type){
    Set* new_set = (Set*) malloc(sizeof(Set));
    if (!new_set){
        return (Set*) NULL;
    }
    size_t pow = _log2(min_capacity) + 1;
    if (pow >= _MAX_POW2_){
        //handle overflow error
        return (Set*) NULL;
    }
    size_t capacity = 1 << pow;
    SetEntry** entries = (SetEntry**) malloc(capacity * sizeof(SetEntry*));
    if (!entries){
        free(new_set);
        return (Set*) NULL;
    }
    new_set->as_copy = as_copy;
    new_set->capacity = capacity;
    new_set->length = 0;
    new_set->hash_fun = hash_fun;
    new_set->entries = entries;
    new_set->key_type = key_type;
    return new_set;
}

static SetEntry* _setCreateEntry(const void* key, const size_t key_size){
    SetEntry* new_entry = (SetEntry*) malloc(sizeof(SetEntry));
    if (!new_entry){
        return (SetEntry*) NULL;
    }
    new_entry->key = key;
    new_entry->key_size = key_size;
    return new_entry;
}

static void _setDestroyEntry(SetEntry* entry, const bool is_copy){
    if (!entry){
        return;
    }
    if (is_copy){
        free((void*) entry->key);
    }
    free(entry);
}

void setDelete(Set* set){
    if (!set){
        return;
    }
    for (size_t i=0; i<set->capacity; i++){
        _setDestroyEntry(set->entries[i], set->as_copy);
    }
    free(set->entries);
    free(set);
}
static size_t _setGetIndexFromEntry(const SetEntry* entry, const HashFunction hash_func,
                                    const KeyType key_type, const size_t capacity){
    uint64_t hash = hash_func(entry->key, key_type);
    size_t index = (size_t) (hash & ((uint64_t) capacity - 1));
    return index;
}

static bool _setExpand(Set* const set, const size_t new_capacity){
    SetEntry** new_entries = (SetEntry**) malloc(new_capacity * sizeof(SetEntry*));
    if (!new_entries){
        //handle errors
        return false;
    }
    size_t index;
    for (size_t i=0; i<set->capacity; i++){
        index = _setGetIndexFromEntry(set->entries[i], set->hash_fun, set->key_type,
                                      new_capacity);
        while (new_entries[index]){
            index++;
            if (index == new_capacity){
                index = 0;
            }
        }
        new_entries[index] = set->entries[i];
    }
    return true;
}

bool setInsert(Set* set, const void* key, const size_t key_size){
    bool has_inserted = false;
    if (!key){
        //decide if it's gonna be an error!
        return false;
    }
    bool has_expanded = false;
    if ( (double) set->length >= (((double) set->capacity) * _EXPANSION_RATE_CHECK) ){
        size_t new_capacity = set->capacity * 2;
        if (new_capacity <= set->capacity){
            //overflow of size_t 
        }
        // deal properly 
        has_expanded = _setExpand(set, new_capacity);
    }
    if (!has_expanded || set->length + 1 >= set->capacity){
        return false;
    }
    size_t index = _hashGetIndexFromKey(set->hash_fun, set->key_type, key, set->capacity);
    while (set->entries[index]){
        if (_hashKeyComp(set->entries[index]->key, key,set->key_type)){
            break;
        }
        index++;
        if (index == set->capacity){
            index = 0;
        }
    }
    set->entries[index] = _setCreateEntry(key, key_size);
    return true; 
}
