#include <stdbool.h>
#include "../include/comparison_functions.h"

int8_t intOrderComp(const void* const x, const void* const y){
    int _x = *(int*) x;
    int _y = *(int*) y;

    if (_x < _y){
        return -1;
    }else if(_x == _y){
        return 0;
    }else{
        return 1;
    }
}
