#include <stdio.h>
#include <string.h>
#include "../include/hash.h"

/*
 * Hash Tables
*/

typedef struct HashTableEntry{
    union {
        const char* str_key;
        int int_key;
    };
    void* data;
    size_t data_size;
}HashTableEntry;


struct HashTable{
    HashTableEntry** entries;
    size_t capacity;
    size_t length;
    HashFunction hash_fun;
    bool is_copy;
    KeyType key_type;
};

size_t _log2(size_t x){
    size_t log = 1;
    while( (x >> log) > 0 && (x>>log) != x){ log++; }
    return log-1;
}

static size_t _htGetIndexFromKey(const HashFunction hash_fun, KeyType key_type, void* key, 
                                  const size_t capacity){

    uint64_t hash = hash_fun( (void*) key, key_type);
    // since the capacity is always a power of two
    // the reminder can be done easily by the following bitwise op
    size_t index = (size_t) (hash & ( (uint64_t)(capacity - 1) ));
    return index;
}
static size_t _htGetIndexFromEntry(const HashFunction hash_fun, KeyType key_type, HashTableEntry* entry, 
                                  const size_t capacity){
    void* key;
    switch (key_type){
        case STR_KEY:
            key = (void*) entry->str_key;
            break;
        case INT_KEY:
            key = &(entry->int_key);
    }
    uint64_t hash = hash_fun( (void*) key, key_type);
    // since the capacity is always a power of two
    // the reminder can be done easily by the following bitwise op
    size_t index = (size_t) (hash & ( (uint64_t)(capacity - 1) ));
    return index;
}
static size_t _strLen(const char* string){
    size_t len = 0;
    while (*string != '\0'){
        len++;
        string++;
    }
    return len;
}
static const char* _strDup(const char* string){
    size_t len = _strLen(string);
    const char* new_str = (char*) malloc(len+1 * sizeof(char));
    if (!new_str){ 
        return (char*) NULL; 
    }
    memcpy( (void*) new_str, string, len+1);
    return new_str;
}

static void* _dataDup(void* data, size_t data_size){
    void* new_data = (void*) malloc(data_size);
    if (!new_data){ 
        return (void*) NULL; 
    }
    memcpy(new_data, data, data_size);
    return new_data;
}

#define MAX_POW _log2(SIZE_MAX)

HashTable* htCreate(const HashFunction hash_fun, const size_t min_capacity, 
                   const bool copy_data, const KeyType key_type){
    if (min_capacity <= 0){
        // handle error
        return (HashTable*) NULL;
    }
    size_t pow = _log2(min_capacity) + 1;
    if (pow >= MAX_POW){
        //handle size_t overflow error 
        return (HashTable*) NULL;
    }
    //make the capacity the next power of 2 
    size_t capacity = 1 << pow;
    HashTable* new_table = (HashTable*) malloc(sizeof(HashTable));
    if (!new_table){
        //handle errors
        return (HashTable*) NULL;
    }
    HashTableEntry** entries = (HashTableEntry**) malloc(capacity*sizeof(HashTableEntry*));
    if (!entries){
        //handle errors
        free(new_table);
        return (HashTable*) NULL;
    }
    new_table->key_type = key_type;
    new_table->entries = entries;
    new_table->capacity = capacity;
    new_table->length = 0;
    new_table->hash_fun = hash_fun;
    new_table->is_copy = copy_data;
    return new_table;
}

static void _htDestroyEntry(HashTableEntry* entry, KeyType type, bool is_copy){
    if (is_copy){
        switch (type){
            case STR_KEY:
                free((void*) entry->str_key);
                break;
            case INT_KEY:
                break;
        }
        free(entry->data);
    }
    free(entry);
}

void htDelete(HashTable* table){
    if (!table){ 
        return;
    }
    // check if the table stores the original data or a copy 
    for (size_t i=0; i<table->capacity; i++){
        if (table->entries[i]){
            _htDestroyEntry(table->entries[i], table->key_type, table->is_copy);
        }
    }
    free(table->entries);
    free(table);
}

static HashTableEntry* _htCreateEntry(void* key, void* data, const size_t data_size,
                                    const bool copy_data, KeyType type){
    HashTableEntry* new_entry = (HashTableEntry*) malloc(sizeof(HashTableEntry));
    if (!new_entry){
        //handle error 
        return (HashTableEntry*) NULL;
    }
    new_entry->data_size = data_size;
    switch (type){
        case STR_KEY:
            if (copy_data){
                new_entry->data = _dataDup(data, data_size);
                new_entry->str_key = _strDup((const char*) key);
                if (!new_entry->str_key || !new_entry->data){
                    //handle error
                    free(new_entry);
                    return (HashTableEntry*) NULL;
                }
                return new_entry;
            }
            new_entry->data = data;
            new_entry->str_key = (const char*) key;
            break;
        case INT_KEY:
            new_entry->data_size = data_size;
            new_entry->int_key = *(int*) key;
            if (copy_data){
                new_entry->data = _dataDup(data, data_size);
                if (!new_entry){
                    //handle it 
                    free(new_entry);
                    return (HashTableEntry*) NULL;
                }
                return new_entry;
            }
            new_entry->data = data;
    }
    return new_entry;
}

