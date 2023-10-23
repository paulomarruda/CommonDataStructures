#include <stdio.h>
#include "../include/common.h"

#define ERROR_MESSAGE(internal_message, message, file) fprintf(file, internal_message, message)

struct CDSStatus CDS_ERROR = {
    false,
    STATUS_SUCCESS,
    ACTION_EXIT,
    ""
};

struct CDSStatus CDS_WARNING = {
    false,
    STATUS_SUCCESS,
    ACTION_WARN,
    ""
};

void _raise(enum StatusType type, enum StatusAction action, const char* const message){
    switch (action){
        case ACTION_EXIT:
            switch (type){
                case STATUS_SUCCESS:
                    break;
                case STATUS_ALLOCATION_FAILED:
                    ERROR_MESSAGE("ALLOCATION ERROR: %s", message, stderr);
                    break;
                case STATUS_TYPE_OVERFLOW:
                    break;
                case STATUS_MAX_CAPACITY_REACHED:
                    break;
                case STATUS_INVALID_SIZE:
                    break;
                case STATUS_INVALID_RANGE:
                    break;
                case STATUS_NULL_STRUCT:
                    break;
            }
            exit(EXIT_FAILURE);
        case ACTION_WARN:           
            switch (type){
                case STATUS_SUCCESS:
                    break;
                case STATUS_ALLOCATION_FAILED:
                    break;
                case STATUS_TYPE_OVERFLOW:
                    break;
                case STATUS_MAX_CAPACITY_REACHED:
                    break;
                case STATUS_INVALID_SIZE:
                    break;
                case STATUS_INVALID_RANGE:
                    break;
                case STATUS_NULL_STRUCT:
                    break;
            }
            break;
    }
}

size_t _log2(const size_t x){
    size_t log = 1;
    while (x >> log && log <= _MAX_POW2_){
        log++;
    }
    return log - 1;
}

const void* _dataDup(const void* const data, const size_t data_size){
    void* dup = (void*) malloc(data_size);
    if (!dup){
        //handle erros
        return (void*) NULL;
    }
    memcpy(dup, data, data_size);
    return (const void*) dup;
}

size_t _strLen(const char* s){
    if (!s){
        return 0;
    }
    size_t len = 0;
    while (s[len] != '\0'){
        len++;
    }
    return len;
}

const char* _strDup(const char* string){
    size_t len = _strLen(string);
    const char* new_str = (char*) malloc(len+1 * sizeof(char));
    if (!new_str){ 
        return (char*) NULL; 
    }
    memcpy( (void*) new_str, string, len+1);
    return new_str;
}
