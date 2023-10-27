/*!
 * @file test_int_linear.c
 * @author Paulo Arruda
 * @copyright GPL v3 or later.
 * @brief Testing the linear API with integer types.
*/

#include <time.h>
#include <string.h>
#include <criterion/criterion.h>
#include "../include/linear.h"

#define LENGTH 100
#define DATA_SIZE sizeof(cds_size)

Tuple* t = NULL;

void tupleSetup(void){
    cds_size arr[LENGTH];
    for (cds_size i=0; i<LENGTH; i++){
        arr[i] = i;
    }
    t = tupleFromArray(arr, DATA_SIZE, LENGTH);
}

void tupleTeardown(void){
    tupleDelete(t);
    t = NULL;
}

TestSuite(tuple_int, .init=tupleSetup, .fini=tupleTeardown);

Test(tuple_int, tuple_basics){
    cr_assert(t, "Tuple should not be empty");
    cr_assert(LENGTH == tupleLength(t));
    srand(time(0));
    for (cds_size i=0; i<10; i++){
        const void* data = tupleGetAt(t, i);
        cr_assert(data);
        cr_expect(i == *(cds_size*) data);
    }
}
