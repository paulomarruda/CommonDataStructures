#include <stdio.h>
#include "../include/linear.h"

int main(void){
    Vector* v = vectorCreate(10UL, sizeof(cds_size));
    for (cds_size i=0; i<10UL; i++){
        (void) vectorPush(v, &(cds_size){i});
    }
    Iter* iter = iterCreate(v, VECTOR);
    const void* data = NULL;
    while (iter){
        data = iterGetData(iter);
        if (data){
            printf("Retrieved %d.\n", *(int*)data);
        }else{
            printf("Failed to retrieve.\n");
        }
        iter = iterNext(iter);
    }
    if (iter){
        iterDelete(iter);
    }
    vectorDelete(v);
    return 0;
}
