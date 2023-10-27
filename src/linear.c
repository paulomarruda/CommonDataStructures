/*!
 * @file linear.c
 * @copyright GNU General Public Licence 3 or Later (GPLv3).
 * @author Paulo Arruda
 * @brief Implementation of the linear API.
*/

#include "../include/linear.h"
#include "../include/_private_hash.h"
#include <string.h>
#include <stdarg.h>

enum ContainerType{
    CDS_VECTOR,
    CDS_TUPLE,
    CDS_SLINKED_LIST,
    CDS_DLINKED_LIST,
    CDS_STACKS,
    CDS_HASH_TABLE,
    CDS_SET,
};

/** Preprossessors functions for container types */

#define LENGTH(container) (container? container->length: 0)

#define CAPACITY(container) (container? container->capacity: 0)

/**/
struct Vector{
    cds_size length;
    cds_size capacity;
    cds_size data_size;
    void* container;
};

struct Tuple{
    cds_size length;
    cds_size data_size;
    const void* container;
};


static cds_bool _checkIndex(const void* const container, const enum ContainerType type,
                            const cds_size index){
    union {
        Vector* vec;
        Tuple* tuple;
    }instance;
    cds_bool result = true;
    switch(type){
        case CDS_VECTOR:
        instance.vec = (Vector*) container;
        if (!instance.vec){
            result = false;
        }else if(!instance.vec->length || index >= instance.vec->length){
            result = false;
        }
        break;
        case CDS_TUPLE:
        instance.tuple = (Tuple*) container;
        if (!instance.tuple){
            result = false;
        }else if (index >= instance.tuple->length){
            result = false;
        }
        default:
        break;
    }
    return result;
}
/**
* VECTOR
*/

/**
 * Constructor for the dynamic array structure (vector). The array is created with
 * capacity determined by min_capacity to be the least power of 2 greater than The
 * min_capacity.
*/
Vector* vectorCreate(const cds_size min_capacity, const cds_size data_size){
    if (min_capacity <= 0){
        return (Vector*) NULL;
    }
    Vector* new_vec = (Vector*) malloc(sizeof(Vector));
    if (!new_vec){
        return (Vector*) NULL;
    }
    cds_size pow = _log2(min_capacity) + 1;
    if (pow >= _MAX_POW2_){
        free(new_vec);
        return (Vector*) NULL;
    }
    cds_size capacity = 1 << pow;
    void* container = (void*) malloc(capacity * data_size);
    if (!container){
        free(new_vec);
        return (Vector*) NULL;
    }
    new_vec->length = 0;
    new_vec->capacity = capacity;
    new_vec->data_size = data_size;
    new_vec->container = container;
    return new_vec;
}

Vector* vectorFromArray(const void* arr, const cds_size data_size, const cds_size arr_len){
    Vector* new_vec = vectorCreate(arr_len, data_size);
    if (!new_vec){
        return (Vector*) NULL;
    }
    if (!arr || 0 == data_size){
        return new_vec;
    }
    (void) memmove(new_vec->container, arr, arr_len*data_size);
    new_vec->length = arr_len;
    return new_vec;
}

Vector* vectorCopy(const Vector* const vec){
    Vector* copy = (Vector*) malloc(sizeof(Vector));
    if (!copy){
        return (Vector*) NULL;
    }
    void* container = malloc(vec->capacity * vec->data_size);
    if (!container){
    free(copy);
        return (Vector*) NULL;
    }
    copy->container = container;
    if (vec->length > 0){
        (void) memmove(copy->container, vec->container, vec->length*vec->data_size);
    }
    copy->length = vec->length;
    copy->data_size = vec->data_size;
    copy->capacity = vec->capacity;
    return copy;
}

Vector* vectorFromValues(const cds_size data_size, const cds_size num_values, ...){
    Vector* new_vec = vectorCreate(num_values, data_size);
    va_list args;
    va_start(args, num_values);
    for (cds_size i=0; i<num_values; i++){
        void* data = va_arg(args, void*);
        (void) memmove(CDS_BYTE_OFFSET(new_vec->container, i*data_size), data, data_size);
    }
    va_end(args);
    new_vec->length = num_values;
    return new_vec;
}

