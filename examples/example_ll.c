#include<stdio.h>
#include "../include/linked_list.h"

Node* middle(Node* head){
    Node* slow = head;
    Node* fast = head;
    Node* result = NULL;
    while( fast != NULL){
        if (fast->next == NULL){
            insertLinkedList(&result, slow->data);
            printf("size odd\n");
            return result;
        } else{
            if ((fast->next)->next == NULL){
                insertLinkedList(&result, slow->data);
                printf("size even\n");
                slow = slow->next;
                break;
            } else {
                fast = (fast->next)->next;
            }
        }
        slow = slow->next;
    }
    insertLinkedList(&result, slow->data);
    return result;
}

int main(){
    Node* head = NULL;
    for (int i=0; i<8; i++){
        insertLinkedList(&head,i);
    }
    printLinkedList(head);
    Node* result = middle(head);
    printLinkedList(result);
    return 0;
}
