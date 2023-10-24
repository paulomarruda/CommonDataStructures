# Introduction
**Common Data Structures** (CDS): A collection of data structures and algorithms commonly used. It includes APIs for the following data structures:

- **Vector**: A dynamic allocated array inspired by the C++ `std::vector` class.
- **Tuple**: An immutable array-like structure inspired by Python's `tuple` class.
- **SLList**: A singly linked list.
- **Stack**: A (singly linked list implemented) stack.
- **Queue**: A (singly linked list implemented) queue
- **HashTable**: An unordered key hash table similar to C++ `std::unordered_map`.
- **Set**: An unordered set.

All data structures provided here are generic, in the sense that, in principle, any data type is accepted. 

# Compound literals
All container types store values instead of references. As such, when initializing structures, the user can use [compond literals](https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html):
```c
Vector* v = vectorCreate(10UL, sizeof(cds_size));
vectorPush(v, &(int){1});
```
# Types
We use the following aliases for basic types:
```c
typedef char            cds_char;
typedef unsigned char   cds_uchar;

typedef int8_t          cds_byte;

typedef int             cds_int;
typedef bool            cds_bool;
typedef short           cds_short;
typedef long            cds_long;
typedef size_t          cds_size;
typedef int8_t          cds_int8;
typedef uint8_t         cds_uint8;
typedef int16_t         cds_int16;
typedef uint16_t        cds_uint16;
typedef int32_t         cds_int32;
typedef uint32_t        cds_uint32;
typedef float           cds_float;
typedef double          cds_double;
typedef long double     cds_ldouble;
```
If the system is 64bits, the following are also defined 
```c
typedef int64_t         cds_int64;
typedef uint64_t        cds_uint64;
typedef cds_uint64      cds_hash;
typedef cds_uint64      cds_uintkey;
typedef cds_int64       cds_intkey;
```
Otherwise, i.e. in 32bits systems, the following are defined
```c
typedef cds_uint32      cds_hash;
typedef cds_uint32      cds_uintkey;
typedef cds_int32       cds_intkey;
```

# Linear Containers
The `linear.h` defines types of linear container data structures. It contains type definitions and methods for the following data structures:
- **Vector** 
- **Tuple**
- **SLList**
- **Stack**:
- **Queue**: 
The `linear` API also provides an iterator structure and methods capable of dealing with some containers in the `linear` API and the `hash` API. The following enumeration describes the iterable containers:
```c
enum IterableType{
    VECTOR,
    TUPLE,
    SLLIST,
    HASH_TABLE,
    SET,
};
```
## Vector
We show below simple examples of dealing with the vector structure:

#### Simple Example
Below is a simple example of creating initializing and deleting a vector:
```c
#include <stdio.h>
#include "linear.h"

int main(void){
    Vector* v = vectorCreate(10UL, sizeof(cds_size));
    for (cds_size i=0; i<10UL; i++){
        (void) vectorPush(v, &(cds_size){i});
    }
    printf("Length: %zu\nCapacity: %zu\n", vectorLength(v), vectorCapacity(v));
    void* data = vectorGetAt(v, 5UL);
    if (data)
        printf("Retrieving element %d at position %zu\n", *(int*) data, 5UL);
    else{
        printf("Failed to retrieve\n");
    }
    vectorDelete(v);
    return 0;
}
```
### Iterating over Vectors
Below is a simple example of iterating data over a vector.
```c
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
```
Note that you only need to call `iterDelete` if the loop is interrupted before it reaches the end of the vector since the `iterNext` function deletes the iterator and sets it to `NULL` at the end of the iteration.

## Tuple 

## Simple Example
The example below shows how to create a tuple using a variadic number of values and retrieve the values from a tuple using the iterator.
```c
#include <stdio.h>
#include "linear.h"

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
```

# Hash Containers
TO-DO
## Sets
TO-DO
# Hash Tables
TO-DO

# TO-DO:
- add unit tests;
- Add error handling;
- Add cross-compiler preprocessors;
- Add cross-plataform preprossessors;
- leetcode examples; and
- Add SipHash function for hashing structures.
