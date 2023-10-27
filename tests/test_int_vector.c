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

/****************  INT VECTOR TESTS ***************/

#define MIN_CAPACITY 1000
#define INIT_CAPACITY 1024
#define DATA_SIZE sizeof(cds_size)

Vector* v = (Vector*) NULL;

void vectorSetup(void){
    v = vectorCreate(MIN_CAPACITY, DATA_SIZE);
    cr_assert(v, "vectorCreate should return a not NULL vector");
}

void vectorTeardown(void){
    vectorDelete(v);
    v = (Vector*) NULL;
}

TestSuite(vector_int, .init=vectorSetup, .fini=vectorTeardown);

/*
 * Testing simple creation of vectors and the retrieving of the length and the capacity
 * at creation time.
*/
Test(vector_int, vector_basics){
    cds_size length = vectorLength(v);
    cds_size capacity = vectorCapacity(v);
    cr_expect(0 == length, "Initial length of the vector should be 0");
    cr_expect(INIT_CAPACITY == capacity, "Initial length of the vector should be 1024");
    cr_expect(!vectorGetAt(v, 0), "Vector is empty. Expected a NULL pointer.");
}

Test(vector_int, vector_from_array){
    cds_size arr[] = {0, 1 , 2, 3, 4, 5, 6};
    const cds_size data_size = sizeof(arr[0]);
    const cds_size arr_len = sizeof(arr)/sizeof(data_size);
    Vector* vec = vectorFromArray(arr, data_size, arr_len);
    cr_assert(vec, "Vector should not be NULL.");
    for (cds_size i=0; i<arr_len; i++){
        const void* data = vectorGetAt(vec, i);
        cr_assert(data);
        cr_expect(i == *(cds_size*) data);
    }
    cr_expect(!vectorGetAt(vec, 200), "Vector does not have a 200th position. Expected a NULL pointer.");
}

Test(vector_int, vector_creation_from_values){
    Vector* vec = vectorFromValues(DATA_SIZE, 10, 
                                   &(cds_size){1},
                                   &(cds_size){2},
                                   &(cds_size){3},
                                   &(cds_size){4},
                                   &(cds_size){5},
                                   &(cds_size){6},
                                   &(cds_size){7},
                                   &(cds_size){8},
                                   &(cds_size){9},
                                   &(cds_size){10});
    cr_assert(vec);
    cr_assert(10 == vectorLength(vec));
    cr_assert(16 == vectorCapacity(vec));
    for (cds_size i=0; i<10; i++){
        const void* data = vectorGetAt(vec, i);
        cr_assert(data);
        cr_expect(i+1 == *(cds_size*)data, "Wrong value added. Expected: %zu. Got: %zu.", i+1, *(cds_size*)data);
    }
    cr_expect(10 == vectorLength(vec), "Expected vector length 10");
    vectorDelete(vec);
}

/*
 * Testing simple push back operation and retrieval without the expansion.
*/
Test(vector_int, pushing_without_expansion){
    for (cds_size i=0; i<10; i++){
        cr_expect(vectorPrepend(v, &(cds_size){i}), "Expected pushing operation to succeed");
    }
    cr_expect(10 == vectorLength(v), "Length after pushing should be 10");
    cr_expect(INIT_CAPACITY == vectorCapacity(v), "Capacity should not have increased.");
    for (cds_size i=0; i<10; i++){
        const void* data = vectorGetAt(v, i);
        cr_expect(data, "Data retrieved should not be null");
        cr_expect(i == *(cds_size*) data);
    }
}

/*
 * Testing simple push back operation and retrieval with expansion.
*/
Test(vector_int, pushing_with_expansion){ 
    for (cds_size i=0; i<INIT_CAPACITY; i++){
        cr_expect(vectorPrepend(v, &(cds_size){i}), "Expected pushing operation to succeed");
    }
    cr_expect(INIT_CAPACITY == vectorLength(v), "Length after pushing should be 1024");
    cr_expect(2*INIT_CAPACITY == vectorCapacity(v), "Capacity should have doubled.");
    for (cds_size i=0; i<INIT_CAPACITY; i++){
        const void* data = vectorGetAt(v, i);
        cr_expect(data, "Data retrieved should not be null");
        cr_expect(i == *(cds_size*) data, "Expected value: %zu\nGot: %zu", i, *(cds_size*)data);
    }
}

/*
 * Testing simple push back operation with expansion and data retrieving with iterators.
*/
Test(vector_int, vector_iterator){
    for (cds_size i=0; i<INIT_CAPACITY; i++){
        cr_expect(vectorPrepend(v, &(cds_size){i}), "Expected pushing operation to succeed.");
    } 
    cds_size i = 0;
    Iter* iter = iterCreate(v, VECTOR);
    cr_assert(iter);
    while (iter){
        const void* data = vectorGetAt(v, i);
        cr_assert(data);
        cr_assert(i == *(cds_size*)data, "Wrong value. Expected: %zu. Got: %zu", i, *(cds_size*)data);
        i++;
        iter = iterNext(iter);
    }
}
Test(vector_int, vector_copying){    
    for (cds_size i=0; i<INIT_CAPACITY; i++){
        cr_expect(vectorPrepend(v, &(cds_size){i}), "Expected pushing operation to succeed.");
    }
    Vector* copy = vectorCopy(v);
    cr_assert(copy);
    cr_expect(vectorLength(copy) == vectorLength(v));
    for (cds_size i=0; i<vectorLength(v); i++){
        const void* data_v = vectorGetAt(v,i);
        cr_assert(data_v);
        const void* data_c = vectorGetAt(copy,i);
        cr_assert(data_c);
        cr_expect(*(cds_size*) data_v == *(cds_size*) data_c);
    }
}

Test(vector_int, vector2tuple){
    for (cds_size i=0; i<INIT_CAPACITY; i++){
        cr_expect(vectorPrepend(v, &(cds_size){i}), "Expected pushing operation to succeed.");
    }
    Tuple* t = vectorToTuple(v);
    cr_assert(t);
    cr_expect(vectorLength(v) == tupleLength(t));
    for (cds_size i=0; i<INIT_CAPACITY; i++){
        const void* data_v = vectorGetAt(v,i);
        cr_assert(data_v);
        const void* data_t = tupleGetAt(t,i);
        cr_assert(data_t);
        cr_expect(*(cds_size*)data_v == *(cds_size*)data_t);
    }
}

