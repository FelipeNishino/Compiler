#include <stdio.h>
#include <stdlib.h>
#include "token.h"

//insert link at the first location
void insertFirst(TokenNode *list, Token token) {
    //create a link
    TokenNode *link = (TokenNode *) malloc(sizeof(TokenNode ));

//    link->key = key;
    link->token = token;

    //point it to old first node
    link->next = list;

    //point first to new first node
    list = link;
}

//is list empty
int isEmpty(TokenNode *list) {
    return list == NULL;
}

int length(TokenNode *list) {
    int length = 0;
    TokenNode *current;

    for(current = list; current != NULL; current = current->next) {
        length++;
    }

    return length;
}
        
void printTokenList(TokenNode *list) {
    TokenNode *ptr = list;
    printf("\n[ ");
    
    //start from the beginning
    while(ptr != NULL) {
        printToken(ptr->token);
        ptr = ptr->next;
    }
    
   printf(" ]\n");
}