#include <stdio.h>
#include "../include/linear.h"

#define DATA_SIZE sizeof(cds_int)

int main(void){ 
    Vector* vec = vectorFromValues(DATA_SIZE, 10, 
                                   &(cds_int){1},
                                   &(cds_int){2},
                                   &(cds_int){3},
                                   &(cds_int){4},
                                   &(cds_int){5},
                                   &(cds_int){6},
                                   &(cds_int){7},
                                   &(cds_int){8},
                                   &(cds_int){9},
                                   &(cds_int){10});
    printf("Created a vector of length %zu and capacity %zu.\n", 
           vectorLength(vec), vectorCapacity(vec));
    Tuple* t = vectorToTuple(vec);
    printf("Created a tuple of size %zu.\n", tupleLength(t));
    for (Iter* iter=iterCreate(t, TUPLE); iter; iter=iterNext(iter)){
        const void* data = iterGetData(iter);
        if (data){
            printf("Retrieved: %d\n", *(cds_int*) data);
        }
    }
    return 0;
}
