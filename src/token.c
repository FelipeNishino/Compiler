#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/token.h"

void token_print(token* t) { fprintf(stderr, "<token={%s}, type={%s}, pos={%d:%d}>\n", t->value, TOKEN_STRING[t->type], t->line, t->col); }

size_pos*
sizepos_init(int i, int n) {
    size_pos* sp = calloc(1, sizeof(size_pos));

    sp->i = i;
    sp->n = n;

    return sp;
}

token*
token_init(char* value, token_type t_type, unsigned int line, unsigned int col) {
    token* token = calloc(1, sizeof(token));

    token->value = value;
    token->type = t_type;
    token->line = line;
    token->col = col;

    return token;
}

int
token_is_relational_operator(token* t) {
    return t->type == token_op_lt || t->type == token_op_lte || t->type == token_op_gt || t->type == token_op_gte || t->type == token_op_eq || t->type == token_op_neq;
}

int
token_is_unary_operator(token* t) {
    return t->type == token_op_plus || t->type == token_op_minus || t->type == token_op_NOT;
}

int
token_is_lesser_precedence_operator(token* t) {
    return t->type == token_op_plus || t->type == token_op_minus || t->type == token_op_OR;
}

int
token_is_higher_precedence_operator(token* t) {
    return t->type == token_op_div || t->type == token_op_mod || t->type == token_op_multi || t->type == token_op_pow || t->type == token_op_AND;
}

int
token_compare(token* t1, token* t2) {
    return strcmp(t1->value, t2->value) == 0 && t1->type == t2->type && t1->line == t2->line && t1->col == t2->col;
}


char* token_to_str(token* t) {
    const char* type_str = TOKEN_STRING[t->type];
    const char* template = "<type={%s}, int_type={%d}, value={%s}>";

    char* str = calloc(strlen(type_str) + strlen(template) + 8, sizeof(char));
    sprintf(str, template, type_str, t->type, t->value);

    return str;
}
