#ifndef CDS_COMMON_H
#define CDS_COMMON_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

/**
 * @brief Constant definition to be used by container structures.
 *
 * This constant is used to container structures to determine
 * the its initial capacity: given a minimum capacity passed to
 * the constructor function, the capacity stored will be the least
 * power of 2 larger or equal to the minimum capacity passed.
*/
#define _MAX_POW2_ 64

/**
 * @brief Constant definition to be used when expanding container structures.
 *
 * This constant is used to determine when a container structure should expand.
 * The rule adpoted is whenever the length of the container structure is at least 
 * `_EXPASION_RATE_CHECK * struct->length`, then the structure will expand its 
 * capacity to twice its actual capacity.
*/
#define _EXPANSION_RATE_CHECK 0.85


/**
 * CDS TYPES
 *
 * @brief definition of the types used by the library.
*/
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
typedef int64_t         cds_int64;
typedef uint64_t        cds_uint64;

typedef float           cds_float;
typedef double          cds_double;
typedef long double     cds_ldouble;


#define CDS_BYTE_OFFSET(ptr, nbytes) ((void*) ((cds_byte*)ptr + nbytes))

/**
 * STATUS HANDLING
*/

enum StatusType{
    // SUCCESS
    STATUS_SUCCESS                  = 0,
    // WARNINGS OR ERROR
    STATUS_ALLOCATION_FAILED        = 1,
    STATUS_TYPE_OVERFLOW            = 2,
    STATUS_MAX_CAPACITY_REACHED     = 3,
    STATUS_INVALID_SIZE             = 4,
    STATUS_INVALID_RANGE            = 5,
    STATUS_NULL_STRUCT              = 6,
};

enum StatusAction{
    ACTION_EXIT = 0,
    ACTION_WARN = 1,
};

void _raise(enum StatusType type, enum StatusAction action, const char* message);

struct CDSStatus{
    bool has_ocurred;
    enum StatusType type;
    enum StatusAction action;
    const char* message;
};

extern struct CDSStatus CDS_ERROR;

extern struct CDSStatus CDS_WARNING;

//cross compailer

#ifdef __GNUC__ 
    //GCC 
#endif // __GNUC__

#ifdef _MSC_VER
    // MSVC
#endif // _MSC_VER 

// cross-plataform

// functions used

size_t _log2(const size_t x);

size_t _strLen(const char* s);

const void* _dataDup(const void* const data, const size_t data_size);

const char* _strDup(const char* string);

#endif //COMMON_H