/** Destructor function for the dynamic structure. */
void vectorDelete(Vector* vec){
    if (!vec){
        return;
    }
    free(vec->container);
    free(vec);
}

const void* vectorGetAt(const Vector* const vec, const size_t index){
    if (!_checkIndex(vec, CDS_VECTOR, index)){
        return (void*) NULL;
    }
    void* _data = CDS_BYTE_OFFSET(vec->container, vec->data_size*index);
    return _data;
}

/** Expanding function for the vector structure.
 * It expand the array to twice its capacity. 
*/
static cds_bool _vectorExpand(Vector* vec){
    cds_size new_capacity = vec->capacity << 1;
    if (new_capacity <= vec->capacity){
        return false;
    }
    void* new_container = (void*) realloc(vec->container, new_capacity * vec->data_size);
    if (!new_container){
        return false;
    }
    vec->container = new_container;
    vec->capacity = new_capacity;
    return true;
}

/** Push back function for the dynamic array structure */
cds_bool vectorPrepend(Vector* const vec, void* data){
    if ( ((double) vec->capacity) * _EXPANSION_RATE_CHECK <= (double) vec->length){
        (void) _vectorExpand(vec);
    }
    if (vec->length + 1 > vec->capacity){
        return false;
    }
    (void) memmove(CDS_BYTE_OFFSET(vec->container, vec->length*vec->data_size), data, vec->data_size);
    vec->length++;
    return true;
}

void* vectorUpdate(Vector* const vec, void* data, const cds_size index){
    if (!_checkIndex(vec, CDS_VECTOR, index)){
        return (void*) NULL;
    }
    void* _data = (void*) vectorGetAt(vec, index);
    (void) memmove(CDS_BYTE_OFFSET(vec->container, index*vec->data_size), data, vec->data_size);
    return _data;
}
/** Get the number elements of an dynamic array */
cds_size vectorLength(const Vector* const vec){
    return LENGTH(vec);
}

/** Get the current capacity of a dynamic array*/
cds_size vectorCapacity(const Vector* const vec){
    return CAPACITY(vec);
}

/**
 * Shrinks the vector to half of its current capacity.

static cds_bool _vectorShrink(Vector* const vec){
    cds_size new_capacity = vec->capacity / 2;
    if (!new_capacity){
       return false;
    }
    void* new_container = (void*) realloc(vec->container, new_capacity*vec->data_size);
    if (!new_container){
        return false;
    }
    vec->container = new_container;
    vec->capacity = new_capacity;
    return true;
}

*/

const void* vectorToArr(const Vector* const vec){
    return vec->container;
}

Tuple* vectorToTuple(const Vector* const vec){
    Tuple* tuple = tupleFromArray(vectorToArr(vec), vec->data_size, vec->length);
    if (!tuple){
        return (Tuple*) NULL;
    }
    return tuple;
}

/**
 * TUPLES
 * ------
*/

Tuple* tupleCreate(const cds_size data_size, const cds_size length, ...){
    Tuple* new_tuple = (Tuple*) malloc(sizeof(Tuple));
    if (!new_tuple){
        return (Tuple*) NULL;
    }    
    void* container = malloc(length * data_size);
    if (!container){
        free(new_tuple);
        return (Tuple*) NULL;
    }
    if (!data_size || 0 == length){
        new_tuple->data_size = 0;
        new_tuple->length = 0;
        new_tuple->container = container;
        return new_tuple;
    }
    new_tuple->length = 0;
    va_list args;
    va_start(args, length);
    void* data = NULL;
    for (cds_size i=0; i<length; i++){
        data = va_arg(args, void*);
        (void) memmove(CDS_BYTE_OFFSET(container, i*data_size), data, data_size);
        new_tuple->length++;
    }
    va_end(args);
    new_tuple->data_size = data_size;
    new_tuple->container = container;
    return new_tuple;
}

