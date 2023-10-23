#include <stdio.h>
#include "../include/hash.h"


cds_int main(void){
    HashTable* ht = htCreate(fnv1aHash, 3, STR_KEY);
    if (!ht){
        return EXIT_FAILURE;
    }
    const char* key1 = "foo";
    const char* key2 = "bar";
    const char* key3 = "boo";
    const char* key4 = "baa";
    const int val1 = 0;
    const int val2 = -5;
    const int val3 = 14;
    const int val4 = -1;
    (void) htSet(ht, key1, 3, &val1, sizeof(int));
    (void) htSet(ht, key2, 3, &val2, sizeof(int));
    (void) htSet(ht, key3, 3, &val3, sizeof(int));
    (void) htSet(ht, key4, 3, &val4, sizeof(int));
    printf("HT status: \n");
    printf("    HT Length: %zu\n    HT Capacity: %zu\n", htLength(ht), htCapacity(ht));
    const void* data = htGet(ht, key3, (cds_size*) NULL);
    if (data){
        printf("Retrived data=%d at key=%s\n", *(cds_int*) data, key3);
    }
    htDelete(ht);
    return EXIT_SUCCESS;
}

