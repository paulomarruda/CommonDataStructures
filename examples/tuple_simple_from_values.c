#include <stdio.h>
#include "../include/linear.h"

int main(void){
    Tuple* tuple = tupleCreate(sizeof(int), 10UL, 
                            &(int){1},
                            &(int){2},
                            &(int){3},
                            &(int){4},
                            &(int){5},
                            &(int){6},
                            &(int){7},
                            &(int){8},
                            &(int){9},
                            &(int){10});
    Iter* iter = iterCreate(tuple, TUPLE);
    printf("Created nre tuple:\n\tLength: %zu\n", tupleLength(tuple));
    const void* data = NULL;
    while (iter){
        data = iterGetData(iter);
        if (data){
            printf("Retrieved value %d.\n", *(int*)data);
        }else{
            printf("Failed to retrieved.\n");
        }
        iter = iterNext(iter);
    }
    tupleDelete(tuple);
}
