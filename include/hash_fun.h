
#ifndef HASH_FUN_H
#define HASH_FUN_H 
#include <stdint.h>
#include <stdlib.h> 
#include <stdint.h> 

typedef enum KeyType{
    STR_KEY,
    INT_KEY,
}KeyType;

typedef uint64_t (*HashFunction)(void *key, KeyType type);

uint64_t FNV1aHash(void* key, KeyType type);

#endif //HASH_FUN_MAP 
