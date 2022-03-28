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
