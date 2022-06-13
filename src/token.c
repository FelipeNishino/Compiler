#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/token.h"

void token_print(Token* t) { fprintf(stderr, "<token={%s}, type={%s}, pos={%d:%d}>\n", t->value, TOKEN_STRING[t->type], t->line, t->col); }

SizePos*
sizepos_init(int i, int n) {
    SizePos* sp = calloc(1, sizeof(SizePos));

    sp->i = i;
    sp->n = n;

    return sp;
}

Token*
token_init(char* value, TokenType type, unsigned int line, unsigned int col) {
    Token* token = calloc(1, sizeof(Token));

    token->value = value;
    token->type = type;
    token->line = line;
    token->col = col;

    return token;
}

int
token_is_relational_operator(Token* t) {
    return t->type == token_op_lt || t->type == token_op_lte || t->type == token_op_gt || t->type == token_op_gte || t->type == token_op_eq || t->type == token_op_neq;
}

int
token_is_unary_operator(Token* t) {
    return t->type == token_op_plus || t->type == token_op_minus || t->type == token_op_NOT;
}

int
token_is_lesser_precedence_operator(Token* t) {
    return t->type == token_op_plus || t->type == token_op_minus || t->type == token_op_OR;
}

int
token_is_higher_precedence_operator(Token* t) {
    return t->type == token_op_div || t->type == token_op_mod || t->type == token_op_multi || t->type == token_op_pow || t->type == token_op_AND;
}

int
token_compare(Token* t1, Token* t2) {
    return strcmp(t1->value, t2->value) == 0 && t1->type == t2->type && t1->line == t2->line && t1->col == t2->col;
}


char* token_to_str(Token* t) {
    const char* type_str = TOKEN_STRING[t->type];
    const char* template = "<type={%s}, int_type={%d}, value={%s}>";

    char* str = calloc(strlen(type_str) + strlen(template) + 8, sizeof(char));
    sprintf(str, template, type_str, t->type, t->value);

    return str;
}