static bool _htUpdateEntry(HashTableEntry* entry, void* data, const size_t data_size, 
                        const bool as_copy){
    if (as_copy){
        void* _data = _dataDup(data, data_size);
        if (!_data){
            return false;
        }
        free(entry->data);
        entry->data = _data;
        entry->data_size = data_size;
        return true;
    }
    entry->data = data;
    entry->data_size = data_size;
    return true;
}

static bool _keyComp(HashTableEntry* entry, void* key, KeyType key_type){
    switch (key_type){
        case INT_KEY:
            if ( *(int*)key == entry->int_key){
                return true;
            }
            break;
        case STR_KEY:
            if (0 == strcmp(entry->str_key, (char*) key) ){
                return true;
            }
    }
    return false;
}

static bool _htExpand(HashTable* ht, const size_t new_capacity){
    HashTableEntry** new_entries = (HashTableEntry**) malloc(new_capacity*sizeof(HashTableEntry*));
    if (!new_entries){
        //handle errors
        return false;
    }
    for (size_t i=0; i<ht->capacity; i++){
        if (ht->entries[i]){
            size_t new_index = _htGetIndexFromEntry(ht->hash_fun, ht->key_type, ht->entries[i], 
                                                new_capacity);
            while (new_entries[new_index]){
                new_index++;
                if (new_index == new_capacity){
                    new_index = 0;
                }
            }
            new_entries[new_index] = ht->entries[i];
        }
    }
    free(ht->entries);
    ht->entries = new_entries;
    ht->capacity = new_capacity;
    return true;
}

#define _EXPANSION_RATE_CHECK 0.75

static bool _htSetData(HashTable *ht, void* key, void* data, const size_t data_size){
    size_t index = _htGetIndexFromKey(ht->hash_fun, ht->key_type, key, ht->capacity);
    while (ht->entries[index]){
        if (_keyComp(ht->entries[index], key, ht->key_type)){
            return _htUpdateEntry(ht->entries[index], data, data_size, ht->is_copy);
        }
        index++;
        if (index == ht->capacity){
            index = 0;
        }
    }
    ht->entries[index] = _htCreateEntry(key, data, data_size, ht->is_copy, ht->key_type);
    if (!ht->entries[index]){
        //handle errors
        return false;
    }
    ht->length++;
    return true;
}

bool htSet(HashTable* ht, void* key, void* data, size_t data_size){
    if (!key || !data){
        // handle error
        return false;
    }
    if ( ((double) ht->length) >= _EXPANSION_RATE_CHECK * ((double) ht->capacity)){
        size_t new_capacity = ht->capacity << 1;
        // always have at least one NULL space at the table,
        // so that the searching process can stop
        if (new_capacity <= ht->capacity && ht->length + 2 == ht->capacity){
            //handle overflow of size_t
            return false;
        }else{
            (void) _htExpand(ht, new_capacity);
        }
    }
    // we have at least two NULL spaces at this point
    return _htSetData(ht, key, data, data_size);
}

void* htGet(const HashTable* ht, void* key, const bool get_copy){
    size_t index = _htGetIndexFromKey(ht->hash_fun, ht->key_type, key, ht->capacity);
    void* data = (void*) NULL;
    while (ht->entries[index]){
        if ( _keyComp(ht->entries[index], key, ht->key_type) ){
            data = ht->entries[index]->data;
            break;
        }
        index++;
        if (index == ht->capacity){
            index = 0;
        }
    }
    if (get_copy && ht->entries[index]){
        data = _dataDup(ht->entries[index]->data, ht->entries[index]->data_size);
        if (!data){
            //handle error
        }
    }
    return data;
}

size_t htLength(const HashTable* ht){
    return ht->length;
}
size_t htCapacity(const HashTable* ht){
    return ht->capacity;
}

typedef struct HashTableIter{
    const HashTable* ht;
    size_t index;
}HashTableIter; 

HashTableIter* htiterCreate(const HashTable* ht){
    if (!ht){
        return (HashTableIter*) NULL;
    }
    HashTableIter* new_iter = (HashTableIter*) malloc(sizeof(HashTableIter));
    if (!new_iter){
        //handle error
        return (HashTableIter*) NULL;
    }
    new_iter->index = 0;
    while (!ht->entries[new_iter->index]){
        new_iter->index++;
    }
    new_iter->ht = ht;
    return new_iter; 
}

HashTableIter* htiterNext(HashTableIter* iter){
    do{
        iter->index++;
        if (iter->index == iter->ht->capacity){
            return (HashTableIter*) NULL;
        }
    }while(!(iter->ht->entries[iter->index]));    
    return iter;
}

void* htGetKeyFromIter(const HashTableIter* const iter){
    if (!iter){
        return (void*) NULL;
    }
    if (iter->ht->entries[iter->index]){
        switch(iter->ht->key_type){
            case INT_KEY:
                return &(iter->ht->entries[iter->index]->int_key);
            case STR_KEY:
                return (void*) iter->ht->entries[iter->index]->str_key; 
        }
    }else{
        return (void*) NULL;
    }
}

void* htGetDataFromIter(const HashTableIter* const iter){
    if (!iter){
        return (void*) NULL;
    }
    return iter->ht->entries[iter->index]->data;
}
