#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/token.h"

void printToken(Token t) { printf("(Token: {%s}, Type: {%s}, line/col: {%d:%d})\n", t.value, TOKEN_TYPE_STRING[t.type], t.line, t.col); }

Token*
init_token(char* value, TokenType type, unsigned int line, unsigned int col) {
    Token* token = calloc(1, sizeof(Token));

    token->value = value;
    token->type = type;
    token->line = line;
    token->col = col;

    return token;
}

char* token_to_str(Token* t) {
    const char* type_str = TOKEN_TYPE_STRING[t->type];
    const char* template = "<type={%s}, int_type={%d}, value={%s}>";

    char* str = calloc(strlen(type_str) + strlen(template) + 8, sizeof(char));
    sprintf(str, template, type_str, t->type, t->value);

    return str;
}
// //insert link at the first location
// TokenNode *tokenList_insertFirst(TokenNode *list, Token token) {
//     //create a link
//     TokenNode *link = (TokenNode *) malloc(sizeof(TokenNode ));

// //    link->key = key;
//     link->token = token;

//     //point it to old first node
//     link->next = list;

//     //point first to new first node
//     list = link;
//     return list;
// }

// //is list empty
// int tokenList_isEmpty(TokenNode *list) {
//     return list == NULL;
// }

// int tokenList_length(TokenNode *list) {
//     int length = 0;
//     TokenNode *current;

//     for(current = list; current != NULL; current = current->next) {
//         length++;
//     }

//     return length;
// }
        
// void tokenList_printList(TokenNode *list) {
//     TokenNode *ptr = list;
//     printf("\n[ ");
    
//     //start from the beginning
//     while(ptr != NULL) {
//         printToken(ptr->token);
//         ptr = ptr->next;
//     }
    
//    printf(" ]\n");
// }
