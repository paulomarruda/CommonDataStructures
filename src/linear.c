/*!
 * @file linear.c
 * @copyright GNU General Public Licence 3 or Later (GPLv3).
 * @author Paulo Arruda
 * @brief Implementation of the linear API.
*/


#include "../include/linear.h"
#include <string.h>

enum ContainerType{
    C_VECTOR,
    C_TUPLE,
    C_SLINKED_LIST,
    C_DLINKED_LIST,
    C_STACKS,
};

/** Preprossessors functions for container types */

#define LENGTH(container) container->length

#define CAPACITY(container) container->capacity


/**
 * 
 * ------------
*/
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
        case C_VECTOR:
        instance.vec = (Vector*) container;
        if (!instance.vec){
            result = false;
        }else if(!instance.vec->length || index >= instance.vec->length){
            result = false;
        }
        break;
        case C_TUPLE:
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
        _raise(STATUS_INVALID_SIZE,
               LINEAR_DEFAULT_STATUS_ACTION_INVALID_SIZE,
               "Invalid size. Returning NULL.\n");
        return (Vector*) NULL;
    }
    Vector* new_vec = (Vector*) malloc(sizeof(Vector));
    if (!new_vec){
        return (Vector*) NULL;
    }
    cds_size pow = _log2(min_capacity) + 1;
    if (pow >= _MAX_POW2_){
        _raise(STATUS_INVALID_SIZE,
               LINEAR_DEFAULT_STATUS_ACTION_INVALID_SIZE,
               "Invalid minimum capacity.\n");
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

/** Destructor function for the dynamic structure. */
void vectorDelete(Vector* vec){
    if (!vec){
        return;
    }
    free(vec->container);
    free(vec);
}

void* vectorGetAt(const Vector* const vec, const size_t index){
    if (!_checkIndex(vec, C_VECTOR, index)){
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
        _raise(STATUS_TYPE_OVERFLOW,
               LINEAR_DEFAULT_STATUS_ACTION_TYPE_OVERFLOW,
               "Overflow of size. Container not expanded.\n");
        return false;
    }
    void* new_container = (void*) realloc(vec->container, new_capacity);
    if (!new_container){
        return false;
    }
    vec->container = new_container;
    vec->capacity = new_capacity;
    return true;
}

/** Push back function for the dynamic array structure */
cds_bool vectorPush(Vector* const vec, void* data){
    if ( ((double) vec->capacity) * _EXPANSION_RATE_CHECK <= (double) vec->length){
        (void) _vectorExpand(vec);
    }
    if (vec->length + 1 > vec->capacity){
        return false;
    }
    memmove(vec->container + vec->length*vec->data_size, data, vec->data_size);
    vec->length++;
    return true;
}

void* vectorUpdate(Vector* const vec, void* data, const cds_size index){
    if (!_checkIndex(vec, C_VECTOR, index)){
        return (void*) NULL;
    }
    void* _data = vectorGetAt(vec, index);
    memmove(vec->container + index*vec->data_size, data, vec->data_size);
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
*/
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

/**
 * TUPLES
 * ------
*/

Tuple* tupleCreate(const void* const arr, const size_t length, const size_t data_size){
    if (!arr || 0 == length){
        return (Tuple*) NULL;
    }
    Tuple* new_tuple = (Tuple*) malloc(sizeof(Tuple));
    if (!new_tuple){
        return (Tuple*) NULL;
    }
    void* container = (void*) malloc(length*data_size);
    if (!container){
        free(new_tuple);
        return (Tuple*) NULL;
    }
    memcpy(container, arr, length*data_size);
    new_tuple->container = (const void*) container;
    new_tuple->length = length;
    new_tuple->data_size = data_size;
    return new_tuple;
}

void tupleDelete(Tuple* tuple){
    if (!tuple){
        return;
    }
    free((void*) tuple->container);
    free(tuple);
}

const void* tupleGetAt(const Tuple* const tuple, const cds_size index){
    if (!_checkIndex(tuple, C_TUPLE, index)){
        return (void*) NULL;
    }
    const void* data = (const void*) CDS_BYTE_OFFSET(tuple->container, index*tuple->data_size);
    return data;
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

SLList sllCreate(const cds_size data_size){
    return (SLList) {
        .data_size = data_size,
        .length = 0,
        .head = (SLLNode*) NULL,
        .tail = (SLLNode*) NULL,
    };
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
    enum IterableType type;
    union {
        Vector* vec;
        Tuple* tuple;
        SLLNode* head;
    };
};

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
        new_iter->vec = (Vector*) container;
        break;
        case TUPLE:
        new_iter->tuple = (Tuple*) container;
        case SLLIST:
        new_iter->head = ((SLList*) container)->head;;
        break;
        default:
        break;
    }
    new_iter->type = type;
    new_iter->index = 0;
    return new_iter;
}

void iterDelete(Iter* iter){
    free(iter);
}

Iter* iterNext(Iter* iter){
    if (!iter){
        return (Iter*) NULL;
    }
    switch (iter->type){
        case VECTOR:
        if (iter->vec->length == iter->index){
            iterDelete(iter);
            return (Iter*) NULL;
        }else{
            iter->index++;
        }
        break;
        case TUPLE:
        if (iter->tuple->length == iter->index){
            iterDelete(iter);
            return (Iter*) NULL; 
        }else{
            iter->index++;
        }
        break;
        case SLLIST:
        if (!iter->head){
            iterDelete(iter);
            return (Iter*) NULL;
        }else{
            iter->head = iter->head->next;
            iter->index++;
        }
        break;
        default:
        break;
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
        data = vectorGetAt(iter->vec, iter->index);
        break;
        case TUPLE:
        data = tupleGetAt(iter->tuple, iter->index);
        break;
        case SLLIST:
        data = iter->head->data;
        break;
        default:
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
