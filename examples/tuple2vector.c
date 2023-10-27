#include <stdio.h>
#include "../include/linear.h"

#define DATA_SIZE sizeof(cds_size)

int main(void){
    Tuple* t = tupleCreate(DATA_SIZE, 5,
                            &(cds_size){1},
                            &(cds_size){2},
                            &(cds_size){3},
                            &(cds_size){4},
                            &(cds_size){5});
    printf("Create tuple of length: %zu\n", tupleLength(t));
    Vector* v = tupleToVector(t);
    printf("Create vector of length: %zu\n", vectorLength(v));
    for (Iter* iter = iterCreate(v,VECTOR); iter; iter=iterNext(iter)){
        const void* data = iterGetData(iter);
        if (data){
            printf("retrieved %zu\n", *(cds_size*)data);
        }
    }
    vectorDelete(v);
    tupleDelete(t);
    return 0;
}
