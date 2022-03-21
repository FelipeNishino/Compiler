#include <stdio.h>
#include <stdlib.h>
#include "token.h"

void printToken(Token t) { printf("(Token: {%s}, Type: {%s}, line/col: {%d:%d})\n", t.token, TOKEN_TYPE_STRING[t.type], t.line, t.col); }

//insert link at the first location
TokenNode *tokenList_insertFirst(TokenNode *list, Token token) {
    //create a link
    TokenNode *link = (TokenNode *) malloc(sizeof(TokenNode ));

//    link->key = key;
    link->token = token;

    //point it to old first node
    link->next = list;

    //point first to new first node
    list = link;
    return list;
}

//is list empty
int tokenList_isEmpty(TokenNode *list) {
    return list == NULL;
}

int tokenList_length(TokenNode *list) {
    int length = 0;
    TokenNode *current;

    for(current = list; current != NULL; current = current->next) {
        length++;
    }

    return length;
}
        
void tokenList_printList(TokenNode *list) {
    TokenNode *ptr = list;
    printf("\n[ ");
    
    //start from the beginning
    while(ptr != NULL) {
        printToken(ptr->token);
        ptr = ptr->next;
    }
    
   printf(" ]\n");
}