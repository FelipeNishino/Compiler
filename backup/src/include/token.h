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

static const char *TOKEN_TYPE_STRING[] = {
    FOREACH_TOKEN_TYPE(GENERATE_STRING)
};

#include <stdio.h>

static const char* RESERVED_WORDS[] = {"let", "var"};

typedef struct token {
    char* value;
    TokenType type;
    unsigned int line;
    unsigned int col;
} Token;

Token* init_token(char* value, TokenType type, unsigned int line, unsigned int col);
char* token_to_str(Token* token);

// typedef struct tokenNode {
//     Token token;
//     struct tokenNode *next;
// } TokenNode;

// void printToken(Token t);

// //insert link at the first location
// TokenNode *tokenList_insertFirst(TokenNode *list, Token token);

// // Check if list is empty
// int tokenList_isEmpty(TokenNode *list);

// // return number of nodes in list
// int tokenList_length(TokenNode *list);

// void tokenList_printList(TokenNode *list);

#endif //COMPILER_TOKEN_H
