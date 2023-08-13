/*--------------------------------
 *          STACKS 
 *--------------------------------
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/linear.h"


/*--------------------------------
 *          LINKED LIST 
 *--------------------------------
 * 
*/

typedef struct Node{
    int data;
    struct Node* next;
} Node;

typedef struct LinkedList{
    Node* head;
} LinkedList;

LinkedList* createLinkedList(){
    LinkedList* linked_list = malloc(sizeof(LinkedList));

}

/*
 *----------------------------------------
 * IMPLEMENTATION OF A STACK USING ARRAYS
 * --------------------------------------
*/
typedef struct Stack{
    int* collection;
    int capacity;
    int size;
} Stack;

Stack* createStack(const int capacity){
    if (capacity <= 0) return NULL;

    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;
    
    stack->collection = malloc(sizeof(int)*capacity);
    if (stack->collection == NULL){
        free(stack);
        return NULL;
    }
    stack->capacity = capacity;
    stack->size = 0;

    return stack;
}

void destroyStack(Stack* stack){
    free(stack->collection);
    free(stack);
}

bool isFull(const Stack *stack){
    return stack->capacity == stack->size;
}

bool isEmpty(const Stack* stack){
    return stack->size == 0;
}

bool popItem(Stack* stack, int* item){
    if (isEmpty(stack)) return false;
    stack->size--;
    *item = stack->collection[stack->size];
    return true;
}
bool peek(const Stack* stack, int* item){
    if(isEmpty(stack)) return false;
    *item = stack->collection[stack->size-1];
    return true;
}

bool pushItem(Stack* stack, const int item){
    if (isFull(stack)) return false;
    stack->collection[stack->size] = item;
    stack->size++;
    return true;
}

int main(){
    Stack* stack = createStack(5);
    if (stack == NULL){
        printf("Error creating stack\n");
        return 1;
    }
    int i = 0;
    while(pushItem(stack, 10*i)){ 
        printf("Item %d added!\n", 10*i);
        i++;
    }
    printf("Size: %d\n", stack->size);

    if (isFull(stack)) printf("stack is full\n");

    int* pitem;
    while (popItem(stack, pitem)){
        printf("Item %d removed!\n", *pitem);
    }
    if (isEmpty(stack)) printf("Stack is empty\n");
    return 0;
}


/*--------------------------------
 *          QUEUES 
 *--------------------------------
 * 
*/

typedef struct Queue{
    int* collection;
    int capacity;
    int size;
} Queue;


