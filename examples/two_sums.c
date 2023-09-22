/**
 * @brief This is LeetCode problem no. Two Sums:
 * Given an array `arr` and an integer `target`, 
 * returns the indexes of the elements that sum up to 
 * `target`. Each array is assumed to have one and only
 * one pair of such kind. The array's length is between 
 * 2 and 10^9 and the integer elements are between -10^9
 * and 10^9. 
**/
#include <time.h>
#include <stdio.h>
#include "../include/hash.h"
#include "../include/hash_fun.h"

int* twoSum(const int* arr, const size_t arr_len, const int target);

int main(void){
    int arr[] = {-5,-8,-7,0,3,8,2,4}; 
    size_t len = sizeof(arr)/sizeof(int);
    int target = -4;// 2 & 4
    int* result = twoSum(arr, len, target);
    printf("[%d,%d]", arr[0], arr[1]);
    free(result);
    return 0;
}

int* twoSum(const int* arr, const size_t arr_len, const int target){
    HashTable* ht = htCreate(FNV1aHash, arr_len, true, INT_KEY);
    int* answer = malloc(2*sizeof(int));
    if (!answer){
        return (int*) NULL;
    }
    answer[0] = -1;
    answer[1] = -1;
    for (int i=0; i<arr_len; i++){
        int complement = target - arr[i];
        void* data = htGet(ht, &complement, false);
        if (data){
            answer[0] = i;
            answer[1] = *(int*) data;
            break;
        }else{
            htSet(ht, (void*) &arr[i], &i, sizeof(size_t));
        }
    }
    htDelete(ht);
    return answer;
}
