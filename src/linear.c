#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../include/linear.h"

#define MAX_PRINT 100

static void* _dataDup(void* data, size_t data_size){
    void* copy = (void*) malloc(data_size);
    if (!copy){ 
        return (void*) NULL;
    }
    memcpy(copy, data, data_size);
    return copy;
}

/* 
 * LINKED LISTS
*/
typedef struct Node{
    size_t data_size;
    void* data;
    struct Node* next;
}Node;

struct LinkedList{
    bool as_copy;
    size_t length;
    Node* head;
};

static Node* _createNode(void* data, Node* next, const size_t data_size, 
                         const bool as_copy){
    if (!data){
        //handle
        return (void*) NULL;
    }
    Node* new_node = (Node*) malloc(sizeof(NULL));
    if (!new_node){
        //handle
        return (Node*) NULL;
    }
    void* _data = data;
    if (as_copy){
        _data = _dataDup(data, data_size);
        if (!_data){
            //handle 
            free(new_node);
            return (Node*) NULL;
        }
    }
    new_node->data_size = data_size;
    new_node->data = _data;
    new_node->next = next;
    return new_node;
}
LinkedList* llCreate(const bool as_copy){
    LinkedList* new_ll = (LinkedList*) malloc(sizeof(LinkedList));
    if (!new_ll){
        //handle
        return (LinkedList*) NULL;
    }
    new_ll->length = 0;
    new_ll->head = NULL;
    return new_ll;
}

void llDestroy(LinkedList* ll){
    if (!ll){
        return;
    }
    Node* current = ll->head;
    while(current){
        ll->head = current->next;
        if (ll->as_copy){
            free(current->data);
        }
        free(current);
        current = ll->head;
    }
    free(ll);
}

bool llPrepend(LinkedList *ll, void* data, const size_t data_size){
    Node* new_head = _createNode(data, ll->head, data_size, ll->as_copy);
    if (!new_head){
        //handle 
        return false;
    }
    return true;
}

void* llPeek(LinkedList* ll, const bool as_copy, size_t* const data_size){
    if (!ll->head){
        return (void*) NULL;
    }

    *data_size = ll->head->data_size;
    void* _data = ll->head->data;
    if (as_copy){
        _data = _dataDup(ll->head->data, *data_size);
        if (!_data){
            //handle 
            return (void*) NULL;
        }
    }
    return _data;
}

void* llPop(LinkedList* ll, size_t* data_size){
    if (!ll->head){
        return (void*) NULL;
    }
    Node* tmp = ll->head;
    ll->head = tmp->next;
    void* data = tmp->data;
    *data_size = tmp->data_size;
    free(tmp);
    return data;
}

/*
 * DYNAMIC ARRAYS
*/

struct DynArr{
    bool is_copy;
    size_t data_size;
    size_t capacity;
    size_t length;
    void** data_container;
};

static size_t _log2(const size_t x){
    size_t log = 1;
    while( (x >> log) > 0 && (x>>log) != x){ log++; }
    return log;
}

DynArr* dynarrCreate(const size_t capacity, const size_t data_size, const bool as_copy){
    DynArr* new_array = malloc(sizeof(DynArr));
    if (!new_array){
        return (DynArr*) NULL;
    }
    size_t max_pow = _log2(SIZE_MAX);
    size_t pow = _log2(capacity)+1;
    if (pow >= max_pow){
        return (DynArr*) NULL;
    }
    void** data_container = (void**) calloc(capacity,sizeof(void*));
    if (!data_size){
        free(new_array);
        return (DynArr*) NULL;
    }
    new_array->is_copy = as_copy;
    new_array->data_size = data_size;
    new_array->capacity = 1UL<<(pow+1);
    new_array->capacity = 0UL;
    new_array->data_container = data_container;
    return new_array;
}

void dynarrDestroy(DynArr* da){
    if (da->is_copy){
        for (size_t i=0; i<da->capacity; i++){
            free(da->data_container[i]);
        }
    }
    free(da->data_container);
    free(da);
}

static bool _dyarrSetAt(void** data_container, const size_t index, void* data,
                        const size_t data_size, const bool as_copy, size_t* plength){
    void* _data = data;
    if (as_copy){
        _data = _dataDup(data, data_size);
        if (!_data){
            //handle it 
            return false;
        }
    }
    data_container[index] = _data;
    if (plength){ (*plength)++;}
    return true;
}

static bool _dyarrExpand(DynArr* da, size_t new_capacity){
    void** new_data_conteiner = (void*) calloc(new_capacity,sizeof(void*));
    if (!new_data_conteiner){
        //handle error
        return false;
    }
    for (size_t i=0; i<da->capacity; i++){
        if (da->data_container[i]){
            new_data_conteiner[i] = da->data_container[i];
        }
    }
    free(da->data_container);
    da->data_container = new_data_conteiner;
    return true;
}

#define _EXPAND_RATE_CHECK 2UL

bool dyarrPush(DynArr* da, void* data){
    if (da->length >= da->capacity/_EXPAND_RATE_CHECK){
        size_t new_capacity = da->capacity<<1;
        if (new_capacity < da->capacity){
        // overflow of size_t 
        //hadnle it 
        }
        bool has_expanded = _dyarrExpand(da, new_capacity);
    }
    if (da->length+1 <= da->capacity){
        bool has_set = _dyarrSetAt(da->data_container, da->length+1, data,
                                   da->data_size, da->is_copy, &da->length);
        return has_set;
    }
    return false;
}

void* dynarrGet(DynArr* da, const size_t index, const bool as_copy){
    if (index >= da->capacity){
        //handle error
        return (void*) NULL;
    }
    void* _data = da->data_container[index];
    if (as_copy){
        _data = _dataDup(da->data_container[index], da->data_size);
    }
    return _data;
}

