# Introduction
A collection of data structures and algorithms I use. Written for 64 bytes wordsize. Still in development.

# Vector
#Simple example
# Sets
## Simple example

# Hash Tables
The `hash.h` header contains a simple implementation of a hash table.

## Simple Example
In the example below, we show how to create a simple hash table:
```c
#include <stdio.h>
#include "hash.h"
#define DATA_SIZE sizeof(cds_int)

cds_int main(){
    HashTable* ht = htCreate(FNV1aHash, 3, STR_KEY);
    if (!ht){
        return EXIT_FAILURE;
    }
    const cds_char* key1 = "foo";
    const cds_char* key2 = "bar";
    const cds_char* key3 = "boo";
    const cds_int val1 = 0;
    const cds_int val2 = -5;
    const cds_int val3 = 14;
    (void) htSet(ht, key1, 3, &val1, sizeof(DATA_SIZE));
    (void) htSet(ht, key2, 3, &val2, sizeof(DATA_SIZE));
    (void) htSet(ht, key3, 3, &val3, sizeof(DATA_SIZE));
    printf("HT status: \n");
    printf("    HT Length: %zu\n    HT Capacity: %zu\n", htLength(ht), htCapacity(ht));
    const void* data = htGet(ht, key2);
    if (data){
        printf("Retrived data=%d at key=%s\n", *(cds_int*) data, key2);
    }
    htDelete(ht);
    return EXIT_SUCCESS;
}
```

# TO-DO:
- add unit tests.
- Add error handling;
- Add cross-compiler preprocessors;
- Add cross-plataform preprossessors; and
- Add SipHash function for hashing structures.
