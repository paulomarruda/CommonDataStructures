#include <stdio.h>
#include "../include/hash.h"

#define MAX_N 30

static size_t fib_calc(size_t N, HashTable* previous){
    if (htSearch(previous, &N)){
        return *(size_t*) htGet(previous, &N, false);
    }
    return fib_calc(N-1,previous) + fib_calc(N-2,previous);
}

static size_t fib(size_t N){
    HashTable* previous = htCreate(FNV1aHash, MAX_N, true, INT_KEY);
    const size_t N0 = 0;
    const size_t N1 = 1;
    const size_t f0 = 0;
    const size_t f1 = 1;
    htSet(previous, &N0, sizeof(size_t), &f0, sizeof(size_t));
    htSet(previous, &N1, sizeof(size_t), &f1, sizeof(size_t));
    
    size_t fib = fib_calc(N, previous);
    htDelete(previous);
    return fib;
}

int main(int argc, char** argv){
    printf("%d\n", argc);
    if (2 != argc){
        fprintf(stderr, "Number required. Usage: fib.out NUM.\n");
        exit(EXIT_FAILURE);
    }
    size_t N = (size_t) strtoll(argv[1],(char**) NULL, 10);
    if (N > MAX_N){
        fprintf(stderr, "Max number for calculation is %d\n", MAX_N);
        exit(EXIT_FAILURE);
    }
    size_t fib_N = fib(N);
    fprintf(stdout, "Fib(%zu) = %zu\n", N, fib_N);
    return 0;
}
