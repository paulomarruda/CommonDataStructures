#include <time.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/hash.h"

#define NUM_LINES 100000000000UL

static char* generateHex(void){
    static const char hexdigit[16] = "0123456789abcdef";
    char* hex = (char*) malloc(65*sizeof(char));
    if (!hex){ return (char*) NULL; }
    for (size_t i=0; i<64; i++){
        size_t index = rand() % 16;
        hex[i] = hexdigit[index];
    }
    hex[64] = '\0';
    return hex;
}

#define HEX_NOT_GENERATED "HEX NOT GENERATED"

static char** generateKeys(void){
    char** keys = (char**) malloc(NUM_LINES*sizeof(char*));
    if (!keys){ return (char**) NULL; }
    for (size_t i=0; i<NUM_LINES; i++){
        keys[i] = generateHex();
        if (!keys[i]){
            keys[i] = HEX_NOT_GENERATED;
        }
    }
    return keys;
}

static int* generateArray(const size_t len){
    int* arr = (int*) malloc(len*sizeof(int));
    if (!arr){
        return NULL;
    }
    for (size_t i=0; i<len; i++){
        arr[i] = i;
    }
    return arr;
}

static HashTable* generateHexTable(int* arr, const size_t capacity){
    HashTable* ht = htCreate(FNV1aHash, capacity, false, STR_KEY);
    char* key;
    for (size_t i=0; i<capacity; i++){
        key = generateHex();
        htSet(ht, key, &arr[i], sizeof(int));
    }
    return ht;
}
#define MAX_PRINT 20
int main(){
    srand(time(0));
    size_t i = 0;
    int* arr = generateArray(NUM_LINES);
    HashTable* ht = generateHexTable(arr, NUM_LINES);
    for (HashTableIter* iter = htiterCreate(ht); iter; iter = htiterNext(iter)){
        char* key = (char*) htGetKeyFromIter(iter);
        int* data = (int*) htGetDataFromIter(iter);
        if (data)
            printf("%s: %d\n", key, *data);
        i++;
        if (i > MAX_PRINT)
            break;
    }
    htDelete(ht);

    return 0;
}
