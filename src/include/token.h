#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#define FOREACH_TOKEN_TYPE(TOKEN) \
        TOKEN(token_identifier)   \
        TOKEN(token_literal)  \
        TOKEN(token_operator)   \
        TOKEN(token_grouper)  \
        TOKEN(token_declaration)  \
        TOKEN(token_attribution)  \
        TOKEN(token_reserved)  \
        TOKEN(token_UNK)  \
        TOKEN(token_EOF)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum TokenType {
    FOREACH_TOKEN_TYPE(GENERATE_ENUM)
} TokenType;

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const TOKEN_TYPE_STRING[] = {
    FOREACH_TOKEN_TYPE(GENERATE_STRING)
};

#include <stdio.h>

static const char* const RESERVED_WORDS[] = {"let", "var"};

typedef struct token {
    char* value;
    TokenType type;
    unsigned int line;
    unsigned int col;
} Token;

Token* init_token(char* value, TokenType type, unsigned int line, unsigned int col);
char* token_to_str(Token* token);

#endif //COMPILER_TOKEN_H
