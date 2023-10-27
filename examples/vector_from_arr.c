#include <stdio.h>
#include "../include/linear.h"

int main(void){
    int arr[] = {5,8,-3,-15,1,-1,0,2,3};
    const int data_size = sizeof(arr[0]);
    const int len = sizeof(arr)/data_size;
    Vector* v = vectorFromArray(arr, data_size, len);
    for (Iter* iter = iterCreate(v,VECTOR); iter; iter=iterNext(iter)){
        const void* data = iterGetData(iter);
        if (data){
            printf("retrieved %3d\n", *(int*)data);
        }
    }
    vectorDelete(v);
    return 0;
}