Tuple* tupleFromArray(const void* const arr, const size_t data_size, const size_t arr_len){
    Tuple* new_tuple = (Tuple*) malloc(sizeof(Tuple));
    if (!new_tuple){
        return (Tuple*) NULL;
    }    
    void* container = malloc(arr_len*data_size);
    if (!container){
        free(new_tuple);
        return (Tuple*) NULL;
    }
    if (!arr || 0 == arr_len){
        new_tuple->data_size = 0;
        new_tuple->length = 0;
        new_tuple->container = container;
        return new_tuple;
    }
    (void) memmove(container, arr, arr_len*data_size);
    new_tuple->container = (const void*) container;
    new_tuple->length = arr_len;
    new_tuple->data_size = data_size;
    return new_tuple;
}

void tupleDelete(Tuple* tuple){
    if (!tuple){
        return;
    }
    if (tuple->container){
        free((void*) tuple->container);
    }
    free(tuple);
}

const void* tupleGetAt(const Tuple* const tuple, const cds_size index){
    if (!_checkIndex(tuple, CDS_TUPLE, index)){
        return (void*) NULL;
    }
    const void* data = (const void*) CDS_BYTE_OFFSET(tuple->container, index*tuple->data_size);
    return data;
}

cds_size tupleLength(const Tuple* const tuple){
    return LENGTH(tuple);
}

const void* tupleToArr(const Tuple* const tuple){
    return tuple->container;
}

Vector* tupleToVector(const Tuple* const tuple){
    return vectorFromArray(tuple->container, tuple->data_size, tuple->length);
}
/**
 * SINGLY LINKED LIST
 * -------------------
*/
typedef struct SLLNode{
    void* data;
    struct SLLNode* next;
}SLLNode;

struct SLList{
    cds_size data_size;
    cds_size length;
    SLLNode* head;
    SLLNode* tail;
};

/** Creator function for the singly linked list strucutre */

SLList* sllCreate(const cds_size data_size){
    SLList* new_list = (SLList*) malloc(sizeof(SLList));
    if (!new_list){
        return (SLList*) NULL;
    }
    new_list->data_size = data_size;
    new_list->length = 0;
    new_list->head = (SLLNode*) NULL;
    new_list->tail = (SLLNode*) NULL;
    return new_list;
}

void sllDelete(SLList* list){
    SLLNode* current;
    while (list->head){
        current = list->head;
        list->head = list->head->next;
        free(current);
    }
}

static SLLNode* _sllCreateNode(void* data, SLLNode* const next){
    if (!data){
        return (SLLNode*) NULL;
    }
    SLLNode* new_node = (SLLNode*) malloc(sizeof(SLLNode));
    if (!new_node){
        return (SLLNode*) NULL;
    }
    new_node->data = data;
    new_node->next = next;
    return new_node;
}

cds_bool sllPrepend(SLList* list, void* data){
    SLLNode* new_head = _sllCreateNode(data, list->head);
    if (!new_head){
        return false;
    }
    // checks for empty list.
    if (!list->head){
        list->tail = new_head;
    }
    list->head = new_head;
    list->length++;
    return true;
}

cds_bool sllAppend(SLList* list, void* data){
    if (!list->head){
        return sllPrepend(list, data);
    }
    if (!data){
        return false;
    }
    SLLNode* new_tail = _sllCreateNode(data, (SLLNode*) NULL);
    list->tail->next = new_tail;
    list->tail = new_tail;
    return true;
}

/**
 * ITERATOR
 * --------
*/
struct Iter{
    cds_size index;
    cds_size index_max;
    cds_size data_size; // for linear containers
    enum IterableType type;
    const void* container;
};

#define GET_CONTAINER(ptr_s, type) ((type*) ptr_s)->container
#define GET_LENGTH(ptr_s, type) ((type*) ptr_s)->length
#define GET_CAPACITY(ptr_s, type) ((type*) ptr_s)->capacity
#define GET_SLL_HEAD(ptr_s, type) ((type*) ptr_s)->head
#define GET_DATA_SIZE(ptr_s, type) ((type*) ptr_s)->data_size

