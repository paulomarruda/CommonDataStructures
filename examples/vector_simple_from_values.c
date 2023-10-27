#include <stdio.h>
#include "../include/linear.h"

#define DATA_SIZE sizeof(cds_size)

int main(void){
    Vector* v = vectorFromValues(DATA_SIZE, 5, 
                                 &(cds_size){1},
                                 &(cds_size){2},
                                 &(cds_size){3},
                                 &(cds_size){4},
                                 &(cds_size){5});
    Iter* iter = iterCreate(v, VECTOR);
    cds_size i = 0;
    while (iter){
        const void* data_i = iterGetData(iter);
        const void* data_g = vectorGetAt(v, i);
        if (data_i && data_g){
            printf("(iter) => v[%zu] = %zu\n", i, *(cds_size*)data_i);
            printf("(get) => v[%zu] = %zu\n", i, *(cds_size*)data_g);
        }
        i++;
        iter = iterNext(iter);
    }
    vectorDelete(v);
    return 0;
}
