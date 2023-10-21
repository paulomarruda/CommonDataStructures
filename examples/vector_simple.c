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
    int arr[] = {50,-15, 2, 0, 3, 8, 18, 1, -1, 15, 10, -50}; 
    cds_size arr_size = sizeof(arr)/sizeof(arr[0]);
    Vector* vec = vectorCreate(arr_size, sizeof(int));
    for (cds_size i=0; i<arr_size; i++){
        (void) vectorPush(vec, &arr[i]);
    }
    printf("Created vector of int\nLength: %zu\nCapacity: %zu\n", 
           vectorLength(vec), vectorCapacity(vec));
    printf("Vector:\n");
    const void* data;
    for (cds_size i=0; i<arr_size; i++){
        data = vectorGetAt(vec, i);
        if (data){
            printf("%d ", *(int*) data);
        }
    }
    printf("\nLooping with iterators\n");
    Iter* iter = iterCreate(vec, VECTOR);
    while (iter){
        data = iterGetData(iter);
        if (data){
            printf("%d ", *(int*) data);
        }
        iter = iterNext(iter);
    }
    printf("\n");
    vectorDelete(vec);
}
