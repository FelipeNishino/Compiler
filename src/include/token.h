#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#define FOREACH_TOKEN_TYPE(TOKEN) \
        TOKEN(token_identifier)   \
        TOKEN(token_literal_string)   \
        TOKEN(token_literal_number)   \
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
        TOKEN(token_TYPE)   \
        TOKEN(token_UNK)   \
        TOKEN(token_EOF)   \


#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum TokenType {
    FOREACH_TOKEN_TYPE(GENERATE_ENUM)
} TokenType;

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const TOKEN_TYPE_STRING[] = {
    FOREACH_TOKEN_TYPE(GENERATE_STRING)
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


static const char* const RESERVED_WORD_STRING[] = {"let", "var", "if", "else", "for", "while", "do", "return", "Int", "Float", "String", "Bool"};

typedef struct token {
    char* value;
    TokenType type;
    unsigned int line;
    unsigned int col;
} Token;

Token* init_token(char* value, TokenType type, unsigned int line, unsigned int col);
char* token_to_str(Token* token);

#endif //COMPILER_TOKEN_H