Iter* iterCreate(const void* const container, const enum IterableType type){
    if (!container){
        return (Iter*) NULL;
    }
    Iter* new_iter = (Iter*) malloc(sizeof(Iter));
    if (!new_iter){
        return (Iter*) NULL;
    }
    switch (type){
        case VECTOR:
            new_iter->container = GET_CONTAINER(container, Vector);
            new_iter->index_max = GET_LENGTH(container, Vector);
            new_iter->data_size = GET_DATA_SIZE(container, Vector);
            break;
        case TUPLE:
            new_iter->container = GET_CONTAINER(container, Tuple);
            new_iter->index_max = GET_LENGTH(container, Tuple);
            new_iter->data_size = GET_DATA_SIZE(container, Tuple);
            break;
        case SLLIST:
            new_iter->container = GET_SLL_HEAD(container, SLList);
            new_iter->index_max = GET_LENGTH(container, SLList);
            new_iter->data_size = GET_DATA_SIZE(container, SLList);
            break;
        case HASH_TABLE:
            new_iter->container = GET_CONTAINER(container, HashTable);
            new_iter->index_max = GET_CAPACITY(container, HashTable);
            new_iter->data_size = sizeof(HTEntry);
            break;
        case SET:
            new_iter->container = GET_CONTAINER(container, Set);
            new_iter->index_max = GET_CAPACITY(container, Set);
            new_iter->data_size = sizeof(SetEntry);
            break;
    }
    new_iter->type = type;
    new_iter->index = 0;
    return new_iter;
}

void iterDelete(Iter* iter){
    free(iter);
}

#define HT_CHECK_CONT(ptr_c, type, index, capacity) ( ( (((type*) ptr_c)[index].key) || \
    !((((type*) ptr_c)[index]).is_tombstone) ) && (index < capacity) )

Iter* iterNext(Iter* iter){
    if (!iter){
        return (Iter*) NULL;
    }
    switch (iter->type){
        case SLLIST:
            iter->index++;
            iter->container = (void*) ((SLLNode*) iter->container)->next;
            if (iter->index_max == iter->index){
                iterDelete(iter);
                return (Iter*) NULL;
            }
            break;
        default:
            iter->index++;
            if (iter->index_max == iter->index){
                iterDelete(iter);
                return (Iter*) NULL;
            }
    }
    return iter;
}

const void* iterGetData(const Iter* const iter){
    if (!iter){
        return NULL;
    }
    const void* data = NULL;
    switch (iter->type){
        case VECTOR:
            data = CDS_BYTE_OFFSET(iter->container, iter->index * iter->data_size);
            break;
        case TUPLE:
            data = CDS_BYTE_OFFSET(iter->container, iter->index * iter->data_size);
            break;
        case SLLIST:
            data = ((SLLNode*) iter->container)->data;
            break;
        case HASH_TABLE:
            data = ((HTEntry*) CDS_BYTE_OFFSET(iter->container, iter->index * iter->data_size))->key;
            break;
        case SET:
            data = ((SetEntry*) CDS_BYTE_OFFSET(iter->container, iter->index * iter->data_size))->key;
            break;
    }
    return data;
}

/**
 * STACK
 * -----
 * @note Stacks will be implemented as a singly linked list.
*/

struct Stack{
    cds_size length;
    cds_size data_size;
    SLLNode* head;
};

Stack* stackCreate(cds_size data_size);

void stackDelete(Stack* stack);

cds_bool stackPush(Stack* stack, void* data);

void* stackPop(Stack* stack);

/**
 * QUEUES
 * ------
*/

struct Queue{
    cds_size length;
    cds_size data_size;
    SLLNode* tail;
};

Queue* queueCreate(cds_size data_size);

void queueDelete(Queue* queue);

cds_bool queuePush(Queue* queue);

void* queuePop(Queue* queue);
