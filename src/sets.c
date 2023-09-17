#include <string.h>
#include "../include/sets.h"

// set entries
typedef struct SetEntry{
    const void* key;
    size_t key_size;
}SetEntry;

// definition of the Set type
struct Set{
    SetEntry** keys;
    size_t capacity;
    size_t length;
    HashFunction hash_fun;
    KeyType key_type;
    bool is_copy;
};

// auxiliar function to copy data
static void* _dataDup(const void* data, const size_t data_size){
    void* new_data = (void*) malloc(data_size);
    if (!new_data){ 
        return (void*) NULL; 
    }
    memcpy(new_data, data, data_size);
    return new_data;
}

// determines the floor of log2 
static size_t _log2(size_t x){
    size_t log = 1;
    while (x>>log > 0 && x>>log != SIZE_MAX){
        log++;
    }
    return log;
}

#define MAX_POW _log2(SIZE_MAX)

/*
 * To speedup operations, the set will always have as its capacity 
 * a power of 2. In the creation, the capacity is set to be the 
 * least power of two larger than the `min_capacity` passed.
*/
Set* setCreate(const HashFunction hash_fun, const size_t min_capacity, const KeyType key_type,
               const bool is_copy){
    if (min_capacity <= 0){
        return (Set*) NULL;
    }
    Set* new_set = (Set*) malloc(sizeof(Set));
    if (!new_set){
        //handle error 
        return (Set*) NULL;
    }
    size_t pow = _log2(min_capacity) + 1;
    if (pow+1 >= MAX_POW){
        return (Set*) NULL;
    }
    size_t capacity = 1<<pow;
    SetEntry** keys = (SetEntry**) malloc(capacity*sizeof(SetEntry*));
    if (!keys){
        //handle error 
        free(new_set);
        return (Set*) NULL;
    }
    new_set->keys = keys;
    new_set->capacity = capacity;
    new_set->length = 0;
    new_set->hash_fun = hash_fun;
    new_set->key_type = key_type;
    new_set->is_copy = is_copy;
    return new_set;
}

void setDelete(Set* _set){
    if (!_set){
        return;
    }
    if (_set->is_copy){
        for (size_t i=0; i<_set->capacity; i++){
            free(_set->keys[i]);
        }
    }
    free(_set->keys);
    free(_set);
}

static size_t _setGetIndexFromKey(HashFunction hash_fun, const void* key, const KeyType key_type, 
                                  const size_t capacity){
    uint64_t hash = hash_fun(key, key_type);
    // since the capacity is a power of 2, the reminder is easily 
    // calculated with & operator
    size_t index = (size_t) (hash & ((uint64_t) capacity - 1));
    return index;
}

/*
 * If more key types are implemented, this function has to be updated.
*/
static bool _compKeys(const void* key1, const void* key2, const KeyType key_type){
    switch(key_type){
        case STR_KEY:
            if (0 == strcmp(key1, key2) ){
                return true;
            }
            break;
        case INT_KEY:
            return *(int*) key1 == *(int*) key2;
            break;
    }
    return false;
}

bool setContains(const Set* _set, const void* key){
    if (!_set || !key){
        return false;
    }
    size_t index = _setGetIndexFromKey(_set->hash_fun, key, _set->key_type, _set->capacity);
    while (_set->keys[index]){
        if (_compKeys(_set->keys[index]->key, key, _set->key_type)){
            return true;
        }
        index++;
        if (index == _set->capacity){
            index = 0;
        }
    }
    return false;
}

// expands the set to twice its capacity and re-hash the keys
static bool _setExpand(Set* _set, const size_t new_capacity){
    SetEntry** new_keys = (SetEntry**) malloc(new_capacity*sizeof(SetEntry*));
    if (!new_keys){
        //handle errors
        return false;
    }
    for (size_t i=0; i<_set->capacity; i++){
        if (_set->keys[i]){
            size_t index = _setGetIndexFromKey(_set->hash_fun, _set->keys[i]->key, _set->key_type, 
                                               new_capacity);
            while (new_keys[index]){
                index++;
                if (new_capacity == index){
                    index = 0;
                }
            }
            new_keys[index] = _set->keys[i];
        }
    }
    free(_set->keys);
    _set->keys = new_keys;
    _set->capacity = new_capacity;
    return true;
}

static SetEntry* _setCreateEntry(const void* key, const size_t key_size, const bool as_copy){
    SetEntry* new_entry = (SetEntry*) malloc(sizeof(SetEntry));
    if (!new_entry){
        return (SetEntry*) NULL;
    }
    const void* _key = key;
    if (as_copy){
        _key = _dataDup(key, key_size);
        if (!_key){
            free(new_entry);
            return (SetEntry*) NULL;
        }
    }
    new_entry->key = _key;
    new_entry->key_size = key_size;
    return new_entry;
}

bool setInsert(Set* _set, const void* key, const size_t key_size){
    if (!_set || !key){
        //handle errors
        return false;
    }
    if (setContains(_set, key)){
        return false;
    }
    SetEntry* new_entry = _setCreateEntry(key, key_size, _set->is_copy);
    if (!new_entry){
        return false;
    }
    if ((double) _set->length >= ((double) _set->capacity)*EXPAND_RATE_CHECK){
        size_t new_capacity = _set->capacity<<1;
        if (new_capacity <= _set->capacity && _set->length + 1 == SIZE_MAX){
            return false;
        }
        (void) _setExpand(_set, new_capacity);
    }
    size_t index = _setGetIndexFromKey(_set->hash_fun, key, _set->key_type, _set->capacity);
    while (_set->keys[index]){
        index++;
        if (index == _set->capacity){
            index = 0;
        }
    }
    _set->keys[index] = new_entry;
    _set->length++;
    return true;
}

