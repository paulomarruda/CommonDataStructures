#include <stdio.h>
#include "../include/linear.h"

int main(){
    int arr[] = {2, -5, 9, 15, 18, 2, 5, 69, 21};
    cds_size arr_size = sizeof(arr)/sizeof(arr[0]);
    Tuple* tuple = tupleCreate(arr, arr_size, sizeof(int));
    const void* data;
    for (cds_size i=0; i<arr_size; i++){
        data = tupleGetAt(tuple, i);
        if (data){
            printf("%d ", *(int*) data);
        }
    }
    printf("\n");
    return 0;
} 
