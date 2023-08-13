#pragma once
#include <stdlib.h>
#include <stdbool.h>

#ifndef LINEAR_H
#define LINEAR_H
/*--------------------------------
 *          STACKS 
 *--------------------------------
 * 
*/
typedef struct Stack Stack;

Stack* createStack(const int capacity);

void destroyStack(Stack* stack);

bool isFull(const Stack* stack);

bool isEmpty(const Stack* stack);

bool peek(const Stack* stack, int* item);

bool popItem(Stack* stack, int* item);

bool pushItem(Stack* stack, const int item);

/*--------------------------------
 *          QUEUES 
 *--------------------------------
 * 
*/

typedef struct Queue Queue;


/*--------------------------------
 *          LINKED LIST 
 *--------------------------------
 * 
*/

typedef struct Node Node;

void destroyNode(Node* node);

typedef struct LinkedList LinkedList;

void destroyNode(Node* node);

LinkedList* createLinkedList();

void destroyLinkedList(LinkedList* linked_list);

bool insertLinkedList(LinkedList* linked_list, const int item);

bool popItemLinkedList(LinkedList* linked_list, const int item);

#endif
