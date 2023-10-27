/**
 * @file vector_simple.c
 * @author Paulo Arruda
 * @copyright
 * @brief Example of simple creation, push and get operation for 
 * the vector structure.
*/
#include <stdio.h>
#include "../include/linear.h"

int main(void){
    Vector* v = vectorCreate(1000, sizeof(cds_size));
    for (cds_size i=0; i<1000; i++){
        (void) vectorPrepend(v, &i);
    }
    printf("Length: %zu\nCapacity: %zu\n", vectorLength(v), vectorCapacity(v));
    const void* data;
    for (cds_size i=0; i<vectorLength(v); i++){
        data = vectorGetAt(v, i);
        if (data){
            printf("v[%4zu] = %4zu.\n", i, *(cds_size*)data);
        }
    }
    vectorDelete(v);
    return 0;
}
