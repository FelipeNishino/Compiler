#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include "macros.h"

#define FOREACH_TOKEN(TOKEN) \
        TOKEN(token_identifier)   \
        TOKEN(token_literal_string)   \
        TOKEN(token_literal_float)   \
        TOKEN(token_literal_int)   \
        TOKEN(token_literal_true)   \
        TOKEN(token_literal_false)   \
        TOKEN(token_op_lt)   \
        TOKEN(token_op_lte)   \
        TOKEN(token_op_gt)   \
        TOKEN(token_op_gte)   \
        TOKEN(token_op_eq)   \
        TOKEN(token_op_neq)   \
        TOKEN(token_op_mod)   \
        TOKEN(token_op_plus)   \
        TOKEN(token_op_minus)   \
        TOKEN(token_op_arrow)   \
        TOKEN(token_op_multi)   \
        TOKEN(token_op_colon)   \
        TOKEN(token_op_pow)   \
        TOKEN(token_op_div)   \
        TOKEN(token_op_NOT)   \
        TOKEN(token_op_AND)   \
        TOKEN(token_op_OR)   \
        TOKEN(token_op_assignment)   \
        TOKEN(token_gp_ocb)   \
        TOKEN(token_gp_ccb)   \
        TOKEN(token_gp_osb)   \
        TOKEN(token_gp_csb)   \
        TOKEN(token_gp_op)   \
        TOKEN(token_gp_cp)   \
        TOKEN(token_gp_semicolon)   \
        TOKEN(token_declaration)   \
        TOKEN(token_attribution)   \
        TOKEN(token_reserved)   \
        TOKEN(token_LET)   \
        TOKEN(token_VAR)   \
        TOKEN(token_IF)   \
        TOKEN(token_ELSE)   \
        TOKEN(token_FOR)   \
        TOKEN(token_WHILE)   \
        TOKEN(token_DO)   \
        TOKEN(token_RETURN)   \
        TOKEN(token_type_INT)   \
        TOKEN(token_type_FLOAT)   \
        TOKEN(token_type_BOOL)   \
        TOKEN(token_type_STRING)   \
        TOKEN(token_comment_inline)   \
        TOKEN(token_UNK)   \
        TOKEN(token_EOF)   \

typedef enum TokenType {
    FOREACH_TOKEN(GENERATE_ENUM)
} TokenType;

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const TOKEN_STRING[] = {
    FOREACH_TOKEN(GENERATE_STRING)
};
/*
#define FOREACH_RESERVED_WORD(RESERVED_WORD) \
        RESERVED_WORD(RW_let) \
        RESERVED_WORD(RW_var) \
        RESERVED_WORD(RW_if) \
        RESERVED_WORD(RW_else) \
        RESERVED_WORD(RW_for) \
        RESERVED_WORD(RW_while) \
        RESERVED_WORD(RW_do) \
        RESERVED_WORD(RW_return) \
        RESERVED_WORD(RW_Int) \
        RESERVED_WORD(RW_Float) \
        RESERVED_WORD(RW_String) \
        RESERVED_WORD(RW_Bool) \

typedef enum ReservedWord {
    FOREACH_RESERVED_WORD(GENERATE_ENUM)
} ReservedWord;

static const char* const RESERVED_WORD_STRING[] = {
    FOREACH_RESERVED_WORD(GENERATE_STRING)
};
*/
#include <stdio.h>

static const char* const RESERVED_WORD_STRING[] = {"let", "var", "if", "else", "for", "while", "do", "return", "Int", "Float", "String", "Bool", "True", "False"};

typedef struct {
    int i;
    int n;
} SizePos;

typedef struct token {
    char* value;
    TokenType type;
    unsigned int line;
    unsigned int col;
} Token;

void token_print(Token* t);
SizePos* sizepos_init(int i, int n);
Token* token_init(char* value, TokenType type, unsigned int line, unsigned int col);
int token_is_relational_operator(Token* t);
int token_is_unary_operator(Token* t);
int token_is_lesser_precedence_operator(Token* t);
int token_is_higher_precedence_operator(Token* t);
int token_compare(Token* t1, Token* t2);
char* token_to_str(Token* token);

#endif //COMPILER_TOKEN_H
