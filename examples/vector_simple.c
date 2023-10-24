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
    Vector* v = vectorCreate(10, sizeof(cds_size));
    for (cds_size i=0; i<10; i++){
        (void) vectorPush(v, &(cds_size){i});
    }
    printf("Length: %zu\nCapacity: %zu\n", vectorLength(v), vectorCapacity(v));
    void* data = vectorGetAt(v, 5);
    if (data)
        printf("Retrieving element %d at position %zu\n", *(int*) data, 5UL);
    else{
        printf("Failed to retrieve\n");
    }
    vectorDelete(v);
    return 0;
}
