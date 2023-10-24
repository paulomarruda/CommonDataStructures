#include <stdio.h>
#include "../include/hash.h"


cds_int main(void){
    HashTable* ht = htCreate(fnv1aHash, 3, STR_KEY);
    if (!ht){
        return EXIT_FAILURE;
    }
    (void) htSet(ht, (const cds_char*){"foo"}, 3, &(cds_int){1}, sizeof(cds_int));
    (void) htSet(ht, (const cds_char*){"bar"}, 3, &(cds_int){2}, sizeof(cds_int));
    (void) htSet(ht, (const cds_char*){"maz"}, 3, &(cds_int){3}, sizeof(cds_int));
    (void) htSet(ht, (const cds_char*){"boo"}, 3, &(cds_int){3}, sizeof(cds_int));
    printf("HT status: \n");
    printf("    HT Length: %zu\n    HT Capacity: %zu\n", htLength(ht), htCapacity(ht));
    Iter* iter = iterCreate(ht, HASH_TABLE);
    while (iter){
        const cds_char* key = (const cds_char*) iterGetData(iter);
        if (key){
            printf("Retrieved key %s.\n", key);
        }
        iter = iterNext(iter);
    }
    htDelete(ht);
    return EXIT_SUCCESS;
}

