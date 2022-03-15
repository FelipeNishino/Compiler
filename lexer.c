#include "lexer.h"

//insert link at the first location
void insertFirst(Token token) {
    //create a link
    RegisterNode *link = (RegisterNode *) malloc(sizeof(RegisterNode ));

//    link->key = key;
    link->reg = token;

    //point it to old first node
    link->next = head;

    //point first to new first node
    head = link;
}

//delete first item
RegisterNode *deleteFirst() {

    //save reference to first link
    RegisterNode *tempLink = head;

    //mark next to first link as first
    head = head->next;

    //return the deleted link
    return tempLink;
}

//is list empty
int isEmpty() {
    return head == NULL;
}

int length() {
    int length = 0;
    RegisterNode *current;

    for(current = head; current != NULL; current = current->next) {
        length++;
    }

    return length;
}

//find a link with given key
RegisterNode *find(char *key) {

    //start from the first link
    RegisterNode *current = head;

    //if list is empty
    if(head == NULL) {
        return NULL;
    }

    //navigate through list
    while(strcmp(current->reg.name, key) != 0) {
        //if it is last node
        if(current->next == NULL) {
            return NULL;
        } else {
            //go to next link
            current = current->next;
        }
    }

    //if token found, return the current Link
    return current;
}

//delete a link with given key
RegisterNode *delete(char key) {

    //start from the first link
    RegisterNode *current = head;
    RegisterNode *previous = NULL;

    //if list is empty
    if(head == NULL) {
        return NULL;
    }

    //navigate through list
    while(strcmp(current->reg.name, key) != 0) {

        //if it is last node
        if(current->next == NULL) {
            return NULL;
        } else {
            //store reference to current link
            previous = current;
            //move to next link
            current = current->next;
        }
    }

    //found a match, update the link
    if(current == head) {
        //change first to point to next link
        head = head->next;
    } else {
        //bypass the current link
        previous->next = current->next;
    }

    return current;
}

Token* tokenize(char* input) {
    
}