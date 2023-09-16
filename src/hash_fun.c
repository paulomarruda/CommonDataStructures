#include <stdint.h>
#include <limits.h>
#include "../include/hash_fun.h"

#define _FNV_OFFSET 14695981039346656037UL
#define _FNV_PRIME 1099511628211UL

uint64_t FNV1aHash(void* key, KeyType type){
    uint64_t hash = _FNV_OFFSET;
    const char* str_key = (void*) NULL;
    uint64_t int_key = 0;;
    switch (type) {
        case STR_KEY:
            str_key = (const char*) key;
            for (const char* p = str_key; *p!='\0'; p++){
                hash ^= (uint64_t) (unsigned char) *p;
                hash *= _FNV_PRIME;
            }
            break;
        case INT_KEY:
            int_key = (uint64_t) *(int*) int_key;
            hash ^= int_key;
            hash *= _FNV_PRIME;
            break;
    }
    return hash;
}
